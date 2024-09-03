// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <cstddef>
#include <ranges>

#include "jcu/utf/concepts.hpp"
#include "jcu/utf/iterators.hpp"


namespace jcu::utf {


template <IsCompatibleRange_c R>
class ForwardView : public std::ranges::view_interface<ForwardView<R>> {
public:
    using Range_t = R;
    using Subrange_t = std::ranges::subrange<decltype(Range_t{}.cbegin()), decltype(Range_t{}.cend())>;

    constexpr explicit ForwardView(Range_t&&) noexcept = delete;
    constexpr explicit ForwardView(const Range_t& r) noexcept : subrange{r.cbegin(), r.cend()} {}

    friend constexpr bool operator==(const ForwardView&, const ForwardView&) = default;

    constexpr auto begin() const noexcept { return ForwardIterator<Subrange_t>{subrange}; }
    constexpr auto end()  const noexcept { return ForwardIterator<Subrange_t>{subrange, true}; }

    constexpr auto cbegin() const noexcept { return ForwardIterator<Subrange_t>{subrange}; }
    constexpr auto cend()  const noexcept { return ForwardIterator<Subrange_t>{subrange, true}; }

private:
    Subrange_t subrange{};
};


}
