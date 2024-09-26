// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <algorithm>
#include <cstddef>
#include <string>

#include "jcu/utf/concepts.hpp"
#include "jcu/utf/core.hpp"
#include "jcu/utf/iterators.hpp"
#include "jcu/utf/views.hpp"


namespace jcu::utf {


template <typename I>
struct AttemptedConvertResult {
    I cur;
    I next;
    DecodeError error_code;
};


// TODO: Should I enforce std::ranges::borrowed_range to prevent dangling iterators on temporary ranges?
template <IsUTF_c Dst_t>
constexpr auto AttemptConvertToUTF(IsCompatibleRange_c auto&& rng, std::basic_string<Dst_t>& dst) {
    DecodeDataView view{rng};
    auto out_it = CodePointAppender(dst);
    for (auto it=view.begin(); it!=view.end(); ++it) {
        if (it->error_code != DecodeError::OK) {
            return AttemptedConvertResult{
                .cur=it.base(),
                .next=it->next,
                .error_code=it->error_code
            };
        }
        out_it = it->code_point;
    }
    return AttemptedConvertResult{
        .cur=std::ranges::cend(rng),
        .next=std::ranges::cend(rng),
        .error_code=DecodeError::OK
    };
}


// Assumes input is not validated; will replace invalid code points with REPLACEMENT_CHARACTER.
template <IsCompatibleRange_c Range_t, IsUTF_c Dst_t>
constexpr std::basic_string<Dst_t> ConvertToUTF(const Range_t& rng,
                                                char32_t replacement_character=REPLACEMENT_CHARACTER,
                                                bool reserve=false) {
    std::basic_string<Dst_t> result{};
    auto view = CodePointView{rng};
    if (reserve) { result.reserve(std::ranges::distance(view)); }
    std::ranges::copy(view | ReplaceInvalid(replacement_character), CodePointAppender(result));
    return result;
}


constexpr auto ConvertToUTF8(IsCompatibleRange_c auto && rng,
                             char32_t replacement_character=REPLACEMENT_CHARACTER,
                             bool reserve=false)  {
    return ConvertToUTF<decltype(rng), char8_t>(rng, replacement_character, reserve);
}


constexpr auto ConvertToUTF16(IsCompatibleRange_c auto && rng,
                              char32_t replacement_character=REPLACEMENT_CHARACTER,
                              bool reserve=false) {
    return ConvertToUTF<decltype(rng), char16_t>(rng, replacement_character, reserve);
}


constexpr auto ConvertToUTF32(IsCompatibleRange_c auto && rng,
                              char32_t replacement_character=REPLACEMENT_CHARACTER,
                              bool reserve=false) {
    return ConvertToUTF<decltype(rng), char32_t>(rng, replacement_character, reserve);
}


// TODO: Should I enforce std::ranges::borrowed_range to prevent dangling iterators on temporary ranges?
constexpr auto FindFirstInvalid(IsCompatibleRange_c auto && rng) {
    auto IsOK_f = [](auto v) { return v == DecodeError::OK; };
    DecodeDataView view{rng};
    using Data = std::ranges::range_value_t<decltype(view)>;
    auto it = std::ranges::find_if_not(view, IsOK_f, &Data::error_code);
    return it.base();
}


constexpr bool IsValid(IsCompatibleRange_c auto && rng) {
    auto IsOK_f = [](auto v) { return v == DecodeError::OK; };
    using Data = std::ranges::range_value_t<decltype(DecodeDataView{rng})>;
    return std::ranges::all_of(DecodeDataView{rng}, IsOK_f, &Data::error_code);
}


}
