// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <map>
#include <ranges>
#include <set>
#include <string>
#include <stdexcept>
#include <string_view>

#include "parser/base_iterator.hpp"
#include "sheen_bidi/unicode_version.hpp"
#include "sheen_bidi/script_type.hpp"
#include "sheen_bidi/strings.hpp"
#include "jcu/constants.hpp"


namespace jcu::ucd {


struct ScriptsUnit {
    char32_t code_point_first{0};
    char32_t code_point_last{0};
    std::string script_name{};
    bool operator==(const ScriptsUnit&) const = default;
};


class ScriptsFileIterator : public FileIterator<ScriptsUnit, ScriptsFileIterator> {
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

    ScriptsUnit ProcessLine() override { return buffer[0] == '#' ? ProcessLineMissing() : ProcessLineData(); }

    ScriptsUnit ProcessLineData() const { return ProcessUnit(buffer); }

    ScriptsUnit ProcessLineMissing() const {
        ScriptsUnit unit = ProcessUnit(std::string_view{buffer} | std::views::drop(MISSING.size()));
        unit.script_name = std::string{"missing:"} + unit.script_name;
        return unit;
    }

    ScriptsUnit ProcessUnit(std::string_view sv) const {
        if (auto [whole, match1, match2, match3] = REGEX(sv); whole) {
            char32_t first = static_cast<char32_t>(std::stoul(match1.to_string(), nullptr, 16));
            char32_t last = !match2 ? first : static_cast<char32_t>(std::stoul(match2.to_string(), nullptr, 16));
            return {
                .code_point_first=first,
                .code_point_last=last,
                .script_name=match3.to_string()
            };
        }
        throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
    }

public:
    using FileIterator::FileIterator;

    ScriptsFileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<ScriptsFileIterator>);


class Scripts {
public:
    struct Data {
        char32_t code_point{0};
        ScriptType script_name{ScriptType::Nil};
    };

private:
    static constexpr auto MISSING_REGEX = ctre::match<"^(?:missing[:])?(.+)$">;

    UnicodeVersion version{};
    std::vector<Data> data{};
    std::set<std::string> script_names{"Common", "Inherited", "Unknown"};

public:
    static constexpr const char* FILE_NAME = "Scripts.txt";
    static constexpr std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    Scripts(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)}
    {
        // Create a vector of ScriptType for all possible code points.  Throw exception (.at) for unexpected code points.
        // Assumptions:
        //     1. Missing ranges come before assigned value ranges
        //     2. The global default L is the first range of missing followed by non-overlapping ranges in missing.
        //     3. The ranges of assigned values are non-overlapping for all ranges of assigned values, not missing.
        // TODO: Add validation steps.
        ScriptsFileIterator it{directory / FILE_NAME, OPEN_MODE};
        ScriptsFileIterator end{true};

        std::vector<ScriptType> all_script_names(jcu::CODE_POINT_MAX + 1, ScriptType::Nil);
        std::ranges::for_each(it, end, [&all_script_names, &script_names=this->script_names](auto&& unit) mutable {
            auto result = MISSING_REGEX(unit.script_name);
            std::string_view script_name_sv = result.get<1>().to_view();
            ScriptType script_name = ScriptFromString(script_name_sv);
            size_t start = static_cast<size_t>(unit.code_point_first);
            size_t last  = static_cast<size_t>(unit.code_point_last);
            for (auto i : std::views::iota(start, last + 1)) { all_script_names.at(i) = script_name; }
            script_names.insert(std::string{script_name_sv});
        });

        // Condense script type runs
        data.reserve(1 + std::ranges::count_if(all_script_names | std::views::pairwise, [](auto&& pair) {
            auto [a, b] = pair; return a != b;
        }));
        data.push_back({.code_point=0, .script_name=all_script_names[0]});
        for (auto [index, pair] : std::views::enumerate(all_script_names | std::views::pairwise)) {
            auto [a, b] = pair;
            if (a != b) { data.push_back({.code_point=static_cast<char32_t>(index + 1), .script_name=b}); }
        }
        data.push_back({.code_point=(jcu::CODE_POINT_MAX + 1), .script_name=ScriptType::Nil});
    }

    auto begin() const noexcept { return data.cbegin(); }
    auto end() const noexcept { return data.cend(); }

    ScriptType ToScriptType(char32_t code_point) const {
        if (data.empty()) { return ScriptType::Nil; }
        auto it = std::ranges::upper_bound(data, code_point, {}, &Data::code_point);
        return std::ranges::prev(it)->script_name;
    }

    const auto& GetScriptNames() const { return script_names; }

    const UnicodeVersion &Version() const { return version; }
};


}
