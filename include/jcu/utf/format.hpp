// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>

#include "jcu/utf/format_utf8.hpp"
#include "jcu/utf/utility.hpp"


/***
 * UTF-16
 */
template <>
struct std::formatter<char16_t> : public std::formatter<uint16_t> {
    auto format(const char16_t& c, std::format_context& ctx) const {
        return std::formatter<uint16_t>::format(static_cast<uint16_t>(c), ctx);
    }
};


template <size_t N>
struct std::formatter<char16_t[N], char> : std::formatter<std::u8string_view> {
    auto format(const char16_t(&input)[N], std::format_context& ctx) const {
        std::u16string_view sv{input};
        std::u8string str = jcu::utf::ConvertToUTF8(sv);
        return std::formatter<std::u8string_view>::format(std::u8string_view{str}, ctx);
    }
};


template <>
struct std::formatter<std::u16string_view> : public std::formatter<std::u8string_view> {
    auto format(const std::u16string_view& input, std::format_context& ctx) const {
        std::u8string str = jcu::utf::ConvertToUTF8(input);
        return std::formatter<std::u8string_view>::format(std::u8string_view{str}, ctx);
    }
};


template <>
struct std::formatter<std::u16string> : public std::formatter<std::u8string_view> {
    auto format(const std::u16string& input, std::format_context& ctx) const {
        std::u8string str = jcu::utf::ConvertToUTF8(input);
        return std::formatter<std::u8string_view>::format(std::u8string_view{str}, ctx);
    }
};


/***
 * UTF-32
 */
template <>
struct std::formatter<char32_t> : public std::formatter<uint32_t> {
    auto format(const char32_t& c, std::format_context& ctx) const {
        return std::formatter<uint32_t>::format(static_cast<uint32_t>(c), ctx);
    }
};


template <size_t N>
struct std::formatter<char32_t[N], char> : std::formatter<std::u8string_view> {
    auto format(const char32_t(&input)[N], std::format_context& ctx) const {
        std::u32string_view sv{input};
        std::u8string str = jcu::utf::ConvertToUTF8(sv);
        return std::formatter<std::u8string_view>::format(std::u8string_view{str}, ctx);
    }
};


template <>
struct std::formatter<std::u32string_view> : public std::formatter<std::u8string_view> {
    auto format(const std::u32string_view& input, std::format_context& ctx) const {
        std::u8string str = jcu::utf::ConvertToUTF8(input);
        return std::formatter<std::u8string_view>::format(std::u8string_view{str}, ctx);
    }
};


template <>
struct std::formatter<std::u32string> : public std::formatter<std::u8string_view> {
    auto format(const std::u32string& input, std::format_context& ctx) const {
        std::u8string str = jcu::utf::ConvertToUTF8(input);
        return std::formatter<std::u8string_view>::format(std::u8string_view{str}, ctx);
    }
};
