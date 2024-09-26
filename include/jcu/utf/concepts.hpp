// Copyright © 2024 Jason Stredwick

#pragma once


#include <concepts>
#include <iterator>
#include <ranges>


namespace jcu::utf {


/***
 * Constrain valid UTF types
 */
template <typename T>
concept IsUTF_c = std::same_as<T, char8_t>  ||
                  std::same_as<T, char16_t> ||
                  std::same_as<T, char32_t>;


/***
 * Define compatible/convertible types for UTF types
 */
template <typename T>
concept IsUTF8Compatible_c =  std::same_as<T, char8_t> ||
                              std::same_as<T, uint8_t> ||
                              std::same_as<T, char>    ||
                              std::same_as<T, unsigned char>;

template <typename T>
concept IsUTF16Compatible_c = std::same_as<T, char16_t> ||
                              std::same_as<T, uint16_t>;

template <typename T>
concept IsUTF32Compatible_c = std::same_as<T, char32_t> ||
                              std::same_as<T, uint32_t> ||
                              std::same_as<T, wchar_t>;

template <typename T> concept IsCompatible_c = IsUTF8Compatible_c<T> ||
                                               IsUTF16Compatible_c<T> ||
                                               IsUTF32Compatible_c<T>;

// Special use as they are the same size but different storage requirements.
template <typename T>
concept IsUTF32CompatibleReduced_c = std::same_as<T, char32_t> ||
                                     std::same_as<T, uint32_t>;

template <typename T>
concept IsWChar_c = std::same_as<T, wchar_t>;


/***
 * Extract the UTF type associated with the capatible type.
 */
template <IsCompatible_c T> struct ConvertCompatible;
template <IsUTF8Compatible_c T>  struct ConvertCompatible<T> { using type = char8_t; };
template <IsUTF16Compatible_c T> struct ConvertCompatible<T> { using type = char16_t; };
template <IsUTF32Compatible_c T> struct ConvertCompatible<T> { using type = char32_t; };
template <typename T>
using ConvertCompatible_t = ConvertCompatible<T>::type;


/***
 * Combine compatible types to define compatible iterator constraints
 */
template <typename T>
concept IsCompatibleIterator_c = std::input_iterator<T> && IsCompatible_c<std::iter_value_t<T>>;

template <typename T>
concept IsUTF8CompatibleIterator_c = std::input_iterator<T> && IsUTF8Compatible_c<std::iter_value_t<T>>;

template <typename T>
concept IsUTF16CompatibleIterator_c = std::input_iterator<T> && IsUTF16Compatible_c<std::iter_value_t<T>>;

template <typename T>
concept IsUTF32CompatibleIterator_c = std::input_iterator<T> && IsUTF32Compatible_c<std::iter_value_t<T>>;

template <typename T>
concept IsUTF32CompatibleReducedIterator_c = std::input_iterator<T> &&
                                             IsUTF32CompatibleReduced_c<std::iter_value_t<T>>;

template <typename T>
concept IsWCharIterator_c = std::input_iterator<T> && IsWChar_c<std::iter_value_t<T>>;


/***
 * Combine compatible types to define compatible range constraints
 */
template <typename T>
concept IsCompatibleRange_c = std::ranges::input_range<T> && IsCompatible_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsUTF8CompatibleRange_c = std::ranges::input_range<T> && IsUTF8Compatible_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsUTF16CompatibleRange_c = std::ranges::input_range<T> && IsUTF16Compatible_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsUTF32CompatibleRange_c = std::ranges::input_range<T> && IsUTF32Compatible_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsUTF32CompatibleReducedRange_c = std::ranges::input_range<T> &&
                                          IsUTF32CompatibleReduced_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsWCharRange_c = std::ranges::input_range<T> && IsWChar_c<std::ranges::range_value_t<T>>;


}
