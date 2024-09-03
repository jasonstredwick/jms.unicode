// Copyright © 2024 Jason Stredwick

#pragma once


#include <cstdint>
#include <format>
#include <string>
#include <string_view>


template <>
struct std::formatter<char8_t> : public std::formatter<uint8_t> {
    auto format(const char8_t& c, std::format_context& ctx) const {
        return std::formatter<uint8_t>::format(static_cast<uint8_t>(c), ctx);
    }
};


template <size_t N>
struct std::formatter<char8_t[N], char> : std::formatter<std::string_view> {
    auto format(const char8_t(&input)[N], std::format_context& ctx) const {
        std::string_view sv{reinterpret_cast<const char*>(input), N - 1};
        return std::formatter<std::string_view>::format(sv, ctx);
    }
};


template <>
struct std::formatter<std::u8string> : std::formatter<std::string_view> {
    auto format(const std::u8string& input, std::format_context& ctx) const {
        std::string_view sv{reinterpret_cast<const char*>(input.data()), input.size()};
        return std::formatter<std::string_view>::format(sv, ctx);
    }
};


template <>
struct std::formatter<std::u8string_view> : std::formatter<std::string_view> {
    auto format(const std::u8string_view& input, std::format_context& ctx) const {
        std::string_view sv{reinterpret_cast<const char*>(input.data()), input.size()};
        return std::formatter<std::string_view>::format(sv, ctx);
    }
};
