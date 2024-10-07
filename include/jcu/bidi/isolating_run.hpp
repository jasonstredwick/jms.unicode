// Copyright © 2014-2022 Muhammad Tayyab Akram

#pragma once


#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <deque>
#include <ranges>

#include "jcu/bidi/bidi_chain.hpp"
#include "jcu/bidi/bidi_type.hpp"
#include "jcu/bidi/bracket_queue.hpp"
#include "jcu/bidi/level_run.hpp"
#include "jcu/data/bracket.hpp"
#include "jcu/utf/concepts.hpp"


namespace jcu::bidi {


constexpr BidiType LevelAsNormalBidiType(BidiLevel level) noexcept { return (level & 1) ? BidiType::R : BidiType::L; }                              \
constexpr BidiType LevelAsOppositeBidiType(BidiLevel level) noexcept { return (level & 1) ? BidiType::L : BidiType::R; }                              \


class IsolatingRun {
    BracketQueue bracket_queue{};

public:
    template <typename T>
    requires jcu::utf::IsUTF32CompatibleReduced_c<std::ranges::range_value_t<T>>
    void Resolve(T&& code_points, BidiChain& bidi_chain, LevelRun& base_level_run, BidiLevel base_level) {
        // Save link for restoration at the end.
        BidiLink original_link = bidi_chain.GetNext(bidi_chain.Roller());

        /* Attach level run links to form isolating run. */
        /* Save last subsequent link. */
        LevelRun* last_run = AttachLevelRunLinks(bidi_chain, base_level_run, base_level);

        BidiType start_of_run = base_level_run.start_of_run;
        BidiType end_of_run = [&]() -> BidiType {
            if (!IsRunKindPartialIsolate(base_level_run.kind)) { return last_run->end_of_run; }
            BidiLevel eos_level = std::ranges::max(base_level_run.level, base_level);
            return (eos_level & 1) ? BidiType::R : BidiType::L;
        }();
        BidiLink subsequent_link = last_run->subsequent_link;

        /* Rules W1-W7 */
        BidiLink last_link = ResolveWeakTypes(bidi_chain, start_of_run);

        /* Rule N0 */
        ResolveBrackets(std::forward<T>(code_points), bidi_chain, base_level_run.level, start_of_run);

        /* Rules N1, N2 */
        ResolveNeutrals(bidi_chain, base_level_run.level, start_of_run, end_of_run);

        /* Rules I1, I2 */
        ResolveImplicitLevels(bidi_chain, base_level_run.level);

        /* Re-attach original links. */
        AttachOriginalLinks(bidi_chain, base_level_run, original_link);
        /* Attach new final link (of isolating run) with last subsequent link. */
        bidi_chain.SetNext(last_link, subsequent_link);

        // should bracket_queue be reset after every run?
    }

private:
    LevelRun* AttachLevelRunLinks(BidiChain& bidi_chain, LevelRun& base_level_run, BidiLevel base_level) {
        bidi_chain.SetNext(bidi_chain.Roller(), base_level_run.first_link);

        // Iterate over level runs and attach their links to form an isolating run.  Can be a chain longer than
        // one if run kind can be both an isolate and terminating.
        LevelRun* current = std::addressof(base_level_run);
        LevelRun* next = nullptr;
        for (; (next = current->next); current = next) {
            bidi_chain.SetNext(current->last_link, next->first_link);
        }
        bidi_chain.SetNext(current->last_link, bidi_chain.Roller());

        return current;
    }

    void AttachOriginalLinks(BidiChain& bidi_chain, LevelRun& base_level_run, BidiLink original_link) {
        bidi_chain.SetNext(bidi_chain.Roller(), original_link);

        // Iterate over level runs and attach original subsequent links.
        for (LevelRun* current = std::addressof(base_level_run); current; current = current->next) {
            bidi_chain.SetNext(current->last_link, current->subsequent_link);
        }
    }

    void ResolveAvailableBracketPairs(BidiChain& bidi_chain, BidiLevel run_level, BidiType start_of_run) {
        BidiType embeddingDirection = LevelAsNormalBidiType(run_level);
        BidiType oppositeDirection = LevelAsOppositeBidiType(run_level);

        while (!bracket_queue.Empty()) {
            BidiLink opening_link = bracket_queue.GetOpeningLink();
            BidiLink closing_link = bracket_queue.GetClosingLink();

            if (opening_link != BIDI_LINK_NONE && closing_link != BIDI_LINK_NONE) {
                BidiType pair_type = BidiType::NIL;
                BidiType innerStrongType = bracket_queue.GetStrongType();

                /* Rule: N0.b */
                if (innerStrongType == embeddingDirection) { pair_type = innerStrongType; }
                /* Rule: N0.c */
                else if (innerStrongType == oppositeDirection) {
                    BidiLink prior_strong_link = bracket_queue.GetPriorStrongLink();
                    BidiType priorStrongType = start_of_run;

                    if (prior_strong_link != BIDI_LINK_NONE) {
                        priorStrongType = bidi_chain.GetType(prior_strong_link);
                        if (IsBidiTypeNumber(priorStrongType)) { priorStrongType = BidiType::R; }

                        BidiLink link = bidi_chain.GetNext(prior_strong_link);
                        while (link != opening_link) {
                            BidiType type = bidi_chain.GetType(link);
                            if (type == BidiType::L || type == BidiType::R) { priorStrongType = type; }
                            link = bidi_chain.GetNext(link);
                        }
                    }

                    /* Rule: N0.c.1 */
                    if (priorStrongType == oppositeDirection) { pair_type = oppositeDirection; }
                    /* Rule: N0.c.2 */
                    else { pair_type = embeddingDirection; }
                }
                /* Rule: N0.d */
                else { pair_type = BidiType::NIL; }

                if (pair_type != BidiType::NIL) {
                    /* Do the substitution */
                    bidi_chain.SetType(opening_link, pair_type);
                    bidi_chain.SetType(closing_link, pair_type);
                }
            }

            bracket_queue.Dequeue();
        }
    }

    void ResolveBrackets(auto&& code_points, BidiChain& bidi_chain, BidiLevel run_level, BidiType start_of_run) {
        const BidiLink roller = bidi_chain.Roller();
        BidiLink prior_strong_link = BIDI_LINK_NONE;

        bracket_queue.Reset(LevelAsNormalBidiType(run_level));

        for (BidiLink link = bidi_chain.links[roller]; link != roller; link = bidi_chain.links[link]) {
            BidiType type = bidi_chain.GetType(link);

            bool is_done = false;

            switch (type) {
            case BidiType::ON:
            {
                char32_t code_point = code_points.at(static_cast<size_t>(link));
                auto [paired_code_point, bracket_type] = BracketData::Lookup(code_point);
                switch (bracket_type) {
                case BracketPairedType::OPEN:
                    if (bracket_queue.Full()) { is_done = true; }
                    else { bracket_queue.Enqueue(prior_strong_link, link, paired_code_point); }
                    break;

                case BracketPairedType::CLOSE:
                    if (!bracket_queue.Empty()) {
                        bracket_queue.ClosePair(link, code_point);
                        if (bracket_queue.ShouldDequeue()) {
                            ResolveAvailableBracketPairs(bidi_chain, run_level, start_of_run);
                        }
                    }
                    break;
                }
                break;
            }
            case BidiType::EN:
            case BidiType::AN:
                type = BidiType::R;

            case BidiType::R:
            case BidiType::L:
                if (!bracket_queue.Empty()) { bracket_queue.SetStrongType(type); }
                prior_strong_link = link;
                break;
            }

            if (is_done) { break; }
        }

        ResolveAvailableBracketPairs(bidi_chain, run_level, start_of_run);
    }

    void ResolveImplicitLevels(BidiChain& bidi_chain, BidiLevel run_level) {
        const BidiLink roller = bidi_chain.Roller();

        if ((run_level & 1) == 0) {
            for (BidiLink link = bidi_chain.links[roller]; link != roller; link = bidi_chain.links[link]) {
                BidiType type = bidi_chain.GetType(link);
                assert(IsBidiTypeStrongOrNumber(type));
                BidiLevel level = bidi_chain.GetLevel(link);

                /* Rule I1 */
                if (type == BidiType::R)      { bidi_chain.SetLevel(link, level + 1); }
                else if (type != BidiType::L) { bidi_chain.SetLevel(link, level + 2); }
            }
        } else {
            for (BidiLink link = bidi_chain.links[roller]; link != roller; link = bidi_chain.links[link]) {
                BidiType type = bidi_chain.GetType(link);
                assert(IsBidiTypeStrongOrNumber(type));
                BidiLevel level = bidi_chain.GetLevel(link);
                
                /* Rule I2 */
                if (type != BidiType::R) { bidi_chain.SetLevel(link, level + 1); }
            }
        }
    }

    void ResolveNeutrals(BidiChain& bidi_chain, BidiLevel run_level, BidiType start_of_run, BidiType end_of_run) {
        const BidiLink roller = bidi_chain.Roller();
        BidiType strong_type = start_of_run;
        BidiLink neutralLink = BIDI_LINK_NONE;

        for (BidiLink link = bidi_chain.links[roller]; link != roller; link = bidi_chain.links[link]) {
            BidiType type = bidi_chain.GetType(link);
            assert(IsBidiTypeStrongOrNumber(type) || IsBidiTypeNeutralOrIsolate(type));

            switch (type) {
            case BidiType::L:
                strong_type = BidiType::L;
                break;

            case BidiType::R:
            case BidiType::EN:
            case BidiType::AN:
                strong_type = BidiType::R;
                break;

            case BidiType::B:                           
            case BidiType::S:
            case BidiType::WS:
            case BidiType::ON:
            case BidiType::LRI:
            case BidiType::RLI:                         
            case BidiType::FSI:
            case BidiType::PDI:
                if (neutralLink == BIDI_LINK_NONE) { neutralLink = link; }

                BidiType next_type = bidi_chain.GetType(bidi_chain.GetNext(link));
                if (IsBidiTypeNumber(next_type))     { next_type = BidiType::R; }
                else if (next_type == BidiType::NIL) { next_type = end_of_run; }

                if (IsBidiTypeStrong(next_type)) {
                    /* Rules N1, N2 */
                    BidiType resolvedType = strong_type == next_type ? strong_type : LevelAsNormalBidiType(run_level);

                    do {
                        bidi_chain.SetType(neutralLink, resolvedType);
                        neutralLink = bidi_chain.GetNext(neutralLink);
                    } while (neutralLink != bidi_chain.GetNext(link));

                    neutralLink = BIDI_LINK_NONE;
                }
                break;
            }
        }
    }

    BidiLink ResolveWeakTypes(BidiChain& bidi_chain, BidiType start_of_run) {
        const BidiLink roller = bidi_chain.Roller();
        BidiLink prior_link = roller;
        BidiType w1PriorType = start_of_run;
        BidiType w2StrongType = start_of_run;

        for (BidiLink link = bidi_chain.links[roller]; link != roller; link = bidi_chain.links[link]) {
            BidiType type = bidi_chain.GetType(link);
            bool force_merge = false;

            /* Rule W1 */
            if (type == BidiType::NSM) {
                /* Change the 'type' variable as well because it can be EN on which W2 depends. */
                type = IsBidiTypeIsolate(w1PriorType) ? BidiType::ON : w1PriorType;
                bidi_chain.SetType(link, type);

                /* Fix for 3rd point of rule N0. */
                if (w1PriorType == BidiType::ON) {
                    force_merge = true;
                }
            }
            w1PriorType = type;

            /* Rule W2 */
            if (type == BidiType::EN) {
                if (w2StrongType == BidiType::AL) {
                    bidi_chain.SetType(link, BidiType::AN);
                }
            }

            /*
            * Rule W3
            * NOTE: It is safe to apply W3 in 'else-if' statement because it only depends on type AL.
            *       Even if W2 changes EN to AN, there won't be any harm.
            */
            else if (type == BidiType::AL) {
                bidi_chain.SetType(link, BidiType::R);
            }

            if (IsBidiTypeStrong(type)) {
                /* Save the strong type as it is checked in W2. */
                w2StrongType = type;
            }

            if ((type != BidiType::ON && bidi_chain.GetType(prior_link) == type) || force_merge) {
                bidi_chain.AbandonNext(prior_link);
            } else {
                prior_link = link;
            }
        }

        prior_link = roller;
        BidiType w4PriorType = start_of_run;
        BidiType w5PriorType = start_of_run;
        BidiType w7StrongType = start_of_run;

        for (BidiLink link = bidi_chain.links[roller]; link != roller; link = bidi_chain.links[link]) {
            BidiType type = bidi_chain.GetType(link);
            BidiType next_type = bidi_chain.GetType(bidi_chain.GetNext(link));

            /* Rule W4 */
            if (bidi_chain.IsSingle(link) &&
                IsBidiTypeNumberSeparator(type) &&
                IsBidiTypeNumber(w4PriorType) &&
                (w4PriorType == next_type) &&
                (w4PriorType == BidiType::EN || type == BidiType::CS))
            {
                /* Change the current type as well because it can be EN on which W5 depends. */
                type = w4PriorType;
                bidi_chain.SetType(link, type);
            }
            w4PriorType = type;

            /* Rule W5 */
            if (type == BidiType::ET && (w5PriorType == BidiType::EN || next_type == BidiType::EN)) {
                /* Change the current type as well because it is EN on which W7 depends. */
                type = BidiType::EN;
                bidi_chain.SetType(link, type);
            }
            w5PriorType = type;

            switch (type) {
            /* Rule W6 */
            case BidiType::ET:
            case BidiType::CS:
            case BidiType::ES:
                bidi_chain.SetType(link, BidiType::ON);
                break;

            /*
            * Rule W7
            * NOTE: W7 is expected to be applied after W6. However this is not the case here. The
            *       reason is that W6 can only create the type ON which is not tested in W7 by any
            *       means. So it won't affect the algorithm.
            */
            case BidiType::EN:
                if (w7StrongType == BidiType::L) {
                    bidi_chain.SetType(link, BidiType::L);
                }
                break;

            /*
            * Save the strong type for W7.
            * NOTE: The strong type is expected to be saved after applying W7 because W7 itself creates
            *       a strong type. However the strong type being saved here is based on the type after
            *       W5. This won't effect the algorithm because a single link contains all consecutive
            *       EN types. This means that even if W7 creates a strong type, it will be saved in
            *       next iteration.
            */
            case BidiType::L:
            case BidiType::R:
                w7StrongType = type;
                break;
            }

            if (type != BidiType::ON && bidi_chain.GetType(prior_link) == type) {
                bidi_chain.AbandonNext(prior_link);
            } else {
                prior_link = link;
            }
        }

        return prior_link;
    }
};


}
