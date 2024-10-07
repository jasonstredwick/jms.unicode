// Copyright © 2014-2022 Muhammad Tayyab Akram

#pragma once


#include <algorithm>
#include <ranges>
#include <span>
#include <vector>

#include "jcu/bidi/bidi_type.hpp"
#include "jcu/bidi/level.hpp"


namespace jcu::bidi {


struct Run;
std::vector<Run> ProcessLevels(const std::vector<BidiLevel>&);
void ReorderRuns(std::vector<Run>&, BidiLevel) ;
template <typename R1, typename R2>
requires (std::ranges::range<R1> &&
          std::ranges::range<R2> &&
          std::same_as<BidiType, std::ranges::range_value_t<R1>> &&
          std::same_as<BidiLevel, std::ranges::range_value_t<R2>>)
std::vector<BidiLevel> ResetLevels(R1&&, R2&&, BidiLevel);


struct Run {
    size_t offset{0};   //< The index to the first code unit of the run in source string.
    size_t length{0};   //< The number of code units covering the length of the run.
    BidiLevel level{0}; //< The embedding level of the run.
};


template <typename R1, typename R2>
requires (std::ranges::range<R1> &&
          std::ranges::range<R2> &&
          std::same_as<BidiType, std::ranges::range_value_t<R1>> &&
          std::same_as<BidiLevel, std::ranges::range_value_t<R2>>)
std::vector<Run> CreateRuns(R1&& bidi_types, R2&& src_levels, BidiLevel base_level) {
    std::vector<BidiLevel> reset_levels = ResetLevels(bidi_types, src_levels, base_level);
    std::vector<Run> runs = ProcessLevels(reset_levels);
    BidiLevel max_level = std::ranges::max(src_levels); // why original and not reset version?
    ReorderRuns(runs, max_level);
    return runs;
}


std::vector<Run> ProcessLevels(const std::vector<BidiLevel>& levels) {
    if (levels.empty()) { return {}; }

    // Alternative: Could allocate N, N/2, N/4 or some minimum; could be combined with shrink_to_fit
    std::vector<Run> runs{};
    auto size_est = 1 + std::ranges::count_if(levels | std::views::pairwise, [](auto t) {
        auto [a, b] = t;
        return a != b;
    });
    runs.reserve(size_est);

    runs.push_back({.offset=0, .length=1, .level=levels[0]});
    for (auto const [index, level] : std::views::enumerate(levels) | std::views::drop(1)) {
        Run& run = runs.back();
        if (run.level != level) {
            runs.push_back({.offset=static_cast<size_t>(index), .length=1, .level=level});
        } else {
            run.length++;
        }
    }

    return runs;
}


void ReorderRuns(std::vector<Run>& runs, BidiLevel max_level) {
    for (BidiLevel newLevel = max_level; newLevel; newLevel--) {
        size_t start = runs.size();
        while (start--) {
            if (runs[start].level >= newLevel) {
                size_t count = 1;
                for (; start && runs[start - 1].level >= newLevel; start--) {
                    count += 1;
                }
                std::ranges::reverse(std::span{runs.begin() + start, count});
            }
        }
    }
}


template <typename R1, typename R2>
requires (std::ranges::range<R1> &&
          std::ranges::range<R2> &&
          std::same_as<BidiType, std::ranges::range_value_t<R1>> &&
          std::same_as<BidiLevel, std::ranges::range_value_t<R2>>)
std::vector<BidiLevel> ResetLevels(R1&& bidi_types, R2&& src_levels, BidiLevel base_level) {
    std::vector<BidiLevel> levels{src_levels.begin(), src_levels.end()};

    size_t length = 0;
    bool reset = true;

    // Appears to reset B and S
    //            reset any embedding at the beginning of a run
    //            reset any embedding following an isolate at the end of a run + that isolate
    size_t index = levels.size();
    while (index--) {
        BidiType type = bidi_types[index];

        switch (type) {
        case BidiType::B:
        case BidiType::S:
            std::ranges::fill(std::span{levels.begin() + index, length + 1}, base_level);
            length = 0;
            reset = true;
            break;

        case BidiType::LRE:
        case BidiType::RLE:
        case BidiType::LRO:
        case BidiType::RLO:
        case BidiType::PDF:
        case BidiType::BN:
            length += 1;
            break;

        case BidiType::WS:
        case BidiType::LRI:
        case BidiType::RLI:
        case BidiType::FSI:
        case BidiType::PDI:
            if (reset) {
                std::ranges::fill(std::span{levels.begin() + index, length + 1}, base_level);
                length = 0;
            }
            break;

        default:
            length = 0;
            reset = false;
            break;
        }
    }

    return levels;
}


}
