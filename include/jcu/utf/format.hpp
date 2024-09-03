// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>

#include "jcu/utf/decode_encode.hpp"
#include "jcu/utf/format_utf8.hpp"
#include "jcu/utf/views.hpp"


/***
 * UTF-16
 */
template <>
struct std::formatter<char16_t> : public std::formatter<uint16_t> {
    auto format(const char16_t& c, std::format_context& ctx) const {
        return std::formatter<uint16_t>::format(static_cast<uint16_t>(c), ctx);
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
