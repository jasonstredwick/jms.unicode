// Copyright © 2024 Jason Stredwick

#pragma once



#include <algorithm>
#include <cstdint>
#include <format>
#include <fstream>
#include <limits>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <ctre.hpp>

#include "jcu/bidi/bidi_type.hpp"
#include "jcu/strings/bidi_type.hpp"
#include "jcu/ucd/base_iterator.hpp"
#include "jcu/unicode_version.hpp"


namespace jcu::ucd {


constexpr uint8_t LEVEL_REMOVED = std::numeric_limits<uint8_t>::max();
constexpr uint8_t BIDI_PROPS_DIRECTION_BIT_INVALID = 0;
constexpr uint8_t BIDI_PROPS_DIRECTION_BIT_AUTO_LTR = 1;
constexpr uint8_t BIDI_PROPS_DIRECTION_BIT_LTR = 2;
constexpr uint8_t BIDI_PROPS_DIRECTION_BIT_RTL = 4;


struct BidiTestUnit {
    std::vector<uint8_t> levels{};
    std::vector<size_t> reorders{};
    std::vector<jcu::bidi::BidiType> values{};
    uint8_t direction_bitset{BIDI_PROPS_DIRECTION_BIT_INVALID};
    size_t line_num{0};
};


class BidiTestFileIterator : public FileIterator<BidiTestUnit, BidiTestFileIterator> {
protected:
    static constexpr auto REGEX_LEVELS = ctre::match<"^@Levels:\\s*(?:[x0-9]+\\s*)+.*$">;
    static constexpr auto REGEX_LEVEL_VALS = ctre::search_all<"([x0-9]+)">;
    static constexpr auto REGEX_REORDER = ctre::match<"^@Reorder:\\s*(?:[0-9]+\\s*)*.*$">;
    static constexpr auto REGEX_REORDER_VALS = ctre::search_all<"([0-9]+)">;
    static constexpr auto REGEX_DATA = ctre::match<"^\\s*(?:[A-Z]+\\s*)+;\\s*[0-9]+.*$">;
    static constexpr auto REGEX_DATA_VALS = ctre::search_all<"([A-Z]+)">;

    std::vector<uint8_t> levels{};
    std::vector<size_t> reorders{};
    bool primed{false};

    bool AcceptLine() const noexcept override { return !buffer.empty() && buffer[0] != '#'; }

    BidiTestUnit ProcessLine() override {
        if (REGEX_LEVELS(buffer)) {
            primed = true;
            levels = REGEX_LEVEL_VALS(buffer) |
                     std::views::transform([](auto&& v) {
                         if (v.to_view()[0] == 'x') { return LEVEL_REMOVED; }
                         return static_cast<uint8_t>(std::stoul(v.to_string()));
                     }) |
                     std::ranges::to<std::vector<uint8_t>>();

            this->ReadLines([]() { return true; });
            if (this->is_done) {
                throw std::runtime_error{std::format("Unexpected line of data ({}); not @Reorder.", GetLineNum())};
            } else if (!REGEX_REORDER(buffer)) {
                throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
            }
            reorders = REGEX_REORDER_VALS(buffer) |
                       std::views::transform([](auto&& v) { return static_cast<uint8_t>(std::stoul(v.to_string())); }) |
                       std::ranges::to<std::vector<size_t>>();

            this->ReadLines([this]() { return this->AcceptLine(); });
            if (this->is_done || buffer.empty() || buffer[0] == '#' || buffer[0] == '@') {
                throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
            }
        } else if (!primed) {
            throw std::runtime_error{std::format("Unexpected data before level info.")};
        }

        auto it = std::ranges::find(buffer, ';');
        std::string_view col1{buffer.begin(), it};
        std::string col2{it + 1, buffer.end()};
        auto value_strings = REGEX_DATA_VALS(col1) |
                             std::views::transform([](auto&& v) { return v.to_string(); }) |
                             std::ranges::to<std::vector<std::string>>();
        auto values = value_strings |
                      std::views::transform([](auto&& v) { return jcu::strings::bidi_type::FromString(v); }) |
                      std::ranges::to<std::vector<jcu::bidi::BidiType>>();
        for (auto [str, value] : std::views::zip(value_strings, values)) {
            if (value == jcu::bidi::BidiType::NIL) {
                throw std::runtime_error{
                    std::format("Unexpected error ({}) - Unknown bidi class: {}", GetLineNum(), str)
                };
            }
        }

        return {
            .levels=levels,
            .reorders=reorders,
            .values=std::move(values),
            .direction_bitset=static_cast<uint8_t>(std::stoul(col2)),
            .line_num=GetLineNum()
        };
    }

public:
    using FileIterator::FileIterator;

    BidiTestFileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<BidiTestFileIterator>);


class BidiTestForwardView {
    UnicodeVersion version{};
    std::filesystem::path file_path{};

public:
    static constexpr const char* FILE_NAME ="BidiTest.txt";
    static const std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    BidiTestForwardView(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)}, file_path{directory / FILE_NAME}
    {}

    auto begin() const { return BidiTestFileIterator{file_path, OPEN_MODE}; }
    auto end() const { return BidiTestFileIterator{true}; }

    auto cbegin() const { return BidiTestFileIterator{file_path, OPEN_MODE}; }
    auto cend() const { return BidiTestFileIterator{true}; }

    const UnicodeVersion &Version() const { return version; }
};


}
