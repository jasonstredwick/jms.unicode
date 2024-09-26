// Copyright © 2024 Jason Stredwick

#pragma once


#include <string>

#include "jcu/utf/core.hpp"


namespace jcu::utf {


constexpr std::string ToString(DecodeError e) {
    // eventually replace with c++26 reflection
    switch (e) {
    case DecodeError::OK:
        return "ok";
    case DecodeError::NOT_ENOUGH_ROOM:
        return "not enough room";
    case DecodeError::INVALID_LEAD:
        return "invalid utf-16 lead surrogate";
    case DecodeError::INCOMPLETE_SEQUENCE:
        return "incomplete sequence";
    case DecodeError::OVERLONG_SEQUENCE:
        return "overlong sequence";
    case DecodeError::INVALID_CODE_POINT:
        return "invalid codepoint";
    case DecodeError::SEQUENCE_EMPTY:
        return "sequence empty";
    default:
        break;
    }
    return "Unknown DecodeError";
}


}
