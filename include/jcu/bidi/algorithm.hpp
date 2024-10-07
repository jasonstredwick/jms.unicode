// Copyright © 2016-2018 Muhammad Tayyab Akram

#pragma once


#include <algorithm>
#include <concepts>
#include <cstdint>
#include <iterator>
#include <span>
#include <stdexcept>
#include <utility>
#include <vector>

#include "jcu/bidi/bidi_type.hpp"
#include "jcu/bidi/level.hpp"
#include "jcu/bidi/bidi_chain.hpp"
#include "jcu/bidi/status_stack.hpp"
#include "jcu/bidi/run_queue.hpp"
#include "jcu/bidi/isolating_run.hpp"
#include "jcu/bidi/runs.hpp"
#include "jcu/data/bidi_type.hpp"
#include "jcu/utf/utf.hpp"


namespace jcu::bidi {


BidiLevel DetermineBaseLevel(const BidiChain&, BidiLink, BidiLink, BidiLevel, bool);
template <typename Range_t> requires jcu::utf::IsUTF32CompatibleReduced_c<std::ranges::range_value_t<Range_t>>
void DetermineLevels(Range_t&&, BidiChain&, BidiLevel);
BidiLink SkipIsolatingRun(const BidiChain&, BidiLink, BidiLink);


std::vector<Run> ToRuns(jcu::utf::IsCompatibleRange_c auto&& code_points_rng,
                        BidiLevel base_level=LEVEL_TYPE_DEFAULT_LTR,
                        bool reserve=false) {
    if (code_points_rng.begin() == code_points_rng.end()) { return {}; }

    jcu::utf::CodePointView code_points_view{std::forward<decltype(code_points_rng)>(code_points_rng)};

    std::vector<BidiType> bidi_types{};
    if constexpr (std::ranges::sized_range<decltype(code_points_view)>) {
        bidi_types.reserve(code_points_view.size());
    } else {
        if (reserve) { bidi_types.reserve(std::ranges::distance(code_points_view)); }
    }
    std::ranges::transform(code_points_view, std::back_inserter(bidi_types), BidiTypeData::Lookup);

    BidiChain bidi_chain{bidi_types};

    BidiLevel resolved_level = base_level;
    if (base_level >= LEVEL_TYPE_MAX) {
        resolved_level = DetermineBaseLevel(bidi_chain,
                                            bidi_chain.Roller(),
                                            bidi_chain.Roller(),
                                            (base_level != LEVEL_TYPE_DEFAULT_RTL ? 0 : 1),
                                            false);
    }

    // tmp until we get it working and factor out isolating_run dep on code_points.  maybe a map?
    std::vector<char32_t> code_points{};
    std::ranges::transform(code_points_view, std::back_inserter(code_points), std::identity{});
    DetermineLevels(code_points, bidi_chain, resolved_level);

    // Save levels
    std::vector<BidiLevel> levels(bidi_types.size(), LEVEL_TYPE_INVALID);
    BidiLevel level = resolved_level;
    size_t index = 0; // skip first entry - nil starter value?; SaveLevels(&context->bidiChain, ++paragraph->fixedLevels, resolvedLevel);
    const BidiLink roller = bidi_chain.Roller();
    for (BidiLink link = bidi_chain.links[roller]; link != roller; link = bidi_chain.links[link]) {
        size_t offset = BidiChainGetOffset(link);
        for (; index < offset; index++) {
            levels[index] = level;
        }
        level = bidi_chain.GetLevel(link);
    }

    return CreateRuns(bidi_types, levels, resolved_level); // or should it be base_level ... double check
}


BidiLevel DetermineBaseLevel(const BidiChain& bidi_chain,
                             BidiLink skip_link,
                             BidiLink break_link,
                             BidiLevel default_level,
                             bool is_isolate)
{
    BidiLink link = bidi_chain.GetNext(skip_link);
    bool is_done = false;

    /* Rules P2, P3 */
    for (; link != break_link; link = bidi_chain.GetNext(link)) {
        BidiType type = bidi_chain.GetType(link);

        switch (type) {
        case BidiType::L:
            return 0;

        case BidiType::AL:
        case BidiType::R:
            return 1;

        case BidiType::LRI:
        case BidiType::RLI:
        case BidiType::FSI:
            link = SkipIsolatingRun(bidi_chain, link, break_link);
            if (link == BIDI_LINK_NONE) { is_done = true; }
            break;

        case BidiType::PDI:
            // In case of isolating run, the PDI will be the last code point.
            // *NOTE: The inner isolating runs will be skipped by the case above this one.
            if (is_isolate) { is_done = true; }
            break;
        }

        if (is_done) { break; }
    }

    return default_level;
}


template <typename T>
requires jcu::utf::IsUTF32CompatibleReduced_c<std::ranges::range_value_t<T>>
void DetermineLevels(T&& code_points, BidiChain& bidi_chain, BidiLevel base_level) {
    const BidiLink roller = bidi_chain.Roller();
    StatusStack status_stack{};
    RunQueue run_queue{};
    IsolatingRun isolating_run{};

    BidiLink prior_link = roller;
    BidiLink first_link = BIDI_LINK_NONE;
    BidiLink last_link = BIDI_LINK_NONE;
    BidiLevel prior_level = base_level;
    BidiType start_of_run = BidiType::NIL;
    BidiType end_of_run = BidiType::NIL;

    /* Rule X1 */
    size_t over_isolate = 0;
    size_t over_embedding = 0;
    size_t valid_isolate = 0;

    auto PushEmbedding = [&status_stack, &over_isolate, &over_embedding](auto level, auto override_status) mutable {
        if (level <= LEVEL_TYPE_MAX && !over_isolate && !over_embedding) {
            status_stack.Push(level, override_status, false);
        } else if (!over_isolate) { over_embedding += 1; }
    };

    auto PushIsolate = [&](auto level, auto override_status, auto link) mutable -> bool {
        BidiType prior_status = status_stack.GetOverrideStatus();

        bidi_chain.SetLevel(link, status_stack.GetEmbeddingLevel());

        if (level <= LEVEL_TYPE_MAX && !over_isolate && !over_embedding) {
            valid_isolate += 1;
            status_stack.Push(level, override_status, true);
        } else { over_isolate += 1; }

        if (prior_status != BidiType::ON) {
            bidi_chain.SetType(link, prior_status);
            if (bidi_chain.MergeIfEqual(prior_link, link)) { return true; }
        }

        return false;
    };

    status_stack.Push(base_level, BidiType::ON, false);
    for (BidiLink link = bidi_chain.links[roller]; link != roller; link = bidi_chain.links[link]) {
        bool force_finish = false;
        bool bn_equivalent = false;
        BidiType type = bidi_chain.GetType(link);

        switch (type) {
        /* Rule X2 */
        case BidiType::RLE:
            PushEmbedding(status_stack.LeastGreaterOddLevel(), BidiType::ON);
            bn_equivalent = true;
            break;

        /* Rule X3 */
        case BidiType::LRE:
            PushEmbedding(status_stack.LeastGreaterEvenLevel(), BidiType::ON);
            bn_equivalent = true;
            break;

        /* Rule X4 */
        case BidiType::RLO:
            PushEmbedding(status_stack.LeastGreaterOddLevel(), BidiType::R);
            bn_equivalent = true;
            break;

        /* Rule X5 */
        case BidiType::LRO:
            PushEmbedding(status_stack.LeastGreaterEvenLevel(), BidiType::L);
            bn_equivalent = true;
            break;

        /* Rule X5a */
        case BidiType::RLI:
            if (PushIsolate(status_stack.LeastGreaterOddLevel(), BidiType::ON, link)) { continue; }
            break;

        /* Rule X5b */
        case BidiType::LRI:
            if (PushIsolate(status_stack.LeastGreaterEvenLevel(), BidiType::ON, link)) { continue; }
            break;

        /* Rule X5c */
        case BidiType::FSI:
        {
            bool isRTL = DetermineBaseLevel(bidi_chain, link, roller, 0, true) == 1;
            BidiLevel which_level = isRTL ? status_stack.LeastGreaterOddLevel() : status_stack.LeastGreaterEvenLevel();
            if (PushIsolate(which_level, BidiType::ON, link)) { continue; }
            break;
        }

        /* Rule X6 */
        default:
            bidi_chain.SetLevel(link, status_stack.GetEmbeddingLevel());
            if (status_stack.GetOverrideStatus() != BidiType::ON) {
                bidi_chain.SetType(link, status_stack.GetOverrideStatus());
                if (bidi_chain.MergeIfEqual(prior_link, link)) { continue; }
            }
            break;

        /* Rule X6a */
        case BidiType::PDI:
        {
            if      (over_isolate  != 0) { over_isolate -= 1; }
            else if (valid_isolate == 0) { /* Do nothing */ }
            else {
                over_embedding = 0;

                while (!status_stack.GetIsolateStatus()) { status_stack.Pop(); }
                status_stack.Pop();
                valid_isolate -= 1;
            }

            bidi_chain.SetLevel(link, status_stack.GetEmbeddingLevel());
            BidiType overrideStatus = status_stack.GetOverrideStatus();
            if (overrideStatus != BidiType::ON) {
                bidi_chain.SetType(link, overrideStatus);
                if (bidi_chain.MergeIfEqual(prior_link, link)) { continue; }
            }
            break;
        }

        /* Rule X7 */
        case BidiType::PDF:
            bn_equivalent = true;

            if      (over_isolate   != 0) { /* Do nothing */ }
            else if (over_embedding != 0) { over_embedding -= 1; }
            else if (!status_stack.GetIsolateStatus() && status_stack.Size() >= 2) {
                status_stack.Pop();
            }
            break;

        /* Rule X8 */
        case BidiType::B:
            /*
             * These values are reset for clarity, in this implementation B can only occur as the
             * last code in the array.
             */
            status_stack.Clear();
            status_stack.Push(base_level, BidiType::ON, false);

            over_isolate = 0;
            over_embedding = 0;
            valid_isolate = 0;

            bidi_chain.SetLevel(link, base_level);
            break;

        case BidiType::BN:
            bn_equivalent = true;
            break;

        case BidiType::NIL:
            force_finish = true;
            bidi_chain.SetLevel(link, base_level);
            break;
        }

        /* Rule X9 */
        if (bn_equivalent) {
            /* The type of this link is BN equivalent, so abandon it and continue the loop. */
            bidi_chain.SetType(link, BidiType::BN);
            bidi_chain.AbandonNext(prior_link);
            continue;
        }

        if (start_of_run == BidiType::NIL) {
            start_of_run = LevelAsNormalBidiType(std::ranges::max(base_level, bidi_chain.GetLevel(link)));
            first_link = link;
            prior_level = bidi_chain.GetLevel(link);
        } else if (prior_level != bidi_chain.GetLevel(link) || force_finish) {
            /* Since the level has changed at this link, therefore the run must end at prior link. */
            last_link = prior_link;

            /* Save the current level i.e. level of the next run. */
            BidiLevel current_level = bidi_chain.GetLevel(link);

            /*
             * Now we have both the prior level and the current level i.e. unchanged levels of both
             * the current run and the next run. So, identify end_of_run of the current run.
             * NOTE:
             *      start_of_run of the run has been already determined at this stage.
             */
            end_of_run = LevelAsNormalBidiType(std::ranges::max(prior_level, current_level));

            run_queue.Enqueue(LevelRun{bidi_chain, first_link, last_link, start_of_run, end_of_run});
            if (run_queue.should_dequeue || force_finish) {
        /* Rule X10 */
                for (; !run_queue.Empty(); run_queue.Dequeue()) {
                    LevelRun& peek = run_queue.Peek();
                    if (IsRunKindAttached(peek.kind)) { continue; }
                    isolating_run.Resolve(std::forward<T>(code_points), bidi_chain, peek, base_level);
                }
            }

            /* The start_of_run of next run (if any) should be technically equal to end_of_run of this run. */
            start_of_run = end_of_run;
            /* The next run (if any) will start from this index. */
            first_link = link;

            prior_level = current_level;
        }

        prior_link = link;
    }
}


BidiLink SkipIsolatingRun(const BidiChain& bidi_chain, BidiLink skip_link, BidiLink break_link)
{
    BidiLink link = skip_link;
    size_t depth = 1;

    while ((link = bidi_chain.GetNext(link)) != break_link) {
        BidiType type = bidi_chain.GetType(link);

        switch (type) {
        case BidiType::LRI:
        case BidiType::RLI:
        case BidiType::FSI:
            depth += 1;
            break;

        case BidiType::PDI:
            if (--depth == 0) {
                return link;
            }
            break;
        }
    }

    return BIDI_LINK_NONE;
}


}





#if 0
template <typename Range_t>
requires jcu::utf::IsUTF32CompatibleReduced_c<std::ranges::range_value_t<Range_t>>
std::vector<std::span<const char32_t>> BreakLines(Range_t&& code_points) {
    std::vector<std::span<const char32_t>> out{};
    auto it = code_points.begin();
    while (it != code_points.end()) {
        auto it_end = std::ranges::find_if(it, code_points.end(), [](){});


        // starting from range begin find the first U'\r'.  That is the new range end.  If it was followed by a U'\n'
        // then move end forward one more.
        auto boundary_end_it = std::ranges::find(iter_start, iter_end, U'\r', jcu::utf::IteratorData::code_point);
        if (boundary_end_it != iter_end) {
            ++boundary_end_it;
            /* Don't break in between 'CR' and 'LF'. */
            if (boundary_end_it != iter_end && boundary_end_it->code_point == U'\n') { ++boundary_end_it; }
        }
        iter_end = boundary_end_it;

        return std::span<const std::ranges::range_value_t<decltype(rng)>(iter_start, iter_end);



        out.push_back({it, it_end});
        it = it_end;
    }
    return out;
}

    auto MakeSubrange = [](jcu::utf::UTFCompatibleRange_c auto && rng, size_t start, size_t length) {
        /* TODO: Change start/length to some form of subrange */
        size_t end = start + length;
        if (end <= start) { return; } // check for 0 or overflow

        jcu::utf::ForwardView view{rng};
        // TODO: Figure out if subrange paragraph is used and how; revisit this after. for now assuming it is and
        // have to move the iterators to create bounds for the subrange.  May need to rethink based on usage.
        auto iter_start = view.begin();
        while (iter_start->index < start && iter_start != view.end()) { ++iter_start; }
        if (iter_start == view.end()) { return; }  // requested range out of bounds
        if (iter_start->index > start) { return; } // range not aligned with utf32 code points

        auto iter_end = iter_start;
        while (iter_end->index < end && iter_end != view.end()) { ++iter_end; }
        if (iter_end->index > end) { return; } // range not aligned with utf32 code points

        // starting from range begin find the first U'\r'.  That is the new range end.  If it was followed by a U'\n'
        // then move end forward one more.
        auto boundary_end_it = std::ranges::find(iter_start, iter_end, U'\r', jcu::utf::IteratorData::code_point);
        if (boundary_end_it != iter_end) {
            ++boundary_end_it;
            /* Don't break in between 'CR' and 'LF'. */
            if (boundary_end_it != iter_end && boundary_end_it->code_point == U'\n') { ++boundary_end_it; }
        }
        iter_end = boundary_end_it;

        return std::span<const std::ranges::range_value_t<decltype(rng)>(iter_start, iter_end);
    };
#endif
