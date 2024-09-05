// Copyright © 2024 Jason Stredwick

#pragma once


#include <format>
#include <map>
#include <ranges>
#include <stdexcept>
#include <string_view>

#include <ctre.hpp>

#include "parser/base_iterator.hpp"
#include "sheen_bidi/unicode_version.hpp"


namespace jcu::ucd {


enum class BracketPairedType {
    NONE,
    OPEN,
    CLOSE
};


struct BidiBracketsUnit {
    char32_t code_point{0};
    char32_t paired_code_point{0};
    BracketPairedType bracket_paired_type{BracketPairedType::NONE};
    bool operator==(const BidiBracketsUnit&) const = default;
};


class BidiBracketsFileIterator : public FileIterator<BidiBracketsUnit, BidiBracketsFileIterator> {
protected:
    static constexpr auto REGEX = ctre::match<"^\\s*([[:xdigit:]]+)(?:..([[:xdigit:]]+))?\\s*;\\s*([ocn]).*$">;

    bool AcceptLine() const noexcept override { return !buffer.empty() && buffer[0] != '#'; }

    BidiBracketsUnit ProcessLine() override {
        if (auto [whole, match1, match2, match3] = REGEX(buffer); whole) {
            std::string_view sv{match3};
            BracketPairedType paired_type = BracketPairedType::NONE;
            if (sv == std::string_view{"o"}) { paired_type = BracketPairedType::OPEN; }
            else if (sv == std::string_view{"c"}) { paired_type = BracketPairedType::CLOSE; }
            return {
                .code_point=static_cast<char32_t>(std::stoul(match1.to_string(), nullptr, 16)),
                .paired_code_point=static_cast<char32_t>(std::stoul(match2.to_string(), nullptr, 16)),
                .bracket_paired_type=paired_type
            };
        }
        throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
    }

public:
    using FileIterator::FileIterator;

    BidiBracketsFileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<BidiBracketsFileIterator>);


class BidiBrackets {
    UnicodeVersion version{};
    std::map<char32_t, BidiBracketsUnit> data{};

public:
    static constexpr const char* FILE_NAME = "BidiBrackets.txt";
    static constexpr std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    BidiBrackets(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)},
      data{std::ranges::subrange(BidiBracketsFileIterator{directory / FILE_NAME, OPEN_MODE},
                                 BidiBracketsFileIterator{true}) |
           std::views::transform([](auto&& unit) { return std::make_pair(unit.code_point, unit); }) |
           std::ranges::to<std::map>()}
    {}

    auto begin() const noexcept { return data.cbegin(); }
    auto end() const noexcept { return data.cend(); }

    BidiBracketsUnit GetData(char32_t code_point) const noexcept {
        if (auto it = data.find(code_point); it != data.end()) { return it->second; }
        return {.code_point=code_point, .bracket_paired_type=BracketPairedType::NONE};
    }

    const UnicodeVersion &Version() const { return version; }
};


}
