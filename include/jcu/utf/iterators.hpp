// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <concepts>
#include <cstddef>
#include <memory>

#include "jcu/utf/concepts.hpp"
#include "jcu/utf/core.hpp"
#include "jcu/utf/decode_encode.hpp"


namespace jcu::utf {


/***
 * Iterator for appending code points into a container.
 *
 * *Note: Derived from c++ standard library; back_insert_iterator / back_inserter
 */
template <typename Container>
requires IsUTF_c<typename Container::value_type> && requires (Container c) { {c.append_range}; {c.push_back}; }
class CodePointAppendIterator {
public:
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using difference_type   = ptrdiff_t;
    using pointer           = void;
    using reference         = void;
    using container_type    = Container;

    constexpr explicit CodePointAppendIterator(container_type& c) noexcept : ptr(std::addressof(c)) {}

    constexpr auto& operator=(const char32_t& code_point) {
        using T = container_type::value_type;
        if constexpr (std::is_same_v<T, char8_t>)       { ptr->append_range(EncodeUTF8(code_point)); }
        else if constexpr (std::is_same_v<T, char16_t>) { ptr->append_range(EncodeUTF16(code_point)); }
        else                                            { ptr->push_back(code_point); }
        return *this;
    }

    constexpr bool  operator==(const CodePointAppendIterator&) const noexcept = default;
    constexpr auto& operator*()     noexcept { return *this; }
    constexpr auto& operator++()    noexcept { return *this; }
    constexpr auto  operator++(int) noexcept { return *this; }

protected:
    container_type* ptr;
};


constexpr auto CodePointAppender(auto& c) { return CodePointAppendIterator(c); }


/***
 * Iterator that transforms ranges of compatible UTF types into code points.
 */
struct IteratorData{
    ptrdiff_t index{0};
    size_t consumed{0};
    char32_t code_point{REPLACEMENT_CHARACTER};
    DecodeError error_code{DecodeError::OK};

    constexpr IteratorData() noexcept = default;
    constexpr IteratorData(ptrdiff_t index) noexcept : index{index} {}
    constexpr IteratorData(const DecodeData& data, ptrdiff_t index) noexcept
        : index{index}, consumed{data.consumed}, code_point{data.code_point}, error_code{data.error_code}
        {}
    constexpr auto& operator=(const DecodeData& data) noexcept { // utilized by Iterator::operator++
        consumed = data.consumed;
        code_point = data.code_point;
        error_code = data.error_code;
        return *this;
    }
    friend constexpr bool operator==(const IteratorData&, const IteratorData&) noexcept = default;
};


struct IterationSupport {
    static constexpr DecodeData Decode(IsCompatibleRange_c auto const& rng, ptrdiff_t index) noexcept {
        return DecodeUTF(std::ranges::subrange(std::ranges::next(rng.cbegin(), index, rng.cend()), rng.cend()));
    }

    static constexpr void Inc(IteratorData& data, IsCompatibleRange_c auto const& rng) {
        data.index = std::ranges::min(data.index + static_cast<ptrdiff_t>(data.consumed), std::ranges::ssize(rng));
        data = IterationSupport::Decode(rng, data.index);
    }
};


template <IsCompatibleRange_c Range_t>
class Iterator {
public:
    using difference_type = ptrdiff_t;
    using value_type = char32_t;

    constexpr Iterator() noexcept = default;
    constexpr explicit Iterator(const Range_t& r, char32_t rc=REPLACEMENT_CHARACTER) noexcept
        : rng{std::addressof(r)}, iter_data{IterationSupport::Decode(r, 0), 0}, replacement_character{rc}
        {}
    constexpr explicit Iterator(bool /*sentinel*/, const Range_t& r, char32_t rc=REPLACEMENT_CHARACTER) noexcept
        : rng{std::addressof(r)}, iter_data{std::ranges::ssize(r)}, replacement_character{rc}
        {}
    constexpr explicit Iterator(Range_t&&, char32_t rc=REPLACEMENT_CHARACTER) noexcept = delete;
    constexpr explicit Iterator(bool /*sentinel*/, Range_t&&, char32_t rc=REPLACEMENT_CHARACTER) noexcept = delete;

    friend constexpr bool operator==(const Iterator<Range_t>& lhs, const Iterator<Range_t>& rhs) {
        return lhs.rng == rhs.rng && lhs.iter_data.index == rhs.iter_data.index;
    }

    constexpr auto& operator++() noexcept { IterationSupport::Inc(iter_data, *rng); return *this; }
    constexpr auto operator++(int) noexcept { auto tmp = *this; ++*this; return tmp; }
    constexpr auto operator*() const noexcept {
        return iter_data.error_code == DecodeError::OK ? iter_data.code_point : replacement_character;
    }
    constexpr auto const* operator->() const noexcept { return std::addressof(iter_data); }
    constexpr auto base()       const noexcept { return std::ranges::next(rng->begin(), iter_data.index, rng->end()); }
    constexpr auto base_index() const noexcept { return iter_data.index; }
    constexpr auto consumed()   const noexcept { return iter_data.consumed; }
    constexpr auto error_code() const noexcept { return iter_data.error_code; }

protected:
    const Range_t* rng{nullptr};
    IteratorData iter_data{};
    value_type replacement_character{REPLACEMENT_CHARACTER};
};
static_assert(std::forward_iterator<Iterator<std::string_view>>);


template <IsCompatibleRange_c Range_t>
class DataIterator : public Iterator<Range_t> {
public:
    using value_type = IteratorData;
    using Iterator<Range_t>::Iterator;
    constexpr auto& operator++() noexcept { Iterator<Range_t>::operator++(); return *this; }
    constexpr auto operator++(int) noexcept { auto tmp = *this; Iterator<Range_t>::operator++(); return *this; }
    constexpr auto operator*() const noexcept { return this->iter_data; }
};
static_assert(std::forward_iterator<DataIterator<std::string_view>>);

// Deduction guides
template <typename T>
DataIterator(const T& t1) -> DataIterator<std::remove_cvref_t<T>>;

template <typename T>
DataIterator(const T& t1, char32_t t2) -> DataIterator<std::remove_cvref_t<T>>;

template <typename T>
DataIterator(bool t1, const T& t2) -> DataIterator<std::remove_cvref_t<T>>;

template <typename T>
DataIterator(bool t1, const T& t2, char32_t t3) -> DataIterator<std::remove_cvref_t<T>>;


}
