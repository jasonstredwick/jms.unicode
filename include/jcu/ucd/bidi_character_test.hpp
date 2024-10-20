// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <cstdint>
#include <limits>
#include <map>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <ctre.hpp>

#include "jcu/ucd/base_iterator.hpp"
#include "jcu/unicode_version.hpp"


namespace jcu::ucd {


constexpr uint8_t LEVEL_REMOVED = std::numeric_limits<uint8_t>::max();


enum class ParagraphDirection : uint8_t {
    LTR=0,
    RTL=1,
    AUTO=2,
    INVALID=3
};


struct BidiCharacterTestUnit {
    std::vector<char32_t> text{};
    std::vector<uint8_t> levels{};
    std::vector<size_t> order{};
    ParagraphDirection paragraph_direction{ParagraphDirection::INVALID};
    uint8_t paragraph_level{0};
    size_t line_num{0};
};


class BidiCharacterTestFileIterator : public FileIterator<BidiCharacterTestUnit, BidiCharacterTestFileIterator> {
protected:
    bool AcceptLine() const noexcept override { return !buffer.empty() && buffer[0] != '#'; }

    BidiCharacterTestUnit ProcessLine() override { return ParseTest(); }

    BidiCharacterTestUnit ParseTest() const {
        BidiCharacterTestUnit data{.line_num=GetLineNum()};

        auto columns = buffer |
                       std::views::split(';') |
                       std::views::transform([](auto&& v) { return std::string_view{v}; }) |
                       std::ranges::to<std::vector<std::string_view>>();
        if (columns.size() != 5) {
            throw std::runtime_error{std::format("Unexpected line of data ({}): {}", GetLineNum(), buffer)};
        }

        data.text = columns[0] |
                    std::views::split(' ') |
                    std::views::transform([](auto&& v) {
                        return static_cast<char32_t>(std::stoul(std::string{std::string_view{v}}, nullptr, 16));
                    }) |
                    std::ranges::to<std::vector<char32_t>>();

        auto dir = std::stoul(std::string{std::string_view{columns[1]}});
        if      (dir == 0) { data.paragraph_direction = ParagraphDirection::LTR; }
        else if (dir == 1) { data.paragraph_direction = ParagraphDirection::RTL; }
        else if (dir == 2) { data.paragraph_direction = ParagraphDirection::AUTO; }

        data.paragraph_level = static_cast<uint8_t>(std::stoul(std::string{std::string_view{columns[2]}}));

        data.levels = columns[3] |
                      std::views::split(' ') |
                      std::views::transform([](auto&& v) {
                          std::string_view sv{v};
                          if (sv[0] == 'x') { return LEVEL_REMOVED; }
                          return static_cast<uint8_t>(std::stoul(std::string{sv}));
                      }) |
                      std::ranges::to<std::vector<uint8_t>>();

        data.order = columns[4] |
                     std::views::split(' ') |
                     std::views::transform([](auto&& v) {
                         return static_cast<size_t>(std::stoull(std::string{std::string_view{v}}));
                     }) |
                     std::ranges::to<std::vector<size_t>>();

        return data;
    }

public:
    using FileIterator::FileIterator;

    BidiCharacterTestFileIterator(const std::filesystem::path& path, std::ios_base::openmode mode)
    : FileIterator{path, mode}
    { Init(); }
};
static_assert(std::forward_iterator<BidiCharacterTestFileIterator>);


class BidiCharacterTestForwardView {
    UnicodeVersion version{};
    std::filesystem::path file_path{};

public:
    static constexpr const char* FILE_NAME = "BidiCharacterTest.txt";
    static const std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    BidiCharacterTestForwardView(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)}, file_path{directory / FILE_NAME}
    {}

    auto begin() const noexcept { return BidiCharacterTestFileIterator{file_path, OPEN_MODE}; }
    auto end() const noexcept { return BidiCharacterTestFileIterator{true}; }

    auto cbegin() const noexcept { return BidiCharacterTestFileIterator{file_path, OPEN_MODE}; }
    auto cend() const noexcept { return BidiCharacterTestFileIterator{true}; }

    const UnicodeVersion &Version() const { return version; }
};


}
