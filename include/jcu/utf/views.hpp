// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <algorithm>
#include <array>
#include <concepts>
#include <iterator>
#include <ranges>

#include "jcu/utf/concepts.hpp"
#include "jcu/utf/iterators.hpp"


namespace jcu::utf {


/***
 * Views
 */
template <typename T>
class CodePointView : public std::ranges::view_interface<CodePointView<T>> {
    using range_type = std::conditional_t<std::is_pointer_v<T>, std::remove_pointer_t<std::remove_const_t<T>>, T>;
    using base_type = std::ranges::range_value_t<range_type>;

    T rng;

public:
    template <IsCompatible_c U, size_t N>
    constexpr CodePointView(const U(&ptr)[N]) : rng{ptr, (N && ptr[N-1] != static_cast<U>('\0')) ? N : N - 1} {}

    template <IsCompatibleRange_c Range_t>
    constexpr CodePointView(const Range_t& r) : rng{std::addressof(r)} {}

    constexpr auto begin() const {
        if constexpr (std::same_as<char32_t, base_type>) {
            return std::ranges::cbegin(base());
        } else {
            return CodePointIterator{std::ranges::cbegin(base()), std::ranges::cend(base())};
        }
    }

    constexpr auto end() const {
        if constexpr (std::same_as<char32_t, base_type>) {
            return std::ranges::cend(base());
        } else {
            return CodePointIterator{std::ranges::cend(base()), std::ranges::cend(base())};
        }
    }

    constexpr const auto& base() const {
        if constexpr (std::is_pointer_v<T>) { return *rng; }
        else { return rng; }
    }
};

// Deduction guides
template <IsCompatible_c U, size_t N>  CodePointView(const U(&)[N])  -> CodePointView<std::span<const U>>;
template <IsCompatibleRange_c Range_t> CodePointView(const Range_t&) -> CodePointView<const Range_t*>;


template <typename T>
class DecodeDataView : public std::ranges::view_interface<DecodeDataView<T>> {
    using range_type = std::conditional_t<std::is_pointer_v<T>, std::remove_pointer_t<std::remove_const_t<T>>, T>;
    using base_type = std::ranges::range_value_t<range_type>;

    T rng;

public:
    template <IsCompatible_c U, size_t N>
    constexpr DecodeDataView(const U(&ptr)[N]) : rng{ptr, (N && ptr[N-1] != static_cast<U>('\0')) ? N : N - 1} {}

    template <IsCompatibleRange_c Range_t>
    constexpr DecodeDataView(const Range_t& r) : rng{std::addressof(r)} {}

    constexpr auto begin() const {
        return DecodeDataIterator{std::ranges::cbegin(base()), std::ranges::cend(base())};
    }

    constexpr auto end() const {
        return DecodeDataIterator{std::ranges::cend(base()), std::ranges::cend(base())};
    }

    constexpr const auto& base() const {
        if constexpr (std::is_pointer_v<T>) { return *rng; }
        else { return rng; }
    }
};

// Deduction guides
template <IsCompatible_c U, size_t N>  DecodeDataView(const U(&)[N])  -> DecodeDataView<std::span<const U>>;
template <IsCompatibleRange_c Range_t> DecodeDataView(const Range_t&) -> DecodeDataView<const Range_t*>;


/***
 * Range adaptors
 */
class ReplaceInvalid : public std::ranges::range_adaptor_closure<ReplaceInvalid> {
    char32_t replacement_character{REPLACEMENT_CHARACTER};

public:
    constexpr ReplaceInvalid() noexcept = default;
    constexpr ReplaceInvalid(char32_t replacement_character) noexcept : replacement_character{replacement_character} {}

    constexpr auto operator()(IsCompatibleRange_c auto && rng) {
        return std::views::transform(std::forward<decltype(rng)>(rng), [rc=replacement_character](char32_t cp) {
            return IsCodePointValid(cp) ? cp : rc;
        });
    }

    template <typename T>
    constexpr auto operator()(const DecodeDataView<T>& rng) {
        auto Transform_f = [rc=replacement_character](const auto& data) {
            return IsCodePointValid(data.code_point) ? data.code_point : rc;
        };
        return std::views::transform(std::forward<decltype(rng)>(rng), Transform_f);
    }
};


}


// Mark CodePointView as a borrowed range depending on the underlying range borrowed-ness
template<typename T>
constexpr bool std::ranges::enable_borrowed_range<jcu::utf::CodePointView<T>> =
    std::ranges::enable_borrowed_range<
        std::conditional_t<std::is_pointer_v<T>, std::remove_pointer_t<std::remove_const_t<T>>, T>>;


// Mark DecodeDataView as a borrowed range depending on the underlying range borrowed-ness
template<typename T>
constexpr bool std::ranges::enable_borrowed_range<jcu::utf::DecodeDataView<T>> =
    std::ranges::enable_borrowed_range<
        std::conditional_t<std::is_pointer_v<T>, std::remove_pointer_t<std::remove_const_t<T>>, T>>;
