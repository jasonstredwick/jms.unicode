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


template <IsUTF_c Dst_t>
constexpr IteratorData AttemptConvertToUTF(IsCompatibleRange_c auto && rng,
                                           std::basic_string<Dst_t>& dst,
                                           char32_t replacement_character=REPLACEMENT_CHARACTER) {
    DataView src_view{rng, replacement_character};
    auto out_it = CodePointAppender(dst);
    for (auto it=src_view.begin(); it!=src_view.end(); ++it) {
        if (it->error_code != DecodeError::OK) { return *it; }
        out_it = it->code_point;
    }
    return {std::ranges::ssize(rng)};
}


// Assumes input is validated; will replace invalid code points with REPLACEMENT_CHARACTER.
// User can consider using shrink_to_fit to reduce overall allocated size if concerned about footprint.
template <IsCompatibleRange_c Range_t, IsUTF_c Dst_t>
constexpr std::basic_string<Dst_t> ConvertToUTF(const Range_t& rng,
                                                char32_t replacement_character=REPLACEMENT_CHARACTER) {
    std::basic_string<Dst_t> result{};
    result.reserve(std::ranges::size(rng));
    std::ranges::copy(View{rng, replacement_character}, CodePointAppender(result));
    return result;
}


constexpr auto ConvertToUTF8(IsCompatibleRange_c auto && rng, char32_t replacement_character=REPLACEMENT_CHARACTER)  {
    return ConvertToUTF<decltype(rng), char8_t>(rng, replacement_character);
}


constexpr auto ConvertToUTF16(IsCompatibleRange_c auto && rng, char32_t replacement_character=REPLACEMENT_CHARACTER) {
    return ConvertToUTF<decltype(rng), char16_t>(rng, replacement_character);
}


constexpr auto ConvertToUTF32(IsCompatibleRange_c auto && rng, char32_t replacement_character=REPLACEMENT_CHARACTER) {
    return ConvertToUTF<decltype(rng), char32_t>(rng, replacement_character);
}


constexpr bool Empty(IsCompatibleRange_c auto && rng) {
    View view{rng};
    return rng.begin() == rng.end();
}


constexpr auto FindFirstInvalid(IsCompatibleRange_c auto && rng) {
    auto IsOK_f = [](auto v) { return v == DecodeError::OK; };
    DataView view{rng};
    auto it = std::ranges::find_if_not(view, IsOK_f, &IteratorData::error_code);
    return *it;
}


constexpr bool IsValid(IsCompatibleRange_c auto && rng) {
    auto IsOK_f = [](auto v) { return v == DecodeError::OK; };
    return std::ranges::all_of(DataView{rng}, IsOK_f, &IteratorData::error_code);
}


constexpr auto Length32(IsCompatibleRange_c auto && rng) {
    return std::ranges::count_if(View{rng}, [](auto) { return true; });
}


}
