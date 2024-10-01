// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <filesystem>
#include <format>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <ctre.hpp>

#include "jcu/constants.hpp"
#include "jcu/bidi/bidi_type.hpp"
#include "jcu/strings/bidi_type.hpp"
#include "jcu/ucd/base_iterator.hpp"
#include "jcu/unicode_version.hpp"


namespace jcu::ucd {


struct DerivedBidiClassUnit {
    char32_t code_point_first{0};
    char32_t code_point_last{0};
    std::string bidi_type{};
    bool operator==(const DerivedBidiClassUnit&) const = default;
};


class DerivedBidiClassFileIterator : public FileIterator<DerivedBidiClassUnit, DerivedBidiClassFileIterator> {
protected:
    static constexpr auto REGEX = ctre::match<"^\\s*([[:xdigit:]]+)(?:..([[:xdigit:]]+))?\\s*;\\s*([_a-zA-Z]+).*$">;
    static constexpr std::string_view MISSING{"# @missing:"};
    static constexpr bool IsMissing(std::string_view sv) {
        // convert to starts_with once available on all compilers.
        return std::string_view{sv | std::views::take(MISSING.size())} == MISSING;
    }

    bool AcceptLine() const override {
        if (buffer.empty()) { return false; }
        if (buffer[0] == '#') { return IsMissing(buffer); }
        return true;
    }

    DerivedBidiClassUnit ProcessLine() override { return buffer[0] == '#' ? ProcessLineMissing() : ProcessLineData(); }

    DerivedBidiClassUnit ProcessLineData() const { return ProcessUnit(buffer); }

    DerivedBidiClassUnit ProcessLineMissing() const {
        DerivedBidiClassUnit unit = ProcessUnit(std::string_view{buffer} | std::views::drop(MISSING.size()));
        if (unit.bidi_type == std::string_view{"Left_To_Right"}) {
            unit.bidi_type = std::string{"missing:L"};
        } else if (unit.bidi_type == std::string_view{"Right_To_Left"}) {
            unit.bidi_type = std::string{"missing:R"};
        } else if (unit.bidi_type == std::string_view{"Arabic_Letter"}) {
            unit.bidi_type = std::string{"missing:AL"};
        } else if (unit.bidi_type == std::string_view{"European_Terminator"}) {
            unit.bidi_type = std::string{"missing:ET"};
        } else {
            throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
        }
        return unit;
    }

    DerivedBidiClassUnit ProcessUnit(std::string_view sv) const {
        if (auto [whole, match1, match2, match3] = REGEX(sv); whole) {
            char32_t first = static_cast<char32_t>(std::stoul(match1.to_string(), nullptr, 16));
            char32_t last = !match2 ? first : static_cast<char32_t>(std::stoul(match2.to_string(), nullptr, 16));
            return {
                .code_point_first=first,
                .code_point_last=last,
                .bidi_type=match3.to_string()
            };
        }
        throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
    }

public:
    using FileIterator::FileIterator;

    DerivedBidiClassFileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<DerivedBidiClassFileIterator>);


class DerivedBidiClass {
public:
    struct Data {
        char32_t code_point{0};
        jcu::bidi::BidiType value{jcu::bidi::BidiType::NIL};
    };

private:
    static constexpr auto MISSING_REGEX = ctre::match<"^(?:missing[:])?(.+)$">;

    UnicodeVersion version{};
    std::vector<Data> data{};

public:
    static constexpr const char* FILE_NAME = "DerivedBidiClass.txt";
    static constexpr std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    DerivedBidiClass(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)}
    {
        // Create a vector of BidiType for all possible code points.  Throw exception (.at) for unexpected code points.
        // Assumptions:
        //     1. Missing ranges come before assigned value ranges
        //     2. The global default L is the first range of missing followed by non-overlapping ranges in missing.
        //     3. The ranges of assigned values are non-overlapping for all ranges of assigned values, not missing.
        // TODO: Add validation steps.
        DerivedBidiClassFileIterator it{directory / FILE_NAME, OPEN_MODE};
        DerivedBidiClassFileIterator end{true};

        std::vector all_bidi_types(jcu::CODE_POINT_MAX + 1, jcu::bidi::BidiType::NIL);
        std::ranges::for_each(it, end, [&all_bidi_types](auto&& unit) mutable {
            auto result = MISSING_REGEX(unit.bidi_type);
            auto bidi_type = jcu::bidi::BidiTypeFromString(result.get<1>().to_view());
            size_t start = static_cast<size_t>(unit.code_point_first);
            size_t last  = static_cast<size_t>(unit.code_point_last);
            for (auto i : std::views::iota(start, last + 1)) { all_bidi_types.at(i) = bidi_type; }
        });

        // Condense bidi type runs
        data.reserve(1 + std::ranges::count_if(all_bidi_types | std::views::pairwise, [](auto&& pair) {
            auto [a, b] = pair; return a != b;
        }));
        data.push_back({.code_point=0, .value=all_bidi_types[0]});
        for (auto [index, pair] : std::views::enumerate(all_bidi_types | std::views::pairwise)) {
            auto [a, b] = pair;
            if (a != b) { data.push_back({.code_point=static_cast<char32_t>(index + 1), .value=b}); }
        }
        data.push_back({.code_point=(jcu::CODE_POINT_MAX + 1), .value=jcu::bidi::BidiType::NIL});
    }

    auto begin() const noexcept { return data.cbegin(); }
    auto end() const noexcept { return data.cend(); }

    auto ToBidiType(char32_t code_point) const {
        if (data.empty()) { return jcu::bidi::BidiType::NIL; }
        auto it = std::ranges::upper_bound(data, code_point, {}, &Data::code_point);
        return std::ranges::prev(it)->value;
    }

    const UnicodeVersion &Version() const { return version; }
};


}
