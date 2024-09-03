// Copyright © 2024 Nemanja Trifunovic

#include <array>
#include <cstdint>

#include "jcu/utf/core.hpp"
#include "jcu/utf/decode_encode.hpp"
#include "ftest.h"


TEST(CoreTests, test_DecodeUTF8) {
    using namespace jcu::utf;

    EXPECT_EQ(DecodeUTF8(std::u8string_view{}),
              (DecodeData{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}));

    uint8_t ch1 = static_cast<uint8_t>(u8'a');
    unsigned char ch2 = static_cast<unsigned char>('a');
    EXPECT_EQ(DecodeUTF8(u8'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  ch1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  ch2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF8(u8"a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  "a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(u8"ш"), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  "ш"), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(u8"水"), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  "水"), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(u8"𐌀"), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  "𐌀"), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF8(u8"ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  "ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(u8"шb"), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  "шb"), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(u8"水b"), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  "水b"), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(u8"𐌀b"), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(  "𐌀b"), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    std::array<char8_t,       1> a1{{u8'a'}};
    std::array<char,          1> a2{{'a'}};
    std::array<uint8_t,       1> a3{{ch1}};
    std::array<unsigned char, 1> a4{{ch2}};
    EXPECT_EQ(DecodeUTF8(a1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(a2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(a3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(a4), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    std::vector<char8_t>       v1{a1.begin(), a1.end()};
    std::vector<char>          v2{a2.begin(), a2.end()};
    std::vector<uint8_t>       v3{a3.begin(), a3.end()};
    std::vector<unsigned char> v4{a4.begin(), a4.end()};
    EXPECT_EQ(DecodeUTF8(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(v3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(v4), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    v1.push_back(u8'b');
    v2.push_back('b');
    v3.push_back(static_cast<uint8_t>(u8'b'));
    v4.push_back(static_cast<unsigned char>('b'));
    EXPECT_EQ(DecodeUTF8(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(v3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(v4), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF8(std::span{v1}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::span{v2}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::span{v3}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::span{v4}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF8(std::u8string{u8"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::string{"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF8(std::u8string_view{u8"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::string_view  {"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::u8string_view{u8"ш"}), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::string_view  {  "ш"}), (DecodeData{.consumed=2, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::u8string_view{u8"水"}), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::string_view  { "水"}), (DecodeData{.consumed=3, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::u8string_view{u8"𐌀"}), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::string_view  {  "𐌀"}), (DecodeData{.consumed=4, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    /***
     * Test failure scenarios
     */
    // Invalid characters should return REPLACEMENT_CHARACTER
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{}),
             (DecodeData{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{static_cast<char8_t>(0xff)}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INVALID_LEAD}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xc0}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xc0, 0x3a}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xe0}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xe0, 0x8f}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xe0, 0x8f, 0x3f}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xf0}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xf0, 0x8f}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xf0, 0x8f, 0x8f}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF8(std::vector<char8_t>{{0xf0, 0x8f, 0x8f, 0x3f}}),
             (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    // Not sure how to achieve DecodeError::OVERLONG_SEQUENCE
}


TEST(CoreTests, test_DecodeUTF16) {
    using namespace jcu::utf;

    EXPECT_EQ(DecodeUTF16(std::u16string_view{}),
              (DecodeData{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}));

    uint16_t ch1 = static_cast<uint16_t>(u'a');
    EXPECT_EQ(DecodeUTF16(u'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16( ch1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF16(u"a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(u"ш"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(u"水"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(u"𐌀"), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF16(u"ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(u"шb"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(u"水b"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(u"𐌀b"), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    std::array<char16_t, 1> a1{{u'a'}};
    std::array<uint16_t, 1> a2{{ch1}};
    EXPECT_EQ(DecodeUTF16(a1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(a2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    std::vector<char16_t> v1{a1.begin(), a1.end()};
    std::vector<uint16_t> v2{a2.begin(), a2.end()};
    EXPECT_EQ(DecodeUTF16(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    v1.push_back(u'b');
    v2.push_back(static_cast<uint16_t>(u'b'));
    EXPECT_EQ(DecodeUTF16(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF16(std::span{v1}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(std::span{v2}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF16(std::u16string{u"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF16(std::u16string_view{u"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(std::u16string_view{u"ш"}), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(std::u16string_view{u"水"}), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(std::u16string_view{u"𐌀"}), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    /***
     * Test failure scenarios
     */
    EXPECT_EQ(DecodeUTF16(std::vector<char16_t>{}),
              (DecodeData{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF16(std::vector<char16_t>{{static_cast<char16_t>(LEAD_SURROGATE_MIN)}}),
              (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
    EXPECT_EQ(DecodeUTF16(std::vector<char16_t>{{static_cast<char16_t>(TRAIL_SURROGATE_MIN),
                                                 static_cast<char16_t>(TRAIL_SURROGATE_MAX)}}),
              (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INVALID_LEAD}));
    EXPECT_EQ(DecodeUTF16(std::vector<char16_t>{{static_cast<char16_t>(LEAD_SURROGATE_MIN),
                                                 static_cast<char16_t>(LEAD_SURROGATE_MAX)}}),
              (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INCOMPLETE_SEQUENCE}));
}


TEST(CoreTests, test_DecodeUTF32) {
    using namespace jcu::utf;

    EXPECT_EQ(DecodeUTF32(std::u32string_view{}),
              (DecodeData{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}));

    uint32_t ch1 = static_cast<uint32_t>(U'a');
    EXPECT_EQ(DecodeUTF32(U'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32( ch1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(L'a'), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF32(U"a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(L"a"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(U"ш"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(L"ш"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(U"水"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(L"水"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(U"𐌀"), (DecodeData{.consumed=1, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    auto result1 = DecodeUTF32(L"𐌀");
    EXPECT_EQ(result1.code_point, U'𐌀');
    // the number of consumed characters is platform specific; so not testing.
    EXPECT_EQ(result1.error_code, DecodeError::OK);
    //EXPECT_EQ(DecodeUTF32(L"𐌀"), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF32(U"ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(L"ab"), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(U"шb"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(L"шb"), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(U"水b"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(L"水b"), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(U"𐌀b"), (DecodeData{.consumed=1, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    auto result2 = DecodeUTF32(L"𐌀b");
    EXPECT_EQ(result2.code_point, U'𐌀');
    // the number of consumed characters is platform specific; so not testing.
    EXPECT_EQ(result2.error_code, DecodeError::OK);
    //EXPECT_EQ(DecodeUTF32(L"𐌀b"), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    std::array<char32_t, 1> a1{{U'a'}};
    std::array<uint32_t, 1> a2{{ch1}};
    std::array<wchar_t,  1> a3{{L'a'}};
    EXPECT_EQ(DecodeUTF32(a1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(a2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(a3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    std::vector<char32_t> v1{a1.begin(), a1.end()};
    std::vector<uint32_t> v2{a2.begin(), a2.end()};
    std::vector<wchar_t>  v3{a3.begin(), a3.end()};
    EXPECT_EQ(DecodeUTF32(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(v3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    v1.push_back(U'b');
    v2.push_back(static_cast<uint32_t>(U'b'));
    v3.push_back(L'b');
    EXPECT_EQ(DecodeUTF32(v1), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(v2), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(v3), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF32(std::span{v1}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::span{v2}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::span{v3}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF32(std::u32string{U"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::wstring{L"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));

    EXPECT_EQ(DecodeUTF32(std::u32string_view{U"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::u32string_view{U"ш"}), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::u32string_view{U"水"}), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::u32string_view{U"𐌀"}), (DecodeData{.consumed=1, .code_point=U'𐌀', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::wstring_view{L"ab"}), (DecodeData{.consumed=1, .code_point=U'a', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::wstring_view{L"ш"}), (DecodeData{.consumed=1, .code_point=U'ш', .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::wstring_view{L"水"}), (DecodeData{.consumed=1, .code_point=U'水', .error_code=DecodeError::OK}));
    auto result3 = DecodeUTF32(std::wstring_view{L"𐌀"});
    EXPECT_EQ(result3.code_point, U'𐌀');
    // the number of consumed characters is platform specific; so not testing.
    EXPECT_EQ(result3.error_code, DecodeError::OK);
    //EXPECT_EQ(DecodeUTF32(std::wstring_view{L"𐌀"}), (DecodeData{.consumed=2, .code_point=U'𐌀', .error_code=DecodeError::OK}));

    /***
     * Test failure scenarios
     */
    EXPECT_EQ(DecodeUTF32(std::vector<char32_t>{}),
              (DecodeData{.consumed=0, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::OK}));
    EXPECT_EQ(DecodeUTF32(std::vector<char32_t>{{static_cast<char32_t>(0x1010ffff)}}),
              (DecodeData{.consumed=1, .code_point=REPLACEMENT_CHARACTER, .error_code=DecodeError::INVALID_CODE_POINT}));
}


TEST(CoreTests, test_EncodeUTF8) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF8(U'a'), u8"a");
    EXPECT_EQ(EncodeUTF8(U'ш'), u8"ш");
    EXPECT_EQ(EncodeUTF8(U'水'), u8"水");
    EXPECT_EQ(EncodeUTF8(U'𐌀'), u8"𐌀");
    EXPECT_EQ(EncodeUTF8(static_cast<char32_t>(0x1010ffff)), EncodeUTF8(REPLACEMENT_CHARACTER));
}


TEST(CoreTests, test_EncodeUTF16) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF16(U'a'), u"a");
    EXPECT_EQ(EncodeUTF16(U'ш'), u"ш");
    EXPECT_EQ(EncodeUTF16(U'水'), u"水");
    EXPECT_EQ(EncodeUTF16(U'𐌀'), u"𐌀");
    EXPECT_EQ(EncodeUTF16(static_cast<char32_t>(0x1010ffff)), EncodeUTF16(REPLACEMENT_CHARACTER));
}


TEST(CoreTests, test_EncodeUTF32) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF32(U'a'), U"a");
    EXPECT_EQ(EncodeUTF32(U'ш'), U"ш");
    EXPECT_EQ(EncodeUTF32(U'水'), U"水");
    EXPECT_EQ(EncodeUTF32(U'𐌀'), U"𐌀");
    EXPECT_EQ(EncodeUTF32(static_cast<char32_t>(0x1010ffff)), std::u32string{{REPLACEMENT_CHARACTER}});
}
