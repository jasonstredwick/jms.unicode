// Copyright © 2024 Jason Stredwick

#include <filesystem>
#include <format>
#include <iostream>
#include <print>
#include <random>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "ftest.h"

#include "jcu/bidi/algorithm.hpp"
#include "jcu/bidi/runs.hpp"
#include "jcu/data/bidi_type.hpp"
#include "jcu/ucd/bidi_test.hpp"


struct DerivedBidiClassView {
    auto begin() const { return jcu::data::DerivedBidiClass::begin(); }
    auto end()   const { return jcu::data::DerivedBidiClass::end();   }
};


struct MapData {
    char32_t code_point;
    size_t index;
};


TEST(BidiTests, test_BidiTest) {
    int tests_run = 0;
    try {
        std::unordered_map<jcu::bidi::BidiType, std::vector<MapData>> bidi_type_map{};
        std::unordered_map<jcu::bidi::BidiType, size_t> indices{};
        for (auto [data, next] : DerivedBidiClassView{} | std::views::pairwise) {
            auto map_it = bidi_type_map.find(data.value);

            if (map_it == bidi_type_map.end()) {
                indices[data.value] = 0;
                bidi_type_map[data.value] = {};
                map_it = bidi_type_map.find(data.value);
            }

            auto index_it = indices.find(data.value);
            size_t count = static_cast<size_t>(next.code_point) - static_cast<size_t>(data.code_point);
            size_t old_index = index_it->second;
            map_it->second.push_back({
                .code_point=data.code_point,
                .index=std::exchange(index_it->second, index_it->second + count)
            });
        }

        // Add final index past the end for use with std::ranges::upper_bound
        for (auto& [k, v] : bidi_type_map) { v.push_back({.code_point=jcu::CODE_POINT_INVALID, .index=indices[k]}); }

        std::random_device rd{};
        std::ranlux24_base gen{rd()};
        std::unordered_map<jcu::bidi::BidiType, std::uniform_int_distribution<>> bidi_type_dists{};
        for (auto [k, v] : bidi_type_map) {
            // closed interval
            bidi_type_dists[k] = std::uniform_int_distribution<>{0, static_cast<int>(v.back().index - 1)};
        }

        std::vector<char32_t> buffer{0, 0, 0, 0}; // default to four values (may be more in the future)
        jcu::ucd::BidiTestForwardView btfv{std::filesystem::path{".."} / ".." / "data"};
        auto it = btfv.begin();
        auto end = btfv.end();
        for (int failures = 0; it != end && failures < 10; ++it) {
            auto [levels, reorders, values, direction_bitset, line_num] = *it;
            ++tests_run;

            if (levels.size() != values.size()) {
                status = ftest::Failed;
                ++failures;
                std::println("Line {} : Number of code points inconsistent.", line_num);
                continue;
            }

            try {
                if (levels.size() > buffer.size()) { buffer.resize(levels.size()); }

                std::span text{buffer.begin(), levels.size()};
                std::ranges::transform(values, text.begin(), [&bidi_type_dists, &bidi_type_map, &gen](auto btv) {
                    auto dist = bidi_type_dists[btv];
                    size_t index = static_cast<size_t>(dist(gen));
                    auto index_block_it = std::ranges::prev(
                        std::ranges::upper_bound(bidi_type_map[btv], index, {}, &MapData::index));
                    char32_t delta = static_cast<char32_t>(index - index_block_it->index);
                    char32_t code_point = index_block_it->code_point + delta;
                    return code_point;
                });

                std::vector<jcu::bidi::BidiLevel> base_levels{};
                if (direction_bitset & 1) { base_levels.push_back(jcu::bidi::LEVEL_TYPE_DEFAULT_AUTO); }
                if (direction_bitset & 2) { base_levels.push_back(jcu::bidi::LEVEL_TYPE_LTR); }
                if (direction_bitset & 4) { base_levels.push_back(jcu::bidi::LEVEL_TYPE_RTL); }
                if (!(direction_bitset & 2) && (!direction_bitset || direction_bitset & ~7)) {
                    base_levels.push_back(jcu::bidi::LEVEL_TYPE_LTR);
                }

                for (auto base_level : base_levels) {
                    std::vector<jcu::bidi::Run> runs = jcu::bidi::ToRuns(text, base_level);

                    std::vector<uint8_t> final_levels(text.size(), jcu::bidi::LEVEL_TYPE_INVALID);
                    for (auto& run : runs) {
                        std::ranges::fill(std::ranges::next(final_levels.begin(), run.offset,              final_levels.end()),
                                          std::ranges::next(final_levels.begin(), run.offset + run.length, final_levels.end()),
                                          run.level);
                    }

                    bool result = std::ranges::equal(final_levels, levels, [](auto lhs, auto rhs) {
                        return rhs == 255 || lhs == rhs;
                    });
                    if (!result) {
                        status = ftest::Failed;
                        ++failures;
                        for (auto i : final_levels) { std::print("{} ", i); } std::println("");
                        for (auto i : levels) { std::print("{} ", i); } std::println("");
                        std::println("Line {} : Failed", line_num);
                    }
                }
            } catch (const std::exception& e) {
                status = ftest::Failed;
                ++failures;
                std::println("Line {} : Failed by exception.", line_num);
                std::println("\n\nException caught:\n{}\n\n", e.what());
            }
        }
    } catch (const std::exception& e) {
        status = ftest::Failed;
        std::println("\n\nGeneral exception caught:\n{}\n\n", e.what());
    }

    std::println("Total tests performed: {}", tests_run);
}
