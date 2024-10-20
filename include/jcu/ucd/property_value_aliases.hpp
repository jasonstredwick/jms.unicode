// Copyright © 2024 Jason Stredwick

#pragma once

#include <algorithm>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include <ctre.hpp>

#include "jcu/ucd/base_iterator.hpp"
#include "jcu/unicode_version.hpp"


namespace jcu::ucd {


struct PropertyValueUnit {
    std::string property{};
    std::string short_name{};
    std::string long_name{};
    std::vector<std::string> optional_params{};
};


class PropertyValueFileIterator : public FileIterator<PropertyValueUnit, PropertyValueFileIterator> {
protected:
    static constexpr auto REGEX = ctre::match<"^\\s*([_a-zA-Z]+)\\s*;\\s*([^ ]+)\\s*;\\s*([^ ]+)">;
    static constexpr auto REGEX_VAL = ctre::match<"\\s*([^ ]+)\\s*">;

    bool AcceptLine() const noexcept override { return !buffer.empty() && buffer[0] != '#'; }

    PropertyValueUnit ProcessLine() override {
        PropertyValueUnit unit{};
        if (auto [whole, match1, match2, match3] = REGEX(buffer); whole) {
            unit.property = match1.to_string();
            unit.short_name = match2.to_string();
            unit.long_name = match3.to_string();
        } else {
            throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
        }
        auto it = std::ranges::find(buffer, ';');
        it = std::ranges::find(it, buffer.end(), ';');
        it = std::ranges::find(it, buffer.end(), ';');
        if (it == buffer.end()) { return unit; }
        ++it; // move off of semicolon
        unit.optional_params = std::ranges::subrange(it, buffer.end()) |
                               std::views::split(';') |
                               std::views::transform([](auto&& i) { return REGEX_VAL(i).get<1>().to_string(); }) |
                               std::ranges::to<std::vector>();
        return unit;
    }

public:
    using FileIterator::FileIterator;

    PropertyValueFileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<PropertyValueFileIterator>);


class PropertyValues {
    UnicodeVersion version{};
    std::map<std::string, std::vector<PropertyValueUnit>> data{};
    std::vector<PropertyValueUnit> empty{};

public:
    static constexpr const char* FILE_NAME = "PropertyValueAliases.txt";
    static constexpr std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    PropertyValues(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)}
    {
        auto rng = std::ranges::subrange(PropertyValueFileIterator{directory / FILE_NAME, OPEN_MODE},
                                         PropertyValueFileIterator{true});
        std::ranges::for_each(rng, [&data=this->data](auto&& unit) {
            if (auto it = data.find(unit.property); it != data.end()) {
                it->second.push_back(std::move(unit));
            } else {
                data[unit.property] = std::vector{{unit}};
            }
        });
    }

    auto begin() const { return data.cbegin(); }
    auto end() const { return data.cend(); }

    const std::vector<PropertyValueUnit>& GetPropertyValues(const std::string& property) const noexcept {
        if (auto it = data.find(property); it != data.end()) { return it->second; }
        return empty;
    }

    const UnicodeVersion &Version() const { return version; }
};


}
