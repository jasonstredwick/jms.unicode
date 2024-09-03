// Copyright © 2024 Jason Stredwick

#pragma once


#include <concepts>
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

template <typename T>
concept IsUTF32CompatibleReduced_c = std::same_as<T, char32_t> ||
                                     std::same_as<T, uint32_t>;

template <typename T> concept IsCompatible_c = IsUTF8Compatible_c<T> ||
                                               IsUTF16Compatible_c<T> ||
                                               IsUTF32Compatible_c<T>;


/***
 * Combine compatible types to define compatible range constaints
 */
template <typename T>
concept IsUTF8CompatibleRange_c = std::ranges::forward_range<T> &&
                                  IsUTF8Compatible_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsUTF16CompatibleRange_c = std::ranges::forward_range<T> &&
                                   IsUTF16Compatible_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsUTF32CompatibleRange_c = std::ranges::forward_range<T> &&
                                   IsUTF32Compatible_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsUTF32CompatibleReducedRange_c = std::ranges::forward_range<T> &&
                                          IsUTF32CompatibleReduced_c<std::ranges::range_value_t<T>>;

template <typename T>
concept IsCompatibleRange_c = std::ranges::forward_range<T> && IsCompatible_c<std::ranges::range_value_t<T>>;


}
