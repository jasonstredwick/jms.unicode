// Copyright © 2024 Nemanja Trifunovic

#include <algorithm>
#include <array>
#include <cstdint>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

#include "jcu/utf/core.hpp"
#include "jcu/utf/decode.hpp"
#include "ftest.h"


bool Check(const auto& lhs, auto&& next, char32_t cp, jcu::utf::DecodeError ec) {
    return lhs.next == next && lhs.code_point == cp && lhs.error_code == ec;
}


TEST(DecodeEncodeTests, test_Decode8) {
    using namespace jcu::utf;

    uint8_t ch1 = static_cast<uint8_t>(u8'a');
    unsigned char ch2 = static_cast<unsigned char>('a');
    EXPECT_TRUE(Check(Decode(u8'a'), nullptr, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(  'a'), nullptr, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(  ch1), nullptr, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(  ch2), nullptr, U'a', DecodeError::OK));

    const char8_t c1[] = u8"a";
    const char8_t c2[] = u8"ш";
    const char8_t c3[] = u8"水";
    const char8_t c4[] = u8"𐌀";
    const char c5[] = "a";
    const char c6[] = "ш";
    const char c7[] = "水";
    const char c8[] = "𐌀";
    EXPECT_TRUE(Check(Decode(c1), static_cast<const char8_t*>(c1) + 1, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c2), static_cast<const char8_t*>(c2) + 2, U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c3), static_cast<const char8_t*>(c3) + 3, U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c4), static_cast<const char8_t*>(c4) + 4, U'𐌀', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c5), static_cast<const char*>(c5) + 1, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c6), static_cast<const char*>(c6) + 2, U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c7), static_cast<const char*>(c7) + 3, U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c8), static_cast<const char*>(c8) + 4, U'𐌀', DecodeError::OK));

    const char8_t c11[] = u8"ab";
    const char8_t c12[] = u8"шb";
    const char8_t c13[] = u8"水b";
    const char8_t c14[] = u8"𐌀b";
    const char c15[] = "ab";
    const char c16[] = "шb";
    const char c17[] = "水b";
    const char c18[] = "𐌀b";
    EXPECT_TRUE(Check(Decode(c11), static_cast<const char8_t*>(c11) + 1, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c12), static_cast<const char8_t*>(c12) + 2, U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c13), static_cast<const char8_t*>(c13) + 3, U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c14), static_cast<const char8_t*>(c14) + 4, U'𐌀', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c15), static_cast<const char*>(c15) + 1, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c16), static_cast<const char*>(c16) + 2, U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c17), static_cast<const char*>(c17) + 3, U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c18), static_cast<const char*>(c18) + 4, U'𐌀', DecodeError::OK));

    std::array<char8_t,       1> a1{u8'a'};
    std::array<char,          1> a2{'a'};
    std::array<uint8_t,       1> a3{ch1};
    std::array<unsigned char, 1> a4{ch2};
    EXPECT_TRUE(Check(Decode(a1), a1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a2), a2.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a3), a3.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a4), a4.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a4.begin(), a4.end()), a4.end(), U'a', DecodeError::OK));

    std::vector<char8_t>       v1{a1.begin(), a1.end()};
    std::vector<char>          v2{a2.begin(), a2.end()};
    std::vector<uint8_t>       v3{a3.begin(), a3.end()};
    std::vector<unsigned char> v4{a4.begin(), a4.end()};
    EXPECT_TRUE(Check(Decode(v1), v1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2), v2.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v3), v3.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v4), v4.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v4.begin(), v4.end()), v4.end(), U'a', DecodeError::OK));
    v1.push_back(u8'b');
    v2.push_back('b');
    v3.push_back(static_cast<uint8_t>(u8'b'));
    v4.push_back(static_cast<unsigned char>('b'));
    EXPECT_TRUE(Check(Decode(v1), std::ranges::next(v1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2), std::ranges::next(v2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v3), std::ranges::next(v3.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v4), std::ranges::next(v4.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v4.begin(), v4.end()), std::ranges::next(v4.begin()), U'a', DecodeError::OK));

    std::span s1{v1};
    std::span s2{v2};
    std::span s3{v3};
    std::span s4{v4};
    EXPECT_TRUE(Check(Decode(s1), std::ranges::next(s1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s2), std::ranges::next(s2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s3), std::ranges::next(s3.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s4), std::ranges::next(s4.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s4.begin(), s4.end()), std::ranges::next(s4.begin()), U'a', DecodeError::OK));

    std::u8string str1{u8"ab"};
    std::string str2{"ab"};
    EXPECT_TRUE(Check(Decode(str1), std::ranges::next(str1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(str2), std::ranges::next(str2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(str2.begin(), str2.end()), std::ranges::next(str2.begin()), U'a', DecodeError::OK));

    std::u8string_view sv1{u8"ab"};
    std::string_view   sv2{"ab"};
    std::u8string_view sv3{u8"шb"};
    std::string_view   sv4{  "шb"};
    std::u8string_view sv5{u8"水b"};
    std::string_view   sv6{ "水b"};
    std::u8string_view sv7{u8"𐌀b"};
    std::string_view   sv8{  "𐌀b"};
    EXPECT_TRUE(Check(Decode(sv1), std::ranges::next(sv1.begin(), 1, sv1.end()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv2), std::ranges::next(sv2.begin(), 1, sv2.end()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv3), std::ranges::next(sv3.begin(), 2, sv3.end()), U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv4), std::ranges::next(sv4.begin(), 2, sv4.end()), U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv5), std::ranges::next(sv5.begin(), 3, sv5.end()), U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv6), std::ranges::next(sv6.begin(), 3, sv6.end()), U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv7), std::ranges::next(sv7.begin(), 4, sv7.end()), U'𐌀', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv8), std::ranges::next(sv8.begin(), 4, sv8.end()), U'𐌀', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv8.begin(), sv8.end()), std::ranges::next(sv8.begin(), 4, sv8.end()), U'𐌀', DecodeError::OK));

    auto result1 = Decode(str1.begin(), str1.end());
    auto result2 = Decode(result1.next, str1.end());
    EXPECT_TRUE(Check(result1, std::ranges::next(str1.begin(), 1, str1.end()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(result2, std::ranges::next(str1.begin(), 2, str1.end()), U'b', DecodeError::OK));
    EXPECT_TRUE(Check(result2, str1.end(), U'b', DecodeError::OK));

    /***
     * Test failure scenarios
     */
    const char c9[] = "";
    EXPECT_TRUE(Check(Decode(c9), static_cast<const char*>(c9), jcu::CODE_POINT_INVALID, DecodeError::SEQUENCE_EMPTY));
    const char8_t c10[] = u8"";
    EXPECT_TRUE(Check(Decode(c10), static_cast<const char8_t*>(c10), jcu::CODE_POINT_INVALID, DecodeError::SEQUENCE_EMPTY));
    std::u8string_view empty{};
    EXPECT_TRUE(Check(Decode(empty), empty.end(), jcu::CODE_POINT_INVALID, DecodeError::SEQUENCE_EMPTY));
    std::vector<char8_t> t1{static_cast<char8_t>(0xff)};
    EXPECT_TRUE(Check(Decode(t1), t1.end(), jcu::CODE_POINT_INVALID, DecodeError::INVALID_LEAD));
    std::vector<char8_t> t2{0xc0};
    EXPECT_TRUE(Check(Decode(t2), t2.end(), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char8_t> t3{0xc0, 0x3a};
    EXPECT_TRUE(Check(Decode(t3), std::ranges::next(t3.begin(), 1, t3.end()), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char8_t> t4{0xe0};
    EXPECT_TRUE(Check(Decode(t4), t4.end(), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char8_t> t5{0xe0, 0x8f};
    EXPECT_TRUE(Check(Decode(t5), std::ranges::next(t5.begin(), 1, t5.end()), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char8_t> t6{0xe0, 0x8f, 0x3f};
    EXPECT_TRUE(Check(Decode(t6), std::ranges::next(t6.begin(), 1, t6.end()), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char8_t> t7{0xf0};
    EXPECT_TRUE(Check(Decode(t7), t7.end(), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char8_t> t8{0xf0, 0x8f};
    EXPECT_TRUE(Check(Decode(t8), std::ranges::next(t8.begin(), 1, t8.end()), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char8_t> t9{0xf0, 0x8f, 0x8f};
    EXPECT_TRUE(Check(Decode(t9), std::ranges::next(t9.begin(), 1, t9.end()), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char8_t> t10{0xf0, 0x8f, 0x8f, 0x3f};
    EXPECT_TRUE(Check(Decode(t10), std::ranges::next(t10.begin(), 1, t10.end()), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    // Not sure how to achieve DecodeError::OVERLONG_SEQUENCE
}


TEST(DecodeEncodeTests, test_Decode16) {
    using namespace jcu::utf;

    uint16_t ch1 = static_cast<uint16_t>(u'a');
    EXPECT_TRUE(Check(Decode(u'a'), nullptr, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode( ch1), nullptr, U'a', DecodeError::OK));

    char16_t c1[] = u"a";
    char16_t c2[] = u"ш";
    char16_t c3[] = u"水";
    char16_t c4[] = u"𐌀";
    EXPECT_TRUE(Check(Decode(c1), static_cast<const char16_t*>(c1) + 1, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c2), static_cast<const char16_t*>(c2) + 1, U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c3), static_cast<const char16_t*>(c3) + 1, U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c4), static_cast<const char16_t*>(c4) + 2, U'𐌀', DecodeError::OK));

    char16_t c5[] = u"ab";
    char16_t c6[] = u"шb";
    char16_t c7[] = u"水b";
    char16_t c8[] = u"𐌀b";
    EXPECT_TRUE(Check(Decode(c5), static_cast<const char16_t*>(c5) + 1, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c6), static_cast<const char16_t*>(c6) + 1, U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c7), static_cast<const char16_t*>(c7) + 1, U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c8), static_cast<const char16_t*>(c8) + 2, U'𐌀', DecodeError::OK));

    std::array<char16_t, 1> a1{u'a'};
    std::array<uint16_t, 1> a2{ch1};
    EXPECT_TRUE(Check(Decode(a1), a1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a2), a2.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a2.begin(), a2.end()), a2.end(), U'a', DecodeError::OK));

    std::vector<char16_t> v1{a1.begin(), a1.end()};
    std::vector<uint16_t> v2{a2.begin(), a2.end()};
    EXPECT_TRUE(Check(Decode(v1), v1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2), v2.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2.begin(), v2.end()), v2.end(), U'a', DecodeError::OK));
    v1.push_back(u'b');
    v2.push_back(static_cast<uint16_t>(u'b'));
    EXPECT_TRUE(Check(Decode(v1), std::ranges::next(v1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2), std::ranges::next(v2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2.begin(), v2.end()), std::ranges::next(v2.begin()), U'a', DecodeError::OK));

    std::span s1{v1};
    std::span s2{v2};
    EXPECT_TRUE(Check(Decode(s1), std::ranges::next(s1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s2), std::ranges::next(s2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s2.begin(), s2.end()), std::ranges::next(s2.begin()), U'a', DecodeError::OK));

    std::u16string str1{u"ab"};
    EXPECT_TRUE(Check(Decode(str1), std::ranges::next(str1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(str1.begin(), str1.end()), std::ranges::next(str1.begin()), U'a', DecodeError::OK));

    std::u16string_view sv1{u"ab"};
    std::u16string_view sv2{u"шb"};
    std::u16string_view sv3{u"水b"};
    std::u16string_view sv4{u"𐌀b"};
    EXPECT_TRUE(Check(Decode(sv1), std::ranges::next(sv1.begin(), 1, sv1.end()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv2), std::ranges::next(sv2.begin(), 1, sv2.end()), U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv3), std::ranges::next(sv3.begin(), 1, sv3.end()), U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv4), std::ranges::next(sv4.begin(), 2, sv4.end()), U'𐌀', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv4.begin(), sv4.end()), std::ranges::next(sv4.begin(), 2, sv4.end()), U'𐌀', DecodeError::OK));

    auto result1 = Decode(str1.begin(), str1.end());
    auto result2 = Decode(result1.next, str1.end());
    EXPECT_TRUE(Check(result1, std::ranges::next(str1.begin(), 1, str1.end()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(result2, std::ranges::next(str1.begin(), 2, str1.end()), U'b', DecodeError::OK));
    EXPECT_TRUE(Check(result2, str1.end(), U'b', DecodeError::OK));

    /***
     * Test failure scenarios
     */
    const char16_t c9[] = u"";
    EXPECT_TRUE(Check(Decode(c9), static_cast<const char16_t*>(c9), jcu::CODE_POINT_INVALID, DecodeError::SEQUENCE_EMPTY));
    std::u16string_view empty{};
    EXPECT_TRUE(Check(Decode(empty), empty.end(), jcu::CODE_POINT_INVALID, DecodeError::SEQUENCE_EMPTY));
    std::vector<char16_t> t1{static_cast<char16_t>(LEAD_SURROGATE_MIN)};
    EXPECT_TRUE(Check(Decode(t1), t1.end(), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<char16_t> t2{static_cast<char16_t>(TRAIL_SURROGATE_MIN), static_cast<char16_t>(TRAIL_SURROGATE_MAX)};
    EXPECT_TRUE(Check(Decode(t2), std::ranges::next(t2.begin(), 1, t2.end()), jcu::CODE_POINT_INVALID, DecodeError::INVALID_LEAD));
    std::vector<char16_t> t3{static_cast<char16_t>(LEAD_SURROGATE_MIN), static_cast<char16_t>(LEAD_SURROGATE_MAX)};
    EXPECT_TRUE(Check(Decode(t3), std::ranges::next(t3.begin(), 1, t3.end()), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
}


TEST(DecodeEncodeTests, test_Decode32) {
    using namespace jcu::utf;

    uint32_t ch1 = static_cast<uint32_t>(U'a');
    EXPECT_TRUE(Check(Decode(U'a'), nullptr, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode( ch1), nullptr, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(L'a'), nullptr, U'a', DecodeError::OK));

    // the number of consumed characters for wchar_t is platform specific; so not testing.
    char32_t c1[] = U"a";
    char32_t c2[] = U"ш";
    char32_t c3[] = U"水";
    char32_t c4[] = U"𐌀";
    EXPECT_TRUE(Check(Decode(c1), static_cast<const char32_t*>(c1) + 1, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c2), static_cast<const char32_t*>(c2) + 1, U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c3), static_cast<const char32_t*>(c3) + 1, U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c4), static_cast<const char32_t*>(c4) + 1, U'𐌀', DecodeError::OK));
 
    // the number of consumed characters for wchar_t is platform specific; so not testing.
    char32_t c5[] = U"a";
    char32_t c6[] = U"ш";
    char32_t c7[] = U"水";
    char32_t c8[] = U"𐌀";
    EXPECT_TRUE(Check(Decode(c5), static_cast<const char32_t*>(c5) + 1, U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c6), static_cast<const char32_t*>(c6) + 1, U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c7), static_cast<const char32_t*>(c7) + 1, U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(c8), static_cast<const char32_t*>(c8) + 1, U'𐌀', DecodeError::OK));

    std::array<char32_t, 1> a1{U'a'};
    std::array<uint32_t, 1> a2{ch1};
    auto a3 = std::span{L"a"}.subspan(0, 1);
    auto a4 = std::span{L"ш"}.subspan(0, 1);
    auto a5 = std::span{L"水"}.subspan(0, 1);
    auto a6 = std::span{L"𐌀"}.subspan(0, 2);
    EXPECT_TRUE(Check(Decode(a1), a1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a2), a2.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a3), a3.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a4), a4.end(), U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a5), a5.end(), U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a6), a6.end(), U'𐌀', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a1.begin(), a1.end()), a1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a2.begin(), a2.end()), a2.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(a6.begin(), a6.end()), a6.end(), U'𐌀', DecodeError::OK));

    std::vector<char32_t> v1{a1.begin(), a1.end()};
    std::vector<uint32_t> v2{a2.begin(), a2.end()};
    std::vector<wchar_t>  v3{a3.begin(), a3.end()};
    EXPECT_TRUE(Check(Decode(v1), v1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2), v2.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v3), v3.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v1.begin(), v1.end()), v1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2.begin(), v2.end()), v2.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v3.begin(), v3.end()), v3.end(), U'a', DecodeError::OK));
    v1.push_back(U'b');
    v2.push_back(static_cast<uint32_t>(U'b'));
    v3.push_back(L'b');
    EXPECT_TRUE(Check(Decode(v1), std::ranges::next(v1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2), std::ranges::next(v2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v3), std::ranges::next(v3.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v1.begin(), v1.end()), std::ranges::next(v1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v2.begin(), v2.end()), std::ranges::next(v2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(v3.begin(), v3.end()), std::ranges::next(v3.begin()), U'a', DecodeError::OK));

    std::span s1{v1};
    std::span s2{v2};
    std::span s3{v3};
    EXPECT_TRUE(Check(Decode(s1), std::ranges::next(s1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s2), std::ranges::next(s2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s3), std::ranges::next(s3.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s1.begin(), s1.end()), std::ranges::next(s1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s2.begin(), s2.end()), std::ranges::next(s2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(s3.begin(), s3.end()), std::ranges::next(s3.begin()), U'a', DecodeError::OK));

    std::u32string str1{U"ab"};
    std::wstring str2{L"ab"};
    EXPECT_TRUE(Check(Decode(str1), std::ranges::next(str1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(str2), std::ranges::next(str2.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(str1.begin(), str1.end()), std::ranges::next(str1.begin()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(str2.begin(), str2.end()), std::ranges::next(str2.begin()), U'a', DecodeError::OK));

    std::u32string_view sv1{U"a"};
    std::u32string_view sv2{U"ш"};
    std::u32string_view sv3{U"水"};
    std::u32string_view sv4{U"𐌀"};
    std::wstring_view sv5{L"a"};
    std::wstring_view sv6{L"ш"};
    std::wstring_view sv7{L"水"};
    EXPECT_TRUE(Check(Decode(sv1), sv1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv2), sv2.end(), U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv3), sv3.end(), U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv4), sv4.end(), U'𐌀', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv5), sv5.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv6), sv6.end(), U'ш', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv7), sv7.end(), U'水', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv1.begin(), sv1.end()), sv1.end(), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(Decode(sv7.begin(), sv7.end()), sv7.end(), U'水', DecodeError::OK));

    auto result1 = Decode(str1.begin(), str1.end());
    auto result2 = Decode(result1.next, str1.end());
    EXPECT_TRUE(Check(result1, std::ranges::next(str1.begin(), 1, str1.end()), U'a', DecodeError::OK));
    EXPECT_TRUE(Check(result2, std::ranges::next(str1.begin(), 2, str1.end()), U'b', DecodeError::OK));
    EXPECT_TRUE(Check(result2, str1.end(), U'b', DecodeError::OK));

    std::vector<char32_t> t1{static_cast<char32_t>(0x1010ffff)};
    EXPECT_TRUE(Check(Decode(t1), t1.end(), U'\x1010ffff', DecodeError::OK));
    std::vector<char32_t> t2{static_cast<char32_t>(0x1010ffff), static_cast<char32_t>(0x41)};
    auto result3 = Decode(t2);
    auto result4 = Decode(result3.next, t2.end());
    EXPECT_TRUE(Check(result3, std::ranges::next(t2.begin(), 1, t2.end()), U'\x1010ffff', DecodeError::OK));
    EXPECT_TRUE(Check(result4, t2.end(), U'\x41', DecodeError::OK));

    /***
     * Test failure scenarios
     */
    const char32_t c9[] = U"";
    EXPECT_TRUE(Check(Decode(c9), static_cast<const char32_t*>(c9), jcu::CODE_POINT_INVALID, DecodeError::SEQUENCE_EMPTY));
    const wchar_t c10[] = L"";
    EXPECT_TRUE(Check(Decode(c10), static_cast<const wchar_t*>(c10), jcu::CODE_POINT_INVALID, DecodeError::SEQUENCE_EMPTY));
    std::u32string_view empty{};
    EXPECT_TRUE(Check(Decode(empty), empty.end(), jcu::CODE_POINT_INVALID, DecodeError::SEQUENCE_EMPTY));

    std::vector<wchar_t> t4{static_cast<wchar_t>(LEAD_SURROGATE_MIN)};
    EXPECT_TRUE(Check(Decode(t4), t4.end(), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
    std::vector<wchar_t> t5{static_cast<wchar_t>(TRAIL_SURROGATE_MIN), static_cast<wchar_t>(TRAIL_SURROGATE_MAX)};
    EXPECT_TRUE(Check(Decode(t5), std::ranges::next(t5.begin(), 1, t5.end()), jcu::CODE_POINT_INVALID, DecodeError::INVALID_LEAD));
    std::vector<wchar_t> t6{static_cast<wchar_t>(LEAD_SURROGATE_MIN), static_cast<wchar_t>(LEAD_SURROGATE_MAX)};
    EXPECT_TRUE(Check(Decode(t6), std::ranges::next(t6.begin(), 1, t6.end()), jcu::CODE_POINT_INVALID, DecodeError::INCOMPLETE_SEQUENCE));
}
