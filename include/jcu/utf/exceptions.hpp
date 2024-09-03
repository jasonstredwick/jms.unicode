// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <exception>
#include <stdexcept>

#include "jcu/utf/core.hpp"


namespace jcu::utf {


class Exception : public std::exception {
    const char* what() const noexcept override { return ""; }
};


/***
 * General errors
 */
class OutOfRange : public Exception {};


/***
 * Encoding/Decoding errors
 */
class NotEnoughRoom : public Exception {};
class InvalidLead : public Exception {};
class IncompleteSequence : public Exception {};
class OverlongSequence : public Exception {};
class InvalidCodePoint : public Exception {};


class EncodingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};


class DecodingError : public std::runtime_error {
    using std::runtime_error::runtime_error;
};


auto OnDecodeErrorThrow_f = [](DecodeError e) { throw DecodingError{ToString(e)}; };
auto OnEncodeErrorThrow_f = [](DecodeError e) { throw EncodingError{ToString(e)}; };


}
