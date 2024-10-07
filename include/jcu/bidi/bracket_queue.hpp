// Copyright © 2014-2022 Muhammad Tayyab Akram

#pragma once


#include <algorithm>
#include <cassert>
#include <deque>
#include <ranges>

#include "jcu/bidi/bidi_chain.hpp"
#include "jcu/bidi/bidi_type.hpp"


namespace jcu::bidi {


class BracketQueue {
    static constexpr size_t MAX_CAPACITY = 63;

    struct BracketQueueElement {
        char32_t bracket;
        BidiLink closing_link;
        BidiLink opening_link;
        BidiLink prior_strong_link;
        BidiType strong_type;
    };

    std::deque<BracketQueueElement> elements{};
    bool should_dequeue{false};
    BidiType direction{BidiType::NIL};

public:
    bool Empty() const noexcept { return elements.empty(); }
    bool Full() const noexcept { return elements.size() >= BracketQueue::MAX_CAPACITY; }
    BidiLink GetClosingLink() const { return elements.front().closing_link; }
    BidiLink GetOpeningLink() const { return elements.front().opening_link; }
    BidiLink GetPriorStrongLink() const { return elements.front().prior_strong_link; }
    BidiType GetStrongType() const { return elements.front().strong_type; }
    void Reset(BidiType new_direction) { elements.clear(); should_dequeue = false; direction = new_direction; }
    bool ShouldDequeue() const noexcept { return should_dequeue; }
    size_t Size() const noexcept { return elements.size(); }

    void ClosePair(BidiLink closing_link, char32_t bracket_ch) {
        char32_t canonical = bracket_ch;
        switch (bracket_ch) {
        case 0x232A:
            canonical = 0x3009;
            break;

        case 0x3009:
            canonical = 0x232A;
            break;

        default:
            break;
        }

        auto v = elements | std::views::reverse;
        auto it = std::ranges::find_if(v, [&bracket_ch, &canonical](auto& e) {
            return e.opening_link != BIDI_LINK_NONE &&
                   e.closing_link == BIDI_LINK_NONE &&
                   (e.bracket == bracket_ch || e.bracket == canonical);
        });

        if (it != v.end()) {
            it->closing_link = closing_link;

            // Conditionally mark all subsequent (i.e. towards the top) elements opening_link to none.
            for (auto& e : std::ranges::subrange(v.begin(), it)) {
                if (e.opening_link != BIDI_LINK_NONE && e.closing_link == BIDI_LINK_NONE) {
                    e.opening_link = BIDI_LINK_NONE;
                }
            }

            // check if found element is the front of the queue.
            if (it == std::prev(v.end())) { should_dequeue = true; }
        }
    }

    void Dequeue() {
        assert(!elements.empty());
        elements.pop_front();
    }

    void Enqueue(BidiLink prior_strong_link, BidiLink opening_link, char32_t bracket_ch) {
        assert(!Full());
        elements.push_back({
            .bracket=bracket_ch,
            .closing_link=BIDI_LINK_NONE,
            .opening_link=opening_link,
            .prior_strong_link=prior_strong_link,
            .strong_type=BidiType::NIL
        });
    }

    void SetStrongType(BidiType strong_type) {
        for (auto& e : elements) {
            if (e.closing_link == BIDI_LINK_NONE && e.strong_type != direction) {
                e.strong_type = strong_type;
            }
        }
    }
};


}
