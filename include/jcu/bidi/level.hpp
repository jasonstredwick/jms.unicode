// Copyright © 2014-2018 Muhammad Tayyab Akram

#pragma once


#include <cstdint>


namespace jcu::bidi {


using BidiLevel = uint8_t;


constexpr BidiLevel LEVEL_TYPE_INVALID     = 0xFF; // A value representing an invalid bidi level.
constexpr BidiLevel LEVEL_TYPE_MAX         = 125;  // A value representing maximum explicit embedding level.
constexpr BidiLevel LEVEL_TYPE_DEFAULT_LTR = 0xFE; // A value specifying to set base level to zero (left-to-right) if there is no strong character.
constexpr BidiLevel LEVEL_TYPE_DEFAULT_RTL = 0xFD; // A value specifying to set base level to one (right-to-left) if there is no strong character.


}
