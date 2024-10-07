// Copyright © 2014-2022 Muhammad Tayyab Akram

#pragma once


#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <deque>
#include <iterator>

#include "jcu/bidi/level_run.hpp"


namespace jcu::bidi {


struct RunQueue {
    std::deque<LevelRun> level_run_deque{};
    ptrdiff_t partial_index{-1};
    bool should_dequeue{false};

    void Dequeue() {
        assert(!level_run_deque.empty());
        level_run_deque.pop_front();
        partial_index = std::ranges::max(static_cast<ptrdiff_t>(-1), partial_index - 1);
    }

    bool Empty() const noexcept { return level_run_deque.empty(); }

    template <typename T>
    requires std::same_as<LevelRun, std::remove_cvref_t<T>>
    void Enqueue(T&& _level_run) {
        if constexpr (std::movable<T>) {
            level_run_deque.push_back(std::move(_level_run));
        } else {
            level_run_deque.push_back(_level_run);
        }

        // Can it be terminating and an isolate???
        auto& level_run = level_run_deque.back();

        // Complete the latest isolating run with this terminating run.
        if (partial_index >= 0 && IsRunKindTerminating(level_run.kind)) {
            auto it_partial = level_run_deque.begin();
            std::ranges::advance(it_partial, partial_index, level_run_deque.end());
            it_partial->Attach(level_run);

            // find previous partial isolate run
            auto rev_it = std::ranges::find_if(std::make_reverse_iterator(it_partial),
                                               level_run_deque.rend(),
                                               IsRunKindPartialIsolate, &LevelRun::kind);
            if (rev_it == level_run_deque.rend()) {
                partial_index = -1;
                should_dequeue = false;
            } else {
                partial_index = std::ranges::distance(level_run_deque.begin(), rev_it.base());
            }
        }

        // Save the location of the isolating run.
        if (IsRunKindIsolate(level_run.kind)) {
            partial_index = std::ssize(level_run_deque) - 1;
        }
    }

    LevelRun& Peek() {
        assert(!level_run_deque.empty());
        return level_run_deque.front();
    }
};


}
