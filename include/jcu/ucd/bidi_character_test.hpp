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


struct TestData {
    std::vector<char32_t> text{};
    std::vector<uint8_t> levels{};
    std::vector<size_t> order{};
    ParagraphDirection paragraph_direction{ParagraphDirection::INVALID};
    uint8_t paragraph_level{0};
};


struct BidiCharacterTestUnit {
    struct TestGroup {
        std::vector<std::string> description{};
        std::vector<TestData> tests{};
    };

    std::vector<std::string> description{};
    std::vector<TestGroup> groups{};
};


class BidiCharacterTestFileIterator : public FileIterator<BidiCharacterTestUnit, BidiCharacterTestFileIterator> {
protected:
    static constexpr auto REGEX_SECTION_HEADER = ctre::match<"^[#]{5}.*$">;

    bool AcceptLine() const noexcept override { return true; }

    BidiCharacterTestUnit ProcessLine() override {
        BidiCharacterTestUnit unit{.description=ProcessSectionHeader()};

        do {
            BidiCharacterTestUnit::TestGroup group{
                .description=ProcessDescription(),
                .tests=ProcessTests()
            };
            if (group.tests.empty()) { continue; }
            unit.groups.push_back(std::move(group));
        } while (!is_done && !REGEX_SECTION_HEADER(buffer));

        return unit;
    }

    std::vector<std::string> ProcessSectionHeader() {
        std::vector<std::string> desc{};
        desc.push_back("/***");
        for (; !is_done && !buffer.empty() && buffer[0] == '#'; ReadLines([]() { return true; })) {
            std::string line{" *"};
            line.append_range(std::string_view{buffer.begin() + 1, buffer.end()});
            desc.push_back(std::move(line));
        }
        desc.push_back(" */");
        if (buffer.empty()) { ReadLines([this]() { return !buffer.empty(); }); }
        return desc;
    }

    std::vector<std::string> ProcessDescription() {
        std::vector<std::string> desc{};
        for (; !is_done; ReadLines([]() { return true; })) {
            if (!buffer.empty() && buffer[0] != '#') { break; }

            std::string line{"//"};
            if (!buffer.empty()) { line.append_range(std::string_view{buffer.begin() + 1, buffer.end()}); }
            desc.push_back(std::move(line));
        }
        return desc;
    }

    std::vector<TestData> ProcessTests() {
        std::vector<TestData> tests{};
        for (; !is_done; ReadLines([]() { return true; })) {
            if (buffer.empty()) { continue; }
            else if (buffer[0] == '#') { break; }
            tests.push_back(ParseTest());
        }
        if (buffer.empty()) { ReadLines([this]() { return !buffer.empty(); }); }
        return tests;
    }

    TestData ParseTest() const {
        TestData data{};

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
    {
        buffer.reserve(4096);
        // Skip header comments ending on the first test denoted by multiple pound symbols.
        ReadLines([this]() -> bool { return BidiCharacterTestFileIterator::REGEX_SECTION_HEADER(buffer); });
        Init();
    }
};
static_assert(std::forward_iterator<BidiCharacterTestFileIterator>);


class BidiCharacterTest {
    std::vector<TestData> test_data{};
    std::map<size_t, std::vector<std::string>> group_comments{};
    std::map<size_t, std::vector<std::string>> section_comments{};
    UnicodeVersion version{};

public:
    static constexpr const char* FILE_NAME = "BidiCharacterTest.txt";
    static const std::ios_base::openmode OPEN_MODE = std::ios::in | std::ios::binary;

    BidiCharacterTest(const std::filesystem::path& directory)
    : version{ExtractVersion(directory / FILE_NAME, OPEN_MODE)}
    {
        BidiCharacterTestFileIterator it{directory / FILE_NAME, OPEN_MODE};
        BidiCharacterTestFileIterator end{true};
        test_data.reserve(4096); // estimate for doubling purposes.
        int count = 0;
        for (; it != end; ++it) {
            section_comments[test_data.size()] = it->description;
            for (const auto& group : it->groups) {
                group_comments[test_data.size()] = group.description;
                std::ranges::transform(group.tests, std::back_inserter(test_data), std::identity{});
            }
        }
    }

    auto begin() const noexcept { return test_data.cbegin(); }
    auto end() const noexcept { return test_data.cend(); }

    auto cbegin() const noexcept { return test_data.cbegin(); }
    auto cend() const noexcept { return test_data.cend(); }

    const auto& GetTestCase(size_t index) const { return test_data.at(index); }
    const auto& TestData() const noexcept { return test_data; }
    const auto& GroupComments() const noexcept { return group_comments; }
    const auto& SectionComments() const noexcept { return section_comments; }
    size_t Size() const noexcept { return test_data.size(); }
    const UnicodeVersion &Version() const { return version; }
};


}
