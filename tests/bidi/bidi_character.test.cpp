// Copyright © 2024 Jason Stredwick

#include <filesystem>
#include <format>
#include <iostream>
#include <print>
#include <string>
#include <string_view>
#include <vector>

#include "ftest.h"

#include "jcu/bidi/algorithm.hpp"
#include "jcu/bidi/runs.hpp"
#include "jcu/ucd/bidi_character_test.hpp"


TEST(BidiTests, test_BidiCharacterTest) {
    // Create code point sequence for a sample bidirectional text.
    int tests_run = 0;
    try {
        jcu::ucd::BidiCharacterTestForwardView rng{std::filesystem::path{".."} / ".." / "data"};

        auto it = rng.begin();
        auto end = rng.end();
        for (int failures = 0; it != end && failures < 10; ++it) {
            auto [text, levels, order, paragraph_direction, paragraph_level, line_num] = *it;
            ++tests_run;

            try {
                auto base_level = jcu::bidi::LEVEL_TYPE_DEFAULT_AUTO;
                if (paragraph_direction == jcu::ucd::ParagraphDirection::LTR) {
                    base_level = jcu::bidi::LEVEL_TYPE_LTR;
                } else if (paragraph_direction == jcu::ucd::ParagraphDirection::RTL) {
                    base_level = jcu::bidi::LEVEL_TYPE_RTL;
                }

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
