// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <array>
#include <concepts>
#include <iterator>
#include <span>

#include "jcu/constants.hpp"
#include "jcu/utf/concepts.hpp"
#include "jcu/utf/core.hpp"


namespace jcu::utf {


template <std::input_iterator I>
struct DecodeData {
    I next{};
    char32_t code_point{0};
    DecodeError error_code{DecodeError::OK};

    friend constexpr bool operator==(const DecodeData&, const DecodeData&) noexcept = default;
};


template <IsUTF8CompatibleIterator_c I, std::sentinel_for<I> S=I>
constexpr DecodeData<I> Decode(I begin, S end) noexcept {
    const char32_t lead = Enlarge(*begin);
    ++begin;

    auto OnError = [saved_next=begin](DecodeError err) -> DecodeData<I> {
        return {
            .next=saved_next,
            .code_point=jcu::CODE_POINT_INVALID,
            .error_code=err
        };
    };

    const size_t length = SequenceLength8(lead);
    if (!length) { return OnError(DecodeError::INVALID_LEAD); }
    else if (length == 1) {
        // No need to go through error checks here
        return {
            .next=begin,
            .code_point=lead,
            .error_code=DecodeError::OK
        };
    }

    std::array<char32_t, 4> data{lead, 0, 0, 0};

    // Incomplete sequence may mean:
    // 1) rng does not contain the required number of bytes, or
    // 2) some of the expected trail bytes have invalid value
    for (size_t i=1; i<length; ++i, ++begin) {
        if (begin == end) { return OnError(DecodeError::INCOMPLETE_SEQUENCE); }

        data[i] = Enlarge(*begin);

        if (!IsTrailUTF8(data[i])) { return OnError(DecodeError::INCOMPLETE_SEQUENCE); }
    }

    // initialize to common value used in all initializations.
    char32_t code_point = [](const std::array<char32_t, 4>& data, const size_t length) -> char32_t {
        switch (length) {
        case 2:
            return ((data[0] << 6) & 0x7ff) +
                   ( data[1]       & 0x3f);
        case 3:
            return ((data[0] << 12) & 0xffff) +
                   ((data[1] << 6)  & 0xfff) +
                   ( data[2]        & 0x3f);
        case 4:
            return ((data[0] << 18) & 0x1fffff) +
                   ((data[1] << 12) & 0x3ffff) +
                   ((data[2] << 6)  & 0xfff) +
                   ( data[3]        & 0x3f);
        }
        return jcu::CODE_POINT_INVALID;
    }(data, length);

    // Decoding succeeded. Now, security checks...
    if (IsOverlongSequence(code_point, length)) { return OnError(DecodeError::OVERLONG_SEQUENCE); }

    // Success! Change rng to point to the remainder of the original sequence
    return {
        .next=begin,
        .code_point=code_point,
        .error_code=DecodeError::OK
    };
}


template <IsUTF16CompatibleIterator_c I, std::sentinel_for<I> S=I>
constexpr DecodeData<I> Decode(I begin, S end) noexcept {
    const char32_t first_word = Enlarge(*begin);
    ++begin;

    if (!IsSurrogateUTF16(first_word)) { return {.next=begin, .code_point=first_word, .error_code=DecodeError::OK}; }

    auto OnError = [saved_next=begin](DecodeError err) -> DecodeData<I> {
        return {
            .next=saved_next,
            .code_point=jcu::CODE_POINT_INVALID,
            .error_code=err
        };
    };

    if (begin == end) { return OnError(DecodeError::INCOMPLETE_SEQUENCE); }
    else if (!IsLeadSurrogateUTF16(first_word)) { return OnError(DecodeError::INVALID_LEAD); }

    const char32_t second_word = Enlarge(*begin);
    if (!IsTrailSurrogateUTF16(second_word)) { return OnError(DecodeError::INCOMPLETE_SEQUENCE); }

    return {
        .next=(++begin), // consume second code point
        .code_point=(SURROGATE_OFFSET + (first_word << 10) + second_word),
        .error_code=DecodeError::OK
    };
}


template <IsUTF32CompatibleReducedIterator_c I, std::sentinel_for<I> S=I>
constexpr DecodeData<I> Decode(I begin, S end) noexcept {
    const char32_t code_point = Enlarge(*begin);
    ++begin;
    return {.next=begin, .code_point=code_point, .error_code=DecodeError::OK};
}


template <IsWCharIterator_c I, std::sentinel_for<I> S=I>
constexpr DecodeData<I> Decode(I begin, S end) noexcept {
    const wchar_t code_point = *begin;
    ++begin;
    if (begin == end) {
        std::array tmp{static_cast<char16_t>(code_point)};
        auto dd = Decode(tmp.begin(), tmp.end());
        return {.next=begin, .code_point=dd.code_point, .error_code=dd.error_code};
    } else {
        std::array tmp{static_cast<char16_t>(code_point), static_cast<char16_t>(*begin)};
        auto dd = Decode(tmp.begin(), tmp.end());
        // On error, don't consume any more from begin.  Errors are one code point reads.
        // Otherwise decode consumed two code points.
        if (dd.error_code == DecodeError::OK && dd.next == tmp.end()) { ++begin; } 
        return {.next=begin, .code_point=dd.code_point, .error_code=dd.error_code};
    }
}


constexpr auto Decode(IsCompatible_c auto ch) noexcept {
    auto result = Decode(std::addressof(ch), std::addressof(ch) + 1);
    return DecodeData<decltype(ch) const*>{
        .next=nullptr,
        .code_point=result.code_point,
        .error_code=result.error_code
    };
}


template <IsCompatible_c Value_t, size_t N>
constexpr auto Decode(const Value_t(&ptr)[N]) noexcept {
    size_t total = (N && ptr[N - 1] == static_cast<Value_t>('\0')) ? N - 1 : N;
    if (!total) {
        return DecodeData<Value_t const*>{
            .next=ptr,
            .code_point=jcu::CODE_POINT_INVALID,
            .error_code=DecodeError::SEQUENCE_EMPTY
        };
    }
    return Decode(ptr, ptr + total);
}


constexpr auto Decode(IsCompatibleRange_c auto const& rng) noexcept {
    auto first = std::ranges::cbegin(rng);
    auto end = std::ranges::cend(rng);
    if (first == end) {
        return DecodeData<decltype(first)>{
            .next=end,
            .code_point=jcu::CODE_POINT_INVALID,
            .error_code=DecodeError::SEQUENCE_EMPTY
        };
    }
    return Decode(first, end);
}


}
