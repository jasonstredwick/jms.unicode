// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <ctre.hpp>

#include "jcu/constants.hpp"
#include "jcu/general_category.hpp"
#include "jcu/general_category_strings.hpp"
#include "jcu/ucd/base_iterator.hpp"
#include "jcu/unicode_version.hpp"


namespace jcu::ucd {


struct DerivedGeneralCategoryUnit {
    char32_t code_point_first{0};
    char32_t code_point_last{0};
    std::string general_category{};
    bool operator==(const DerivedGeneralCategoryUnit&) const = default;
};


class DerivedGeneralCategoryFileIterator : public FileIterator<DerivedGeneralCategoryUnit,
                                                               DerivedGeneralCategoryFileIterator> {
protected:
    static constexpr auto REGEX = ctre::match<"^\\s*([[:xdigit:]]+)(?:..([[:xdigit:]]+))?\\s*;\\s*([a-zA-Z]+).*$">;

    bool AcceptLine() const override { return !buffer.empty() && buffer[0] != '#'; }

    DerivedGeneralCategoryUnit ProcessLine() override {
        if (auto [whole, match1, match2, match3] = REGEX(buffer); whole) {
            char32_t first = static_cast<char32_t>(std::stoul(match1.to_string(), nullptr, 16));
            char32_t last = !match2 ? first : static_cast<char32_t>(std::stoul(match2.to_string(), nullptr, 16));
            return {
                .code_point_first=first,
                .code_point_last=last,
                .general_category=match3.to_string()
            };
        }
        throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
    }

public:
    using FileIterator::FileIterator;

    DerivedGeneralCategoryFileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<DerivedGeneralCategoryFileIterator>);


class DerivedGeneralCategory {
public:
    struct Data {
        char32_t code_point{0};
        GeneralCategory value{GeneralCategory::NIL};
    };

private:
    UnicodeVersion version{};
    std::vector<Data> data{};

public:
    static constexpr const char* FILE_NAME = "DerivedGeneralCategory.txt";
    static constexpr std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    DerivedGeneralCategory(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)}
    {
        // Create a vector of GeneralCategory for all possible code points.  Throw exception (.at) for unexpected
        // code points.
        // Assumptions:
        //     1. No gaps; i.e. default value is NIL
        // TODO: Add validation steps.
        DerivedGeneralCategoryFileIterator it{directory / FILE_NAME, OPEN_MODE};
        DerivedGeneralCategoryFileIterator end{true};

        std::vector<GeneralCategory> all_general_categories(jcu::CODE_POINT_MAX + 1, GeneralCategory::NIL);
        std::ranges::for_each(it, end, [&all_general_categories](auto&& unit) mutable {
            GeneralCategory general_category = GeneralCategoryFromString(unit.general_category);
            size_t start = static_cast<size_t>(unit.code_point_first);
            size_t last  = static_cast<size_t>(unit.code_point_last);
            for (auto i : std::views::iota(start, last + 1)) { all_general_categories.at(i) = general_category; }
        });

        // Condense bidi type runs
        data.reserve(1 + std::ranges::count_if(all_general_categories | std::views::pairwise, [](auto&& pair) {
            auto [a, b] = pair; return a != b;
        }));
        data.push_back({.code_point=0, .value=all_general_categories[0]});
        for (auto [index, pair] : std::views::enumerate(all_general_categories | std::views::pairwise)) {
            auto [a, b] = pair;
            if (a != b) { data.push_back({.code_point=static_cast<char32_t>(index + 1), .value=b}); }
        }
        data.push_back({.code_point=(jcu::CODE_POINT_MAX + 1), .value=GeneralCategory::NIL});
    }

    auto begin() const noexcept { return data.cbegin(); }
    auto end() const noexcept { return data.cend(); }

    GeneralCategory ToGeneralCategory(char32_t code_point) const {
        if (data.empty()) { return GeneralCategory::NIL; }
        auto it = std::ranges::upper_bound(data, code_point, {}, &Data::code_point);
        return std::ranges::prev(it)->value;
    }

    const UnicodeVersion &Version() const { return version; }
};


}
