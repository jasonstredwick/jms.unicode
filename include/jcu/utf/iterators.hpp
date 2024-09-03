// Copyright © 2024 Nemanja Trifunovic

#pragma once


#include <concepts>
#include <cstddef>
#include <memory>

#include "jcu/utf/concepts.hpp"
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
    constexpr auto& operator=(const DecodeData& data) noexcept { // utilized by ForwardIterator::operator++
        consumed = data.consumed;
        code_point = data.code_point;
        error_code = data.error_code;
        return *this;
    }
    friend constexpr bool operator==(const IteratorData&, const IteratorData&) noexcept = default;
};


template <IsCompatibleRange_c Range_t>
class ForwardIterator {
public:
    using value_type      = IteratorData;
    using difference_type = ptrdiff_t;

    constexpr ForwardIterator() noexcept = default;
    constexpr explicit ForwardIterator(Range_t&&) noexcept = delete;
    constexpr explicit ForwardIterator(const Range_t& r) noexcept : rng{std::addressof(r)}, iter_data{_Decode(0), 0} {}
    constexpr explicit ForwardIterator(Range_t&&, bool) noexcept = delete;
    constexpr explicit ForwardIterator(const Range_t& r, bool /*sentinel*/) noexcept
        : rng{std::addressof(r)}, iter_data{std::ssize(r)}
        {}

    friend constexpr bool operator==(const ForwardIterator& l, const ForwardIterator& r) {
        return l.rng == r.rng && l.iter_data.index == r.iter_data.index;
    }

    constexpr auto& operator++() noexcept {
        iter_data.index = std::ranges::min(iter_data.index + iter_data.consumed, rng->size());
        iter_data = _Decode(iter_data.index);
        return *this;
    }
    constexpr auto operator++(int) noexcept { auto tmp = *this; ++*this; return tmp; }

    constexpr auto operator*() const noexcept { return iter_data; }

    constexpr const auto* operator->() const noexcept { return std::addressof(iter_data); }

    constexpr auto base() const noexcept { return std::ranges::next(rng->begin(), iter_data.index, rng->end()); }

private:
    const Range_t* rng{nullptr};
    IteratorData iter_data{};

    constexpr DecodeData _Decode(ptrdiff_t index) const noexcept {
        if constexpr (IsUTF8Compatible_c<std::ranges::range_value_t<Range_t>>) {
            return DecodeUTF8(std::ranges::subrange(rng->begin() + index, rng->end()));
        } else if constexpr (IsUTF16Compatible_c<std::ranges::range_value_t<Range_t>>) {
            return DecodeUTF16(std::ranges::subrange(rng->begin() + index, rng->end()));
        } else {
            return DecodeUTF32(std::ranges::subrange(rng->begin() + index, rng->end()));
        }
    }
};


}
