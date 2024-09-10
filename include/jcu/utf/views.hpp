// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <concepts>
#include <cstddef>
#include <ranges>

#include "jcu/utf/concepts.hpp"
#include "jcu/utf/iterators.hpp"


namespace jcu::utf {


template <IsCompatibleRange_c Range_t>
struct SubrangeType {
    using type = std::ranges::subrange<
        decltype(std::declval<Range_t>().cbegin()),
        decltype(std::declval<Range_t>().cend())>;
};
template <IsCompatibleRange_c Range_t> using SubrangeType_t = SubrangeType<Range_t>::type;


template <typename Range_t, template<typename> typename Iter_t>
concept ViewIter_c = std::same_as<Iter_t<SubrangeType_t<Range_t>>, Iterator<SubrangeType_t<Range_t>>> ||
                     std::same_as<Iter_t<SubrangeType_t<Range_t>>, DataIterator<SubrangeType_t<Range_t>>>;


template <IsCompatibleRange_c R, template<typename> typename Iter_t>
requires ViewIter_c<R, Iter_t>
class ViewBase : public std::ranges::view_interface<ViewBase<R, Iter_t>> {
public:
    using Range_t = R;
    using Subrange_t = typename SubrangeType_t<Range_t>;

    constexpr explicit ViewBase(const Range_t& r, char32_t rc=REPLACEMENT_CHARACTER) noexcept
        : subrange{r.cbegin(), r.cend()}, replacement_character{rc}
        {}
    constexpr explicit ViewBase(Range_t&&, char32_t rc=REPLACEMENT_CHARACTER) noexcept = delete;

    friend constexpr bool operator==(const ViewBase&, const ViewBase&) = default;

    constexpr auto begin() const noexcept { return Iter_t<Subrange_t>{subrange, replacement_character}; }
    constexpr auto end() const noexcept { return Iter_t<Subrange_t>{true, subrange, replacement_character}; }

private:
    Subrange_t subrange{};
    char32_t replacement_character{REPLACEMENT_CHARACTER};
};


template <IsCompatibleRange_c Range_t> using View = ViewBase<Range_t, Iterator>;
template <IsCompatibleRange_c Range_t> using DataView = ViewBase<Range_t, DataIterator>;


}
