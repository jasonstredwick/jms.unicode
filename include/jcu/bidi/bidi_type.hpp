// Copyright © 2018 Muhammad Tayyab Akram

#pragma once


#include <cstdint>

#include "jcu/utility.hpp"


namespace jcu::bidi {


/***
 * The bidirectional types of a character.
 */
enum class BidiType : uint8_t {
    NIL = 0x00,

    L   = 0x01,   // <  Strong: Left-to-Right
    R   = 0x02,   // <  Strong: Right-to-Left
    AL  = 0x03,   // <  Strong: Right-to-Left Arabic

    BN  = 0x04,   // <  Weak: Boundary Neutral
    NSM = 0x05,   // <  Weak: Non-Spacing Mark
    AN  = 0x06,   // <  Weak: Arabic Number
    EN  = 0x07,   // <  Weak: European Number
    ET  = 0x08,   // <  Weak: European Number Terminator
    ES  = 0x09,   // <  Weak: European Number Separator
    CS  = 0x0A,   // <  Weak: Common Number Separator

    WS  = 0x0B,   // <  Neutral: White Space
    S   = 0x0C,   // <  Neutral: Segment Separator
    B   = 0x0D,   // <  Neutral: Paragraph Separator
    ON  = 0x0E,   // <  Neutral: Other Neutral

    LRI = 0x0F,   // <  Format: Left-to-Right Isolate
    RLI = 0x10,   // <  Format: Right-to-Left Isolate
    FSI = 0x11,   // <  Format: First Strong Isolate
    PDI = 0x12,   // <  Format: Pop Directional Isolate
    LRE = 0x13,   // <  Format: Left-to-Right Embedding
    RLE = 0x14,   // <  Format: Right-to-Left Embedding
    LRO = 0x15,   // <  Format: Left-to-Right Override
    RLO = 0x16,   // <  Format: Right-to-Left Override
    PDF = 0x17    // <  Format: Pop Directional Formatting
};


constexpr bool IsBidiTypeStrong (BidiType value) noexcept { return InRange(value, BidiType::L,   BidiType::AL); }
constexpr bool IsBidiTypeWeak   (BidiType value) noexcept { return InRange(value, BidiType::BN,  BidiType::CS); }
constexpr bool IsBidiTypeNeutral(BidiType value) noexcept { return InRange(value, BidiType::WS,  BidiType::ON); }
constexpr bool IsBidiTypeFormat (BidiType value) noexcept { return InRange(value, BidiType::LRI, BidiType::PDF); }

constexpr bool IsBidiTypeIsolate          (BidiType value) noexcept { return InRange(value, BidiType::LRI, BidiType::PDI); }
constexpr bool IsBidiTypeIsolateInitiator (BidiType value) noexcept { return InRange(value, BidiType::LRI, BidiType::FSI); }
constexpr bool IsBidiTypeIsolateTerminator(BidiType value) noexcept { return value == BidiType::PDI; }
constexpr bool IsBidiTypeNeutralOrIsolate (BidiType value) noexcept { return InRange(value, BidiType::WS, BidiType::PDI); }
constexpr bool IsBidiTypeNumber           (BidiType value) noexcept { return InRange(value, BidiType::AN, BidiType::EN); }
constexpr bool IsBidiTypeNumberSeparator  (BidiType value) noexcept { return InRange(value, BidiType::ES, BidiType::CS); }
constexpr bool IsBidiTypeStrongOrNumber   (BidiType value) noexcept { return IsBidiTypeStrong(value) || IsBidiTypeNumber(value); }


}
