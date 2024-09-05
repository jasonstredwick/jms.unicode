// Copyright © 2024 Jason Stredwick

#pragma once


#include <algorithm>


namespace jcu {


template <typename T>
constexpr bool InRange(T value, T start, T end) noexcept {
    auto bounds = std::ranges::minmax(start, end);
    return bounds.min <= value && value <= bounds.max;
}


}
