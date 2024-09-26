// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <string>

#include "jcu/utf/core.hpp"


namespace jcu::utf {


// Assumes code_point has been validated; will be converted to replacement_character if not valid.
constexpr std::u8string EncodeUTF8(char32_t code_point, char32_t replacement_character=REPLACEMENT_CHARACTER) {
    if (!IsCodePointValid(code_point)) { code_point = replacement_character; }
    if (code_point < 0x80) {                     // 1 byte
        return std::u8string{static_cast<char8_t>(code_point)};
    } else if (code_point < 0x800) {             // 2 bytes
        return std::u8string{{
            static_cast<char8_t>((code_point >> 6)          | 0xc0),
            static_cast<char8_t>((code_point & 0x3f)        | 0x80)
        }};
    } else if (code_point < 0x10000) {           // 3 bytes
        return std::u8string{
            static_cast<char8_t>((code_point >> 12)         | 0xe0),
            static_cast<char8_t>(((code_point >> 6) & 0x3f) | 0x80),
            static_cast<char8_t>((code_point & 0x3f)        | 0x80)
        };
    }                                            // 4 bytes
    return std::u8string{
        static_cast<char8_t>((code_point >> 18)             | 0xf0),
        static_cast<char8_t>(((code_point >> 12) & 0x3f)    | 0x80),
        static_cast<char8_t>(((code_point >> 6) & 0x3f)     | 0x80),
        static_cast<char8_t>((code_point & 0x3f)            | 0x80)
    };
}


// Assumes code_point has been validated; will be converted to replacement_character if not valid.
constexpr std::u16string EncodeUTF16(char32_t code_point, char32_t replacement_character=REPLACEMENT_CHARACTER) {
    if (!IsCodePointValid(code_point)) { code_point = replacement_character; }
    if (IsInBMP(code_point)) { return std::u16string{static_cast<char16_t>(code_point)}; }
    // Code points from the supplementary planes are encoded via surrogate pairs
    return std::u16string{
        static_cast<char16_t>(LEAD_OFFSET + (code_point >> 10)),
        static_cast<char16_t>(TRAIL_SURROGATE_MIN + (code_point & 0x3FF))
    };
}


constexpr std::u32string EncodeUTF32(char32_t code_point, char32_t replacement_character=REPLACEMENT_CHARACTER) {
    return std::u32string{IsCodePointValid(code_point) ? code_point : replacement_character};
}


template <IsUTF_c T>
constexpr std::basic_string<T> Encode(char32_t code_point, char32_t replacement_character=REPLACEMENT_CHARACTER) {
    if      constexpr (std::same_as<T, char8_t>)  { return EncodeUTF8(code_point, replacement_character); }
    else if constexpr (std::same_as<T, char16_t>) { return EncodeUTF16(code_point, replacement_character); }
    else                                          { return EncodeUTF32(code_point, replacement_character); }
}


}
