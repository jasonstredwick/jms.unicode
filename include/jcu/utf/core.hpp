// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "jcu/constants.hpp"
#include "jcu/utf/concepts.hpp"


namespace jcu::utf {


enum class DecodeError : uint32_t {
    OK,
    NOT_ENOUGH_ROOM,
    INVALID_LEAD,
    INCOMPLETE_SEQUENCE,
    OVERLONG_SEQUENCE,
    INVALID_CODE_POINT,
    SEQUENCE_EMPTY
};


/***
 * Unicode constants
 */
constexpr char32_t TRAIL_UTF8_MIN      {0x80};

// Leading (high) surrogates: d800 - dbff
// Trailing (low) surrogates: dc00 - dfff
constexpr char32_t LEAD_SURROGATE_MIN  {0xd800};
constexpr char32_t LEAD_SURROGATE_MAX  {0xdbff};
constexpr char32_t TRAIL_SURROGATE_MIN {0xdc00};
constexpr char32_t TRAIL_SURROGATE_MAX {0xdfff};
constexpr char32_t LEAD_OFFSET         {0xd7c0};     // LEAD_SURROGATE_MIN - (0x10000 >> 10)
constexpr char32_t SURROGATE_OFFSET    {0xfca02400}; // 0x10000u - (LEAD_SURROGATE_MIN << 10) - TRAIL_SURROGATE_MIN

// Maximum valid value for a Unicode code point
constexpr char32_t CODE_POINT_MAX {jcu::CODE_POINT_MAX};

// Replacement character
constexpr char32_t REPLACEMENT_CHARACTER {0xfffd};


/***
 * codepoint tests
 */
constexpr bool IsTrailUTF8(char32_t ch) noexcept {
    return ch >= TRAIL_UTF8_MIN;
}


constexpr bool IsLeadSurrogateUTF16(char32_t cp) noexcept {
    return cp >= LEAD_SURROGATE_MIN && cp <= LEAD_SURROGATE_MAX;
}


constexpr bool IsTrailSurrogateUTF16(char32_t cp) noexcept {
    return cp >= TRAIL_SURROGATE_MIN && cp <= TRAIL_SURROGATE_MAX;
}


constexpr bool IsSurrogateUTF16(char32_t cp) noexcept {
    return cp >= LEAD_SURROGATE_MIN  && cp <= TRAIL_SURROGATE_MAX;
}


constexpr bool IsCodePointValid(char32_t cp) noexcept {
    return cp <= CODE_POINT_MAX && !IsSurrogateUTF16(cp);
}


constexpr bool IsInBMP(char32_t cp) noexcept {
    return cp < 0x10000;
}


/***
 * codepoint utilities
 */
// For possible signed value ranges, need to convert to unsigned version of the same size before
// converting to the larger unsigned type.
constexpr char32_t Enlarge(IsCompatible_c auto cp) noexcept {
    if constexpr (sizeof(decltype(cp)) < sizeof(char32_t) && std::is_signed_v<decltype(cp)>) {
        return static_cast<char32_t>(static_cast<ConvertCompatible_t<decltype(cp)>>(cp));
    } else {
        return static_cast<char32_t>(cp);
    }
};


constexpr bool IsOverlongSequence(const char32_t cp, const size_t length) noexcept {
    if (cp < 0x80) {
        if (length != 1) { return true; }
    } else if (cp < 0x800) {
        if (length != 2) { return true; }
    } else if (cp < 0x10000) {
        if (length != 3) { return true; }
    }
    return false;
}


constexpr size_t SequenceLength8(char32_t cp) noexcept {
    if (cp < 0x80) { return 1; }
    else if ((cp >> 5) == 0x6) { return 2; }
    else if ((cp >> 4) == 0xe) { return 3; }
    else if ((cp >> 3) == 0x1e) { return 4; }
    return 0;
}


constexpr size_t SequenceLength16(char32_t cp) noexcept {
    return IsSurrogateUTF16(cp) ? 2 : 1;
}


}
