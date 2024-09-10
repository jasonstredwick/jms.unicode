// Copyright © 2024 Nemanja Trifunovic

#include <array>
#include <cstdint>
#include <vector>

#include "jcu/utf/core.hpp"
#include "jcu/utf/decode_encode.hpp"
#include "ftest.h"


TEST(DecodeEncodeTests, test_DecodeUTF8) {
    using namespace jcu::utf;

    EXPECT_EQ(DecodeUTF(std::u8string_view{}),
              (DecodeData{.consumed=0, .code_point=0, .error_code=DecodeError::OK}));

    uint8_t ch1 = static_cast<uint8_t>(u8'a');
    unsigned char ch2 = static_cast<unsigned char>('a');
    EXPECT_EQ(DecodeUTF(u8'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  ch1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  ch2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(u8"a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  "a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u8"ш"), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  "ш"), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u8"水"), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  "水"), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u8"𐌀"), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  "𐌀"), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(u8"ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  "ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u8"шb"), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  "шb"), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u8"水b"), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  "水b"), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u8"𐌀b"), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(  "𐌀b"), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    std::array<char8_t,       1> a1{{u8'a'}};
    std::array<char,          1> a2{{'a'}};
    std::array<uint8_t,       1> a3{{ch1}};
    std::array<unsigned char, 1> a4{{ch2}};
    EXPECT_EQ(DecodeUTF(a1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(a2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(a3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(a4), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    std::vector<char8_t>       v1{a1.begin(), a1.end()};
    std::vector<char>          v2{a2.begin(), a2.end()};
    std::vector<uint8_t>       v3{a3.begin(), a3.end()};
    std::vector<unsigned char> v4{a4.begin(), a4.end()};
    EXPECT_EQ(DecodeUTF(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v4), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    v1.push_back(u8'b');
    v2.push_back('b');
    v3.push_back(static_cast<uint8_t>(u8'b'));
    v4.push_back(static_cast<unsigned char>('b'));
    EXPECT_EQ(DecodeUTF(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v4), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::span{v1}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::span{v2}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::span{v3}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::span{v4}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::u8string{u8"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::string{"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::u8string_view{u8"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::string_view  {"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u8string_view{u8"ш"}), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::string_view  {  "ш"}), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u8string_view{u8"水"}), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::string_view  { "水"}), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u8string_view{u8"𐌀"}), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::string_view  {  "𐌀"}), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    /***
     * Test failure scenarios
     */
    // Invalid characters should return 0
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{}),
             (DecodeData{.consumed=0, .code_point=0, .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{static_cast<char8_t>(0xff)}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INVALID_LEAD}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xc0}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xc0, 0x3a}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xe0}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xe0, 0x8f}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xe0, 0x8f, 0x3f}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xf0}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xf0, 0x8f}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xf0, 0x8f, 0x8f}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char8_t>{{0xf0, 0x8f, 0x8f, 0x3f}}),
             (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    // Not sure how to achieve DecodeError::OVERLONG_SEQUENCE
}


TEST(DecodeEncodeTests, test_DecodeUTF16) {
    using namespace jcu::utf;

    EXPECT_EQ(DecodeUTF(std::u16string_view{}),
              (DecodeData{.consumed=0, .code_point=0, .error_code=DecodeError::OK}));

    uint16_t ch1 = static_cast<uint16_t>(u'a');
    EXPECT_EQ(DecodeUTF(u'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF( ch1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(u"a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u"ш"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u"水"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u"𐌀"), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(u"ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u"шb"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u"水b"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(u"𐌀b"), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    std::array<char16_t, 1> a1{{u'a'}};
    std::array<uint16_t, 1> a2{{ch1}};
    EXPECT_EQ(DecodeUTF(a1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(a2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    std::vector<char16_t> v1{a1.begin(), a1.end()};
    std::vector<uint16_t> v2{a2.begin(), a2.end()};
    EXPECT_EQ(DecodeUTF(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    v1.push_back(u'b');
    v2.push_back(static_cast<uint16_t>(u'b'));
    EXPECT_EQ(DecodeUTF(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::span{v1}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::span{v2}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::u16string{u"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::u16string_view{u"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u16string_view{u"ш"}), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u16string_view{u"水"}), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u16string_view{u"𐌀"}), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    /***
     * Test failure scenarios
     */
    EXPECT_EQ(DecodeUTF(std::vector<char16_t>{}),
              (DecodeData{.consumed=0, .code_point=0, .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::vector<char16_t>{{static_cast<char16_t>(LEAD_SURROGATE_MIN)}}),
              (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF(std::vector<char16_t>{{static_cast<char16_t>(TRAIL_SURROGATE_MIN),
                                                 static_cast<char16_t>(TRAIL_SURROGATE_MAX)}}),
              (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INVALID_LEAD}));
    EXPECT_EQ(DecodeUTF(std::vector<char16_t>{{static_cast<char16_t>(LEAD_SURROGATE_MIN),
                                                 static_cast<char16_t>(LEAD_SURROGATE_MAX)}}),
              (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
}


TEST(DecodeEncodeTests, test_DecodeUTF32) {
    using namespace jcu::utf;

    EXPECT_EQ(DecodeUTF(std::u32string_view{}),
              (DecodeData{.consumed=0, .code_point=0, .error_code=DecodeError::OK}));

    uint32_t ch1 = static_cast<uint32_t>(U'a');
    EXPECT_EQ(DecodeUTF(U'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF( ch1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(L'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(U"a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(L"a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(U"ш"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(L"ш"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(U"水"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(L"水"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(U"𐌀"), (DecodeData{.consumed=1, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    auto result1 = DecodeUTF(L"𐌀");
    EXPECT_EQ(result1.code_point, U'𐌀');
    // the number of consumed characters is platform specific; so not testing.
    EXPECT_EQ(result1.error_code, DecodeError::OK);
    //EXPECT_EQ(DecodeUTF(L"𐌀"), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(U"ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(L"ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(U"шb"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(L"шb"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(U"水b"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(L"水b"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(U"𐌀b"), (DecodeData{.consumed=1, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    auto result2 = DecodeUTF(L"𐌀b");
    EXPECT_EQ(result2.code_point, U'𐌀');
    // the number of consumed characters is platform specific; so not testing.
    EXPECT_EQ(result2.error_code, DecodeError::OK);
    //EXPECT_EQ(DecodeUTF(L"𐌀b"), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    std::array<char32_t, 1> a1{{U'a'}};
    std::array<uint32_t, 1> a2{{ch1}};
    std::array<wchar_t,  1> a3{{L'a'}};
    EXPECT_EQ(DecodeUTF(a1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(a2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(a3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    std::vector<char32_t> v1{a1.begin(), a1.end()};
    std::vector<uint32_t> v2{a2.begin(), a2.end()};
    std::vector<wchar_t>  v3{a3.begin(), a3.end()};
    EXPECT_EQ(DecodeUTF(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    v1.push_back(U'b');
    v2.push_back(static_cast<uint32_t>(U'b'));
    v3.push_back(L'b');
    EXPECT_EQ(DecodeUTF(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(v3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::span{v1}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::span{v2}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::span{v3}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::u32string{U"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::wstring{L"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF(std::u32string_view{U"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u32string_view{U"ш"}), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u32string_view{U"水"}), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::u32string_view{U"𐌀"}), (DecodeData{.consumed=1, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::wstring_view{L"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::wstring_view{L"ш"}), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::wstring_view{L"水"}), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    auto result3 = DecodeUTF(std::wstring_view{L"𐌀"});
    EXPECT_EQ(result3.code_point, U'𐌀');
    // the number of consumed characters is platform specific; so not testing.
    EXPECT_EQ(result3.error_code, DecodeError::OK);
    //EXPECT_EQ(DecodeUTF(std::wstring_view{L"𐌀"}), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    /***
     * Test failure scenarios
     */
    EXPECT_EQ(DecodeUTF(std::vector<char32_t>{}),
              (DecodeData{.consumed=0, .code_point=0, .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF(std::vector<char32_t>{{static_cast<char32_t>(0x1010ffff)}}),
              (DecodeData{.consumed=1, .code_point=0, .error_code=DecodeError::INVALID_CODE_POINT}));
}


TEST(DecodeEncodeTests, test_EncodeUTF8) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF8(U'a'), u8"a");
    EXPECT_EQ(EncodeUTF8(U'ш'), u8"ш");
    EXPECT_EQ(EncodeUTF8(U'水'), u8"水");
    EXPECT_EQ(EncodeUTF8(U'𐌀'), u8"𐌀");
    char32_t bad_ch = 0x1010ffff;
    EXPECT_EQ(EncodeUTF8(bad_ch, 0), EncodeUTF8(0));
}


TEST(DecodeEncodeTests, test_EncodeUTF16) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF16(U'a'), u"a");
    EXPECT_EQ(EncodeUTF16(U'ш'), u"ш");
    EXPECT_EQ(EncodeUTF16(U'水'), u"水");
    EXPECT_EQ(EncodeUTF16(U'𐌀'), u"𐌀");
    char32_t bad_ch = 0x1010ffff;
    EXPECT_EQ(EncodeUTF16(bad_ch, 0), EncodeUTF16(0));
}


TEST(DecodeEncodeTests, test_EncodeUTF32) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF32(U'a'), U"a");
    EXPECT_EQ(EncodeUTF32(U'ш'), U"ш");
    EXPECT_EQ(EncodeUTF32(U'水'), U"水");
    EXPECT_EQ(EncodeUTF32(U'𐌀'), U"𐌀");
    char32_t bad_ch = 0x1010ffff;
    EXPECT_EQ(EncodeUTF32(bad_ch, 0), EncodeUTF32(0));
}


TEST(DecodeEncodeTests, test_EncodeUTF) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF<char8_t>(U'a'), u8"a");
    EXPECT_EQ(EncodeUTF<char16_t>(U'a'), u"a");
    EXPECT_EQ(EncodeUTF<char32_t>(U'a'), U"a");

    EXPECT_EQ(EncodeUTF<char8_t>(U'𐌀'), u8"𐌀");
    EXPECT_EQ(EncodeUTF<char16_t>(U'𐌀'), u"𐌀");
    EXPECT_EQ(EncodeUTF<char32_t>(U'𐌀'), U"𐌀");

    char32_t bad_ch = 0x1010ffff;
    EXPECT_EQ(EncodeUTF<char8_t>(bad_ch, 0), EncodeUTF8(0));
    EXPECT_EQ(EncodeUTF<char16_t>(bad_ch, 0), EncodeUTF16(0));
    EXPECT_EQ(EncodeUTF<char32_t>(bad_ch, 0), EncodeUTF32(0));
}
