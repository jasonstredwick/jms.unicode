// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <concepts>
#include <cstddef>
#include <iterator>
#include <memory>

#include "jcu/utf/concepts.hpp"
#include "jcu/utf/core.hpp"
#include "jcu/utf/decode.hpp"
#include "jcu/utf/encode.hpp"


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
        if constexpr (std::same_as<T, char8_t>)       { ptr->append_range(EncodeUTF8(code_point)); }
        else if constexpr (std::same_as<T, char16_t>) { ptr->append_range(EncodeUTF16(code_point)); }
        else                                          { ptr->push_back(code_point); }
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
 * Iterator that transforms ranges of UTF compatible types into code points.
 */
template <IsCompatibleIterator_c Iter_t, std::sentinel_for<Iter_t> Sentinel_t=Iter_t>
class CodePointIterator {
public:
    using value_type = char32_t;
    using difference_type = std::iter_difference_t<Iter_t>;
    using iterator_type = Iter_t;
    using sentinel_type = Sentinel_t;

protected:
    using data_type = std::iter_value_t<Iter_t>;

    template <IsCompatible_c T>
    struct State {
        iterator_type cur{};
        sentinel_type sentinel{};
        DecodeData<iterator_type> data{};
    };

    template <IsUTF32CompatibleReduced_c T>
    struct State<T> {
        iterator_type cur{};
    };

    State<data_type> state{};

public:
    constexpr CodePointIterator() = default;

    constexpr CodePointIterator(iterator_type first, sentinel_type sentinel)
    requires IsUTF32CompatibleReduced_c<data_type>
    : state{.cur=first}
    {}

    constexpr CodePointIterator(iterator_type first, sentinel_type sentinel)
    requires IsCompatible_c<data_type> && (!IsUTF32CompatibleReduced_c<data_type>)
    : state{
        .cur=first,
        .sentinel=sentinel,
        .data{(first == sentinel ? DecodeData<iterator_type>{.next=first} : Decode(first, sentinel))}
    }
    {}

    constexpr auto base() const noexcept { return state.cur; }

    constexpr bool operator==(const CodePointIterator<iterator_type, sentinel_type>& rhs) const
    requires std::forward_iterator<iterator_type>
    {
        if constexpr (IsUTF32CompatibleReduced_c<data_type>) {
            return state.cur == rhs.state.cur;
        } else {
            return state.cur == rhs.state.cur && state.sentinel == rhs.state.sentinel;
        }
    }

    constexpr value_type operator*() const
    requires std::input_iterator<iterator_type>
    {
        if constexpr (IsUTF32CompatibleReduced_c<data_type>) {
            return static_cast<value_type>(*state.cur);
        } else {
            return state.data.code_point;
        }
    }

    constexpr auto& operator++()
    requires std::input_iterator<iterator_type>
    {
        if constexpr (IsUTF32CompatibleReduced_c<data_type>) {
            ++state.cur;
        } else {
            state.cur = state.data.next;
            if (state.cur == state.sentinel) {
                state.data = {.next=state.cur};
            } else {
                state.data = Decode(state.cur, state.sentinel);
            }
        }
        return *this;
    }

    constexpr auto operator++(int)
    requires std::input_iterator<iterator_type>
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr auto& operator--()
    requires std::bidirectional_iterator<iterator_type> && IsUTF32CompatibleReduced_c<data_type>
    {
        --state.cur;
        return *this;
    }

    constexpr auto operator--(int)
    requires std::bidirectional_iterator<iterator_type> && IsUTF32CompatibleReduced_c<data_type>
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }
};


template <IsCompatibleIterator_c Iter_t, std::sentinel_for<Iter_t> Sentinel_t=Iter_t>
class DecodeDataIterator {
public:
    using iterator_type = Iter_t;
    using sentinel_type = Sentinel_t;
    using difference_type = std::iter_difference_t<iterator_type>;
    using value_type = DecodeData<iterator_type>;

protected:
    using data_type = std::iter_value_t<iterator_type>;

    struct State {
        iterator_type cur{};
        sentinel_type sentinel{};
        value_type data{};
    };

    State state{};

    auto _Decode(iterator_type i1, iterator_type i2) {
        auto result = Decode(i1, i2);
        if (result.error_code == DecodeError::OK && !IsCodePointValid(result.code_point)) {
            result.error_code = DecodeError::INVALID_CODE_POINT;
        }
        return result;
    }

public:
    constexpr DecodeDataIterator() = default;

    constexpr DecodeDataIterator(iterator_type first, sentinel_type sentinel)
    requires IsCompatible_c<data_type>
    : state{
        .cur=first,
        .sentinel=sentinel,
        .data={(first == sentinel ? value_type{.next=first} : _Decode(first, sentinel))}
    }
    {}

    constexpr auto base() const noexcept { return state.cur; }

    constexpr bool operator==(const DecodeDataIterator<iterator_type, sentinel_type>& rhs) const
    requires std::forward_iterator<iterator_type>
    {
        return state.cur == rhs.state.cur && state.sentinel == rhs.state.sentinel;
    }

    constexpr value_type operator*() const
    requires std::input_iterator<iterator_type>
    {
        return state.data;
    }

    constexpr const value_type* operator->() const
    requires std::input_iterator<iterator_type>
    {
        return std::addressof(state.data);
    }

    constexpr auto& operator++()
    requires std::input_iterator<iterator_type>
    {
        state.cur = state.data.next;
        if (state.cur == state.sentinel) {
            state.data = {.next=state.cur};
        } else {
            state.data = _Decode(state.cur, state.sentinel);
        }
        return *this;
    }

    constexpr auto operator++(int)
    requires std::input_iterator<iterator_type>
    {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    constexpr auto& operator--()
    requires std::bidirectional_iterator<iterator_type> && IsUTF32CompatibleReduced_c<data_type>
    {
        --state.cur;
        return *this;
    }

    constexpr auto operator--(int)
    requires std::bidirectional_iterator<iterator_type> && IsUTF32CompatibleReduced_c<data_type>
    {
        auto tmp = *this;
        --*this;
        return tmp;
    }
};


}
