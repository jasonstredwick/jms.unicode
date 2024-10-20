// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "jcu/ucd/base_iterator.hpp"
#include "jcu/ucd/utility.hpp"


namespace jcu::ucd {


struct UnicodeDataUnit {
    char32_t code_point{0};
    std::string character_name{};
    std::string general_category{};
    std::string combining_class{};
    std::string bidirectional_category{};
    std::string decomposition_mapping{};
    std::string decimal_digit_value{};
    std::string digit_value{};
    std::string numeric_value{};
    std::string mirrored{};
    std::string old_name{};
    std::string comment_field{};
    std::string uppercase_mapping{};
    std::string lowercase_mapping{};
    std::string titlecase_mapping{};
};


class UnicodeDataIterator : public FileIterator<UnicodeDataUnit, UnicodeDataIterator> {
protected:
    bool AcceptLine() const noexcept override { return !buffer.empty(); }

    UnicodeDataUnit ProcessLine() override {
        auto split_view = std::views::split(buffer, ';');
        if (std::ranges::count_if(split_view, [](auto) { return true; }) != 15) {
            throw std::runtime_error{"Unexpected line of data."};
        }

        auto it = split_view.begin();
        auto Extract = [](auto&& i) { std::string out{std::string_view{*i}}; ++i; return out; };
        return {
            .code_point=static_cast<char32_t>(std::stoul(Extract(it), nullptr, 16)),
            .character_name=Extract(it),
            .general_category=Extract(it),
            .combining_class=Extract(it),
            .bidirectional_category=Extract(it),
            .decomposition_mapping=Extract(it),
            .decimal_digit_value=Extract(it),
            .digit_value=Extract(it),
            .numeric_value=Extract(it),
            .mirrored=Extract(it),
            .old_name=Extract(it),
            .comment_field=Extract(it),
            .uppercase_mapping=Extract(it),
            .lowercase_mapping=Extract(it),
            .titlecase_mapping=Extract(it)
        };
    }

public:
    using FileIterator::FileIterator;

    UnicodeDataIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<UnicodeDataIterator>);


class UnicodeData {
    std::vector<UnicodeDataUnit> data{};

public:
    static constexpr const char* FILE_NAME = "UnicodeData.txt";
    static constexpr std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    UnicodeData(const std::filesystem::path& directory) {
        {
            std::ifstream fs{directory / FILE_NAME, OPEN_MODE};
            data.reserve(DataFileNumLines(fs));
        }
        UnicodeDataIterator it{directory / FILE_NAME, OPEN_MODE};
        UnicodeDataIterator end{true};
        std::ranges::transform(it, end, std::back_inserter(data), std::identity{});
    }

    auto begin() const { return data.cbegin(); }
    auto end() const { return data.cend(); }

    char32_t FirstCodePoint() const { return data.front().code_point; }
    char32_t LastCodePoint() const { return data.back().code_point; }
    auto&& operator[](this auto&& self, size_t index) { return self.data.at(index); }

    static char32_t LookupLastCodePoint(const std::filesystem::path& directory) {
        UnicodeDataIterator it{directory / FILE_NAME, OPEN_MODE};
        UnicodeDataIterator end{true};
        char32_t code_point = 0;
        for (; it != end; ++it) { code_point = it->code_point; }
        return code_point;
    }
};


}
