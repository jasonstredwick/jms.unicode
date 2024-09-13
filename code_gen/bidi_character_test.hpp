// Copyright © 2024 Jason Stredwick

#pragma once


#include <filesystem>
#include <format>
#include <fstream>
#include <ranges>
#include <string_view>
#include <utility>

#include "jcu/ucd/bidi_character_test.hpp"
#include "jcu/utf/format.hpp"


namespace jcu::code_gen {


std::filesystem::path Path(const jcu::ucd::BidiCharacterTest&, const std::filesystem::path& root) {
    return root / "bidi" / "bidi_character.test.cpp";
}


void WriteHeader(std::ofstream& out, const jcu::ucd::BidiCharacterTest& data) {
    auto DirToString = [](jcu::ucd::ParagraphDirection dir) -> std::string_view {
        if      (dir == jcu::ucd::ParagraphDirection::AUTO) { return std::string_view{"AUTO"}; }
        else if (dir == jcu::ucd::ParagraphDirection::LTR) { return "LTR"; }
        else if (dir == jcu::ucd::ParagraphDirection::RTL) { return "RTL"; }
        return "INVALID";
    };

    auto FormatDec = [](auto i) { return std::format("{}, ", i); };
    auto FormatHex = [](auto i) { return std::format("0x{:04X}, ", i); };
    auto VecToString = [](const auto& v, auto fmt) {
        std::string str{};
        for (auto i : v) { str += fmt(i); }
        return str.substr(0, str.size() - 2);
    };
    auto VecToU32String = [](const auto& v) {
        std::string str{"U\""};
        for (auto i : v) { str += std::format("\\x{:04X}", i); }
        str += "\"";
        return str;
    };
    auto MergeDescription = [](const std::vector<std::string>& desc, std::string_view spacing) {
        std::string out{};
        for (const auto& i : desc) { out += std::format("{}{}\n", spacing, i); }
        return out;
    };

    out <<
R"(/*
 * Automatically generated by code_gen/bidi_character_test.hpp
 * DO NOT EDIT!!
 */

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include "ftest.h"


constexpr uint8_t LEVEL_REMOVED = std::numeric_limits<uint8_t>::max();


enum class ParagraphDirection : uint8_t {
    LTR=0,
    RTL=1,
    AUTO=2,
    INVALID=3
};


bool RunTest(std::u32string_view text,
             std::span<uint8_t> levels,
             std::span<size_t> order,
             ParagraphDirection paragraph_direction,
             uint8_t paragraph_level)
{
    return true;
}
)";


    int group_count = 0;
    const auto& group_comment_map = data.GroupComments();
    const auto& section_comment_map = data.SectionComments();
    for (auto group_it = group_comment_map.begin(); group_it != group_comment_map.end(); ++group_it) {
        auto [group_index, group_comment] = *group_it;

        out << std::format("\n\n");

        if (auto section_it = section_comment_map.find(group_index); section_it != section_comment_map.end()){
            out << std::format("{}\n", MergeDescription(section_it->second, ""));
        }

        auto next_it = std::ranges::next(group_it, 1, group_comment_map.end());
        size_t next_it_index = next_it == group_comment_map.end() ? data.Size() : next_it->first;
        std::ranges::subrange all_test_data(data.begin() + group_index, data.begin() + next_it_index);

        out << std::format("TEST(BidiCharacterTests, test_{}) {{\n", std::exchange(group_count, group_count + 1));
        out << MergeDescription(group_comment, "    ");
        for (auto [test_index, test_data] : all_test_data | std::views::enumerate) {
            out << std::format("    std::array<uint8_t, {}> levels_{}{{{}}};\n", test_data.levels.size(), test_index, VecToString(test_data.levels, FormatDec));
            out << std::format("    std::array<size_t, {}> order_{}{{{}}};\n", test_data.order.size(), test_index, VecToString(test_data.order, FormatDec));
            out << std::format("    EXPECT_TRUE(RunTest({}, levels_{}, order_{}, ParagraphDirection::{}, {}));\n",
                //VecToString(test_data.text, FormatHex),
                VecToU32String(test_data.text),
                test_index,
                test_index,
                DirToString(test_data.paragraph_direction),
                test_data.paragraph_level);
        }
        out << std::format("}}\n");
    }
}


}
