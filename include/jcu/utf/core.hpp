// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <cstddef>
#include <cstdint>
#include <string>


namespace jcu::utf {


enum class DecodeError : uint32_t {
    OK,
    NOT_ENOUGH_ROOM,
    INVALID_LEAD,
    INCOMPLETE_SEQUENCE,
    OVERLONG_SEQUENCE,
    INVALID_CODE_POINT
};


/***
 * Unicode constants
 */
constexpr char8_t  TRAIL_UTF8_MIN      {0x80};

// Leading (high) surrogates: d800 - dbff
// Trailing (low) surrogates: dc00 - dfff
constexpr char16_t LEAD_SURROGATE_MIN  {0xd800};
constexpr char16_t LEAD_SURROGATE_MAX  {0xdbff};
constexpr char16_t TRAIL_SURROGATE_MIN {0xdc00};
constexpr char16_t TRAIL_SURROGATE_MAX {0xdfff};
constexpr char32_t LEAD_OFFSET         {0xd7c0};     // LEAD_SURROGATE_MIN - (0x10000 >> 10)
constexpr char32_t SURROGATE_OFFSET    {0xfca02400}; // 0x10000u - (LEAD_SURROGATE_MIN << 10) - TRAIL_SURROGATE_MIN

// Maximum valid value for a Unicode code point
constexpr char32_t CODE_POINT_MAX {0x10ffff};

// Replacement character
constexpr char32_t REPLACEMENT_CHARACTER {0xfffd};


/***
 * codepoint tests
 */
constexpr bool IsTrailUTF8(char8_t ch) noexcept {
    return ch >= TRAIL_UTF8_MIN;
}


constexpr bool IsLeadSurrogateUTF16(char16_t cp) noexcept {
    return cp >= LEAD_SURROGATE_MIN && cp <= LEAD_SURROGATE_MAX;
}


constexpr bool IsTrailSurrogateUTF16(char16_t cp) noexcept {
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


/***
 * codepoint utilities
 */
constexpr size_t SequenceLength(char8_t lead_byte) noexcept {
    if (lead_byte < 0x80) { return 1; }
    else if ((lead_byte >> 5) == 0x6) { return 2; }
    else if ((lead_byte >> 4) == 0xe) { return 3; }
    else if ((lead_byte >> 3) == 0x1e) { return 4; }
    return 0;
}


constexpr size_t SequenceLength(char16_t cp) noexcept {
    return IsSurrogateUTF16(cp) ? 2 : 1;
}


constexpr std::string ToString(DecodeError e) {
    // eventually replace with c++26 reflection
    switch (e) {
    case DecodeError::OK:
        return {"OK"};
    case DecodeError::NOT_ENOUGH_ROOM:
        return {"Not enough room"};
    case DecodeError::INVALID_LEAD:
        return {"Invalid utf-16 lead surrogate"};
    case DecodeError::INCOMPLETE_SEQUENCE:
        return {"Incomplete sequence"};
    case DecodeError::OVERLONG_SEQUENCE:
        return {"overlong sequence"};
    case DecodeError::INVALID_CODE_POINT:
        return {"invalid codepoint"};
    default:
        break;
    }
    return {"Unknown DecodeError"};
}


}
