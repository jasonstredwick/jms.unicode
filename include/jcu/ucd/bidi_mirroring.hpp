// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <format>
#include <limits>
#include <map>
#include <ranges>
#include <stdexcept>
#include <string_view>

#include <ctre.hpp>

#include "jcu/ucd/base_iterator.hpp"
#include "jcu/unicode_version.hpp"


namespace jcu::ucd {


struct BidiMirroringUnit {
    char32_t code_point{0};
    char32_t mirror_code_point{std::numeric_limits<char32_t>::max()};
    bool operator==(const BidiMirroringUnit&) const = default;
};


class BidiMirroringFileIterator : public FileIterator<BidiMirroringUnit, BidiMirroringFileIterator> {
protected:
    static constexpr auto REGEX = ctre::match<"^\\s*([[:xdigit:]]+)\\s*;\\s*([[:xdigit:]]+))\\s*#.*$">;
    static constexpr auto REGEX_UNMATCHED = ctre::match<"^\\s*([[:xdigit:]]+)\\s*;.+$">;

    bool AcceptLine() const noexcept override {
        if (buffer.empty()) { return false; }
        if (buffer[0] != '#') { return true; }
        return REGEX_UNMATCHED(buffer);
    }

    BidiMirroringUnit ProcessLine() override { return buffer[0] == '#' ? ProcessUnmatched() : ProcessData(); }

    BidiMirroringUnit ProcessData() const {
        if (auto [whole, match1, match2] = REGEX(buffer); whole) {
            return {
                .code_point=static_cast<char32_t>(std::stoul(match1.to_string(), nullptr, 16)),
                .mirror_code_point=static_cast<char32_t>(std::stoul(match2.to_string(), nullptr, 16))
            };
        }
        throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
    }

    BidiMirroringUnit ProcessUnmatched() const {
        if (auto [whole, match1] = REGEX_UNMATCHED(buffer); whole) {
            return {.code_point=static_cast<char32_t>(std::stoul(match1.to_string(), nullptr, 16))};
        }
        throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
    }

public:
    using FileIterator::FileIterator;

    BidiMirroringFileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<BidiMirroringFileIterator>);


class BidiMirroring {
    UnicodeVersion version{};
    std::map<char32_t, char32_t> data{};

public:
    static constexpr const char* FILE_NAME = "BidiMirroring.txt";
    static const std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    BidiMirroring(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)},
      data{std::ranges::subrange(BidiMirroringFileIterator{directory / FILE_NAME, OPEN_MODE},
                                 BidiMirroringFileIterator{true}) |
           std::views::transform([](auto&& unit) { return std::make_pair(unit.code_point, unit.mirror_code_point); }) |
           std::ranges::to<std::map>()}
    {}

    auto begin() const noexcept { return data.cbegin(); }
    auto end() const noexcept { return data.cend(); }

    char32_t MirrorFor(char32_t code_point) const noexcept {
        if (auto it = data.find(code_point); it != data.end()) { return it->second; }
        return std::numeric_limits<char32_t>::max();
    }

    const UnicodeVersion &Version() const { return version; }
};


}
