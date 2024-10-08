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
    try {
        auto path = std::filesystem::path{".."} / ".." / "data" / jcu::ucd::BidiCharacterTest::FILE_NAME;
        if (!std::filesystem::exists(path)) {
            std::println("Could not find test data: {}", path.generic_string());
            status = ftest::Failed;
            return;
        }

        int failures = 0;
        jcu::ucd::BidiCharacterTestFileIterator it{path, jcu::ucd::BidiCharacterTest::OPEN_MODE};
        jcu::ucd::BidiCharacterTestFileIterator end{true};
        for (; it != end; ++it) {
            auto [text, levels, order, paragraph_direction, paragraph_level, line_num] = *it;

            auto base_level = jcu::bidi::LEVEL_TYPE_DEFAULT_AUTO;
            if (paragraph_direction == jcu::ucd::ParagraphDirection::LTR) {
                base_level = jcu::bidi::LEVEL_TYPE_LTR;
            } else if (paragraph_direction == jcu::ucd::ParagraphDirection::RTL) {
                base_level = jcu::bidi::LEVEL_TYPE_RTL;
            }

            std::vector<jcu::bidi::Run> runs = jcu::bidi::ToRuns(text, base_level);

            std::vector<uint8_t> final_levels(text.size(), jcu::bidi::LEVEL_TYPE_INVALID);
            for (auto& run : runs) {
                std::ranges::fill(std::ranges::next(final_levels.begin(), run.offset, final_levels.end()),
                                  std::ranges::next(final_levels.begin(), run.offset + run.length, final_levels.end()),
                                  run.level);
            }

            bool result = std::ranges::equal(final_levels, levels, [](auto lhs, auto rhs) {
                return rhs == 255 || lhs == rhs;
            });
            if (!result) {
                for (auto i : final_levels) { std::print("{} ", i); } std::println("");
                for (auto i : levels) { std::print("{} ", i); } std::println("");
                failures++;
                std::println("Line {} : Failed", line_num);
                status = ftest::Failed;
            }

            if (failures >= 10) { break; }
        }
    } catch (const std::exception& e) {
        std::println("\n\nException caught:\n{}\n\n", e.what());
    }
}
