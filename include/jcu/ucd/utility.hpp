// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>

#include <ctre.hpp>

#include "jcu/unicode_version.hpp"


namespace jcu::ucd {


static constexpr auto REGEX_UNICODE_VERSION = ctre::match<"^[^\\-]+[\\-]([0-9]+)[.]([0-9]+)[.]([0-9]+)[.].+">;


size_t DataFileNumChars(std::ifstream& fs) {
    size_t cur = fs.tellg();
    fs.seekg(0, std::ios_base::end);
    size_t length = fs.tellg();
    fs.seekg(cur, std::ios_base::beg);
    return length;
}


size_t DataFileNumLines(std::ifstream& fs) {
    size_t cur = fs.tellg();
    fs.seekg(0, std::ios_base::beg);
    size_t count = 1 + std::ranges::count(std::istreambuf_iterator<char>(fs), std::istreambuf_iterator<char>(), '\n');
    fs.seekg(cur, std::ios_base::beg);
    return count;
}


std::ifstream OpenDataFile(const std::filesystem::path& path,
                           std::ios_base::openmode mode=(std::ios::in | std::ios::binary)) {
    if (!std::filesystem::exists(path)) {
        throw std::runtime_error{std::format("File not found: {}\n{}",
                                 path.generic_string(), std::filesystem::absolute(path).generic_string())};
    }

    std::ifstream fs{path, mode};
    if (!fs.is_open()) {
        throw std::runtime_error{std::format("File failed to open: {}\n{}",
                                 path.generic_string(), std::filesystem::absolute(path).generic_string())};
    }

    return fs;
}


jcu::UnicodeVersion ExtractVersion(const std::filesystem::path& path, std::ios_base::openmode mode) {
    std::ifstream fs = OpenDataFile(path, mode);
    std::string version_line{};
    version_line.reserve(4096);
    std::getline(fs, version_line);
    if (fs.rdstate() != std::ios_base::goodbit) {
        throw std::runtime_error{std::format("Failed open file: {}", path.generic_string())};
    }
    if (auto [whole, major, minor, micro] = REGEX_UNICODE_VERSION(version_line); whole) {
        return {
            .major=static_cast<uint8_t>(std::stoul(major.to_string())),
            .minor=static_cast<uint8_t>(std::stoul(minor.to_string())),
            .micro=static_cast<uint8_t>(std::stoul(micro.to_string()))
        };
    }
    throw std::runtime_error{std::format("Failed to parse version: {}", version_line)};
}


}
