// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>
#include <bit>
#include <cstdint>
#include <format>
#include <iterator>
#include <string>
#include <string_view>

#include "jcu/utf/encode.hpp"
#include "jcu/utf/views.hpp"


namespace jcu::utf {


/***
 * Iterator for converting code points and appending each byte as a character into an output iterator.
 *
 * *Note: Derived from c++ standard library; back_insert_iterator / back_inserter
 */
template <std::output_iterator<char> O>
class CharAppendIterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using difference_type   = ptrdiff_t;
    using pointer           = void;
    using reference         = void;

    constexpr explicit CharAppendIterator(O&& iter) noexcept : out_iter(std::move(iter)) {}

    constexpr auto& operator=(const char32_t& code_point) {
        for (auto i : EncodeUTF8(code_point)) { *out_iter = static_cast<char>(i); }
        return *this;
    }

    constexpr bool  operator==(const CharAppendIterator&) const noexcept = default;
    constexpr auto& operator*()     noexcept { return *this; }
    constexpr auto& operator++()    noexcept { return *this; }
    constexpr auto  operator++(int) noexcept { return *this; }

protected:
    O out_iter;
};

constexpr auto CodePointCharAppender(auto&& c) { return CharAppendIterator(std::forward<decltype(c)>(c)); }


template <jcu::utf::IsCompatibleRange_c R>
constexpr auto CopyToFormatter(R&& rng, std::format_context& ctx) {
    std::ranges::copy(CodePointView{rng} | ReplaceInvalid(), CodePointCharAppender(ctx.out()));
    return ctx.out();
}


}


/***
 * UTF-8
 */
template <>
struct std::formatter<char8_t> : public std::formatter<uint8_t> {
    auto format(const char8_t& c, std::format_context& ctx) const {
        return std::formatter<uint8_t>::format(static_cast<uint8_t>(c), ctx);
    }
};


template <size_t N>
struct std::formatter<char8_t[N], char> : std::formatter<std::string_view> {
    auto format(const char8_t(&input)[N], std::format_context& ctx) const {
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
    }
};


template <>
struct std::formatter<std::u8string> : public std::formatter<std::string> {
    auto format(const std::u8string& input, std::format_context& ctx) const {
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
    }
};


template <>
struct std::formatter<std::u8string_view> : public std::formatter<std::string_view> {
    auto format(const std::u8string_view& input, std::format_context& ctx) const {
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
    }
};


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
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
    }
};


template <>
struct std::formatter<std::u16string_view> : public std::formatter<std::u8string_view> {
    auto format(const std::u16string_view& input, std::format_context& ctx) const {
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
    }
};


template <>
struct std::formatter<std::u16string> : public std::formatter<std::u8string_view> {
    auto format(const std::u16string& input, std::format_context& ctx) const {
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
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
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
    }
};


template <>
struct std::formatter<std::u32string_view> : public std::formatter<std::u8string_view> {
    auto format(const std::u32string_view& input, std::format_context& ctx) const {
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
    }
};


template <>
struct std::formatter<std::u32string> : public std::formatter<std::u8string_view> {
    auto format(const std::u32string& input, std::format_context& ctx) const {
        return jcu::utf::CopyToFormatter(std::forward<decltype(input)>(input), ctx);
    }
};
