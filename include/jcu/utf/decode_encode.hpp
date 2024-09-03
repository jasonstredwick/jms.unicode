// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <cstddef>
#include <ranges>
#include <span>
#include <string_view>

#include "jcu/utf/concepts.hpp"
#include "jcu/utf/core.hpp"


namespace jcu::utf {


struct DecodeData {
    size_t consumed{0};
    char32_t code_point{REPLACEMENT_CHARACTER};
    DecodeError error_code{DecodeError::OK};

    friend constexpr bool operator==(const DecodeData&, const DecodeData&) noexcept = default;
};


constexpr DecodeData DecodeUTF8(IsUTF8CompatibleRange_c auto && rng) noexcept {
    if (rng.empty()) { return {}; }

    // Expected byte length of the utf-8 sequence, according to the lead byte
    const char8_t lead = rng[0];
    const size_t length = SequenceLength(lead);
    if (!length) { return DecodeData{.consumed=1, .error_code=DecodeError::INVALID_LEAD}; }
    else if (length == 1) {
        // No need to go through error checks here
        return DecodeData{
            .consumed=1,
            .code_point=static_cast<char32_t>(lead),
            .error_code=DecodeError::OK
        };
    }

    // Incomplete sequence may mean:
    // 1) rng does not contain the required number of bytes, or
    // 2) some of the expected trail bytes have invalid value
    if (length > rng.size()) { return DecodeData{.consumed=1, .error_code=DecodeError::INCOMPLETE_SEQUENCE}; }
    for (size_t i=1; i<length; ++i) {
        if (!IsTrailUTF8(rng[i])) {
            return DecodeData{.consumed=1, .error_code=DecodeError::INCOMPLETE_SEQUENCE};
        }
    }

    // For possible signed value ranges, need to convert to unsigned version of the same size before
    // converting to the larger unsigned type.
    auto Convert_f = [](auto v) { return static_cast<char32_t>(static_cast<char8_t>(v)); };

    // initialize to common value used in all initializations.
    char32_t code_point = static_cast<char32_t>(lead);
    switch (length) {
    case 2:
        code_point = ((code_point << 6) & 0x7ff) +
                     (Convert_f(rng[1]) & 0x3f);
        break;
    case 3:
        code_point = ((code_point        << 12) & 0xffff) +
                     ((Convert_f(rng[1]) << 6)  & 0xfff) +
                     ( Convert_f(rng[2])        & 0x3f);
        break;
    case 4:
        code_point = ((code_point        << 18) & 0x1fffff) +
                     ((Convert_f(rng[1]) << 12) & 0x3ffff) +
                     ((Convert_f(rng[2]) << 6)  & 0xfff) +
                     ( Convert_f(rng[3])        & 0x3f);
        break;
    default:
        return DecodeData{.consumed=1, .error_code=DecodeError::INVALID_LEAD};
    }

    // Decoding succeeded. Now, security checks...
    if (!IsCodePointValid(code_point)) {
        return DecodeData{.consumed=1, .error_code=DecodeError::INVALID_CODE_POINT};
    }

    if(IsOverlongSequence(code_point, length)) {
        return DecodeData{.consumed=1, .error_code=DecodeError::OVERLONG_SEQUENCE};
    }

    // Success! Change rng to point to the remainder of the original sequence
    return DecodeData{
        .consumed=length,
        .code_point=code_point,
        .error_code=DecodeError::OK
    };
}


constexpr auto DecodeUTF8(IsUTF8Compatible_c auto ch) noexcept {
    if constexpr (std::same_as<decltype(ch), char8_t>) {
        return DecodeUTF8(std::u8string_view{std::addressof(ch), 1});
    } else {
        char8_t ch8 = static_cast<char8_t>(ch);
        return DecodeUTF8(std::u8string_view{std::addressof(ch8), 1});
    }
}


template <IsUTF8Compatible_c T, size_t N>
constexpr auto DecodeUTF8(const T(&ptr)[N]) noexcept {
    if constexpr (std::same_as<T, char8_t>) {
        return DecodeUTF8(std::u8string_view{ptr});
    } else {
        return DecodeUTF8(std::string_view{ptr});
    }
}


constexpr DecodeData DecodeUTF16(IsUTF16CompatibleRange_c auto && rng) noexcept {
    if (rng.empty()) { return {}; }

    const char16_t first_word = rng[0];
    if (!IsSurrogateUTF16(first_word)) {
        return DecodeData{
            .consumed=1,
            .code_point=static_cast<char32_t>(first_word),
            .error_code=DecodeError::OK
        };
    }

    if (rng.size() < 2) {
        return DecodeData{.consumed=1, .error_code=DecodeError::INCOMPLETE_SEQUENCE};
    } else if (!IsLeadSurrogateUTF16(first_word)) {
        return DecodeData{.consumed=1, .error_code=DecodeError::INVALID_LEAD};
    }

    const char16_t second_word = rng[1];
    if (!IsTrailSurrogateUTF16(second_word)) {
        return DecodeData{.consumed=1, .error_code=DecodeError::INCOMPLETE_SEQUENCE};
    }

    char32_t code_point = SURROGATE_OFFSET +
                          (static_cast<char32_t>(first_word) << 10) +
                          static_cast<char32_t>(second_word);
    if (!IsCodePointValid(code_point)) {
        return DecodeData{.consumed=1, .error_code=DecodeError::INVALID_CODE_POINT};
    }

    return DecodeData{
        .consumed=2,
        .code_point=code_point,
        .error_code=DecodeError::OK
    };
}

constexpr auto DecodeUTF16(IsUTF16Compatible_c auto ch) noexcept {
    if constexpr (std::same_as<decltype(ch), char16_t>) {
        return DecodeUTF16(std::u16string_view{std::addressof(ch), 1});
    } else {
        return DecodeUTF16(std::span{std::addressof(ch), 1});
    }
}


template <IsUTF16Compatible_c T, size_t N>
constexpr auto DecodeUTF16(const T(&ptr)[N]) noexcept {
    if constexpr (std::same_as<T, char16_t>) {
        return DecodeUTF16(std::u16string_view{ptr});
    } else {
        return DecodeUTF16(std::span{ptr, N});
    }
}


constexpr DecodeData DecodeUTF32(IsUTF32CompatibleRange_c auto && rng) noexcept {
    if (rng.empty()) { return {}; }

    char32_t code_point = rng[0];
    if (!IsCodePointValid(code_point)) {
        if constexpr (std::same_as<wchar_t, std::ranges::range_value_t<decltype(rng)>>) {
            if (rng.size() > 1) {
                char16_t wdata[2] = {static_cast<char16_t>(rng[0]), static_cast<char16_t>(rng[1])};
                DecodeData dd = DecodeUTF16(std::span{wdata, 2});
                if (IsCodePointValid(dd.code_point)) { return dd; }
            }
        }
        return DecodeData{.consumed=1, .error_code=DecodeError::INVALID_CODE_POINT};
    }

    return DecodeData{
        .consumed=1,
        .code_point=code_point,
        .error_code=DecodeError::OK
    };
}


constexpr auto DecodeUTF32(IsUTF32Compatible_c auto ch) noexcept {
    if constexpr (std::same_as<decltype(ch), char32_t>) {
        return DecodeUTF32(std::u32string_view{std::addressof(ch), 1});
    } else {
        return DecodeUTF32(std::span{std::addressof(ch), 1});
    }
}


template <IsUTF32Compatible_c T, size_t N>
constexpr auto DecodeUTF32(const T(&ptr)[N]) noexcept {
    if constexpr (std::same_as<T, char32_t>) {
        return DecodeUTF32(std::u32string_view{ptr});
    } else if constexpr (std::same_as<T, wchar_t>) {
        return DecodeUTF32(std::wstring_view{ptr});
    } else {
        return DecodeUTF32(std::span{ptr, N});
    }
}


// Assumes code_point has been validated; will be converted to REPLACEMENT_CHARACTER if not valid.
constexpr std::u8string EncodeUTF8(char32_t code_point) {
    if (!IsCodePointValid(code_point)) { code_point = REPLACEMENT_CHARACTER; }
    if (code_point < 0x80) {                     // 1 byte
        return std::u8string{{static_cast<char8_t>(code_point)}};
    } else if (code_point < 0x800) {             // 2 bytes
        return std::u8string{{
            static_cast<char8_t>((code_point >> 6)          | 0xc0),
            static_cast<char8_t>((code_point & 0x3f)        | 0x80)
        }};
    } else if (code_point < 0x10000) {           // 3 bytes
        return std::u8string{{
            static_cast<char8_t>((code_point >> 12)         | 0xe0),
            static_cast<char8_t>(((code_point >> 6) & 0x3f) | 0x80),
            static_cast<char8_t>((code_point & 0x3f)        | 0x80)
        }};
    }                                            // 4 bytes
    return std::u8string{{
        static_cast<char8_t>((code_point >> 18)             | 0xf0),
        static_cast<char8_t>(((code_point >> 12) & 0x3f)    | 0x80),
        static_cast<char8_t>(((code_point >> 6) & 0x3f)     | 0x80),
        static_cast<char8_t>((code_point & 0x3f)            | 0x80)
    }};
}


// Assumes code_point has been validated; will be converted to REPLACEMENT_CHARACTER if not valid.
constexpr std::u16string EncodeUTF16(char32_t code_point) {
    if (!IsCodePointValid(code_point)) { code_point = REPLACEMENT_CHARACTER; }
    if (IsInBMP(code_point)) { return std::u16string{{static_cast<char16_t>(code_point)}}; }
    // Code points from the supplementary planes are encoded via surrogate pairs
    return std::u16string{{
        static_cast<char16_t>(LEAD_OFFSET + (code_point >> 10)),
        static_cast<char16_t>(TRAIL_SURROGATE_MIN + (code_point & 0x3FF))
    }};
}


constexpr std::u32string EncodeUTF32(char32_t code_point) {
    return std::u32string{{IsCodePointValid(code_point) ? code_point : REPLACEMENT_CHARACTER}};
}


template <IsUTF_c T>
constexpr std::basic_string<T> EncodeUTF(char32_t code_point) {
    if      constexpr (std::same_as<T, char8_t>)  { return EncodeUTF8(code_point); }
    else if constexpr (std::same_as<T, char16_t>) { return EncodeUTF16(code_point); }
    else                                          { return EncodeUTF32(code_point); }
}


}
