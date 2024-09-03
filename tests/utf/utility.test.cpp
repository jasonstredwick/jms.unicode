// Copyright © 2024 Jason Stredwick

#include <string>
#include <string_view>
#include <vector>

#include "jcu/utf/utf.hpp"
#include "ftest.h"


TEST(UtilityTests, test_Utility_ConvertToUTF) {
    using namespace jcu::utf;
    std::string valid0{"abcdxyzшницла水手𐌀"};
    std::u8string valid1{u8"abcdxyzшницла水手𐌀"};
    std::u16string valid2{u"abcdxyzшницла水手𐌀"};
    std::u32string valid3{U"abcdxyzшницла水手𐌀"};

    EXPECT_EQ((ConvertToUTF<decltype(valid0), char8_t>(valid0)), valid1);
    EXPECT_EQ(ConvertToUTF8(valid1), valid1);
    EXPECT_EQ(ConvertToUTF8(valid2), valid1);
    EXPECT_EQ(ConvertToUTF8(valid3), valid1);
    EXPECT_EQ(ConvertToUTF16(valid1), valid2);
    EXPECT_EQ(ConvertToUTF16(valid2), valid2);
    EXPECT_EQ(ConvertToUTF16(valid3), valid2);
    EXPECT_EQ(ConvertToUTF32(valid1), valid3);
    EXPECT_EQ(ConvertToUTF32(valid2), valid3);
    EXPECT_EQ(ConvertToUTF32(valid3), valid3);

    /***
     * Test failures
     * NOTE: Need to include more codepoints that test u16 surrogate values.
     */
    // 8 -> 8, 16, 32
    {
        std::u8string invalid{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa}};
        std::u8string final{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xef, 0xbf, 0xbd}};
        EXPECT_EQ(ConvertToUTF8(invalid), final);
    }
    {
        std::u8string invalid{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa, 0xe6, 0x97, 0xa5}};
        std::u8string final{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xef, 0xbf, 0xbd, 0xe6, 0x97, 0xa5}};
        EXPECT_EQ(ConvertToUTF8(invalid), final);
    }
    {
        // \xfa is invalid (unexpected continuation byte)
        std::u8string invalid{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa}};
        std::u16string final{{0x65e5, 0x0448, 0xfffd}};
        EXPECT_EQ(ConvertToUTF16(invalid), final);
    }
    {
        std::u8string invalid{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa, 0xe6, 0x97, 0xa5}};
        std::u16string final{{0x65e5, 0x0448, 0xfffd, 0x65e5}};
        EXPECT_EQ(ConvertToUTF16(invalid), final);
    }
    {
        // \xfa is invalid (unexpected continuation byte)
        std::u8string invalid{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa}};
        std::u32string final{{0x000065e5, 0x00000448, 0x0000fffd}};
        EXPECT_EQ(ConvertToUTF32(invalid), final);
    }
    {
        std::u8string invalid{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa, 0xe6, 0x97, 0xa5}};
        std::u32string final{{0x000065e5, 0x00000448, 0x0000fffd, 0x000065e5}};
        EXPECT_EQ(ConvertToUTF32(invalid), final);
    }

    // 16 -> 8, 16, 32
    {
        // 0xdc07 is invalid (unexpected lead surrogate)
        std::u16string invalid{{0x65e5, 0x0448, 0xdc07}};
        std::u8string final{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xef, 0xbf, 0xbd}};
        EXPECT_EQ(ConvertToUTF8(invalid), final);
    }
    {
        std::u16string invalid{{0x65e5, 0x0448, 0xdc07, 0x65e5, 0x0448}};
        std::u8string final{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xef, 0xbf, 0xbd, 0xe6, 0x97, 0xa5, 0xd1, 0x88}};
        EXPECT_EQ(ConvertToUTF8(invalid), final);
    }
    {
        // 0xdc07 is invalid (unexpected lead surrogate)
        std::u16string invalid{{0x65e5, 0x0448, 0xdc07}};
        std::u16string final{{0x65e5, 0x0448, 0xfffd}};
        EXPECT_EQ(ConvertToUTF16(invalid), final);
    }
    {
        std::u16string invalid{{0x65e5, 0x0448, 0xdc07, 0x65e5, 0x0448}};
        std::u16string final{{0x65e5, 0x0448, 0xfffd, 0x65e5, 0x0448}};
        EXPECT_EQ(ConvertToUTF16(invalid), final);
    }
    {
        // 0xdc07 is invalid (unexpected lead surrogate)
        std::u16string invalid{{0x65e5, 0x0448, 0xdc07}};
        std::u32string final{{0x000065e5, 0x00000448, 0x0000fffd}};
        EXPECT_EQ(ConvertToUTF32(invalid), final);
    }
    {
        std::u16string invalid{{0x65e5, 0x0448, 0xdc07, 0x65e5, 0x0448}};
        std::u32string final{{0x000065e5, 0x00000448, 0x0000fffd, 0x000065e5, 0x00000448}};
        EXPECT_EQ(ConvertToUTF32(invalid), final);
    }

    // 32 -> 8, 16, 32
    {
        // 0x0011ffff is invalid; beyond last possible code point
        std::u32string invalid{{0x000065e5, 0x00000448, 0x0011ffff}};
        std::u8string final{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xef, 0xbf, 0xbd}};
        EXPECT_EQ(ConvertToUTF8(invalid), final);
    }
    {
        std::u32string invalid{{0x000065e5, 0x00000448, 0x0011ffff, 0x000065e5, 0x00000448}};
        std::u8string final{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xef, 0xbf, 0xbd, 0xe6, 0x97, 0xa5, 0xd1, 0x88}};
        EXPECT_EQ(ConvertToUTF8(invalid), final);
    }
    {
        // 0x0011ffff is invalid; beyond last possible code point
        std::u32string invalid{{0x000065e5, 0x00000448, 0x0011ffff}};
        std::u16string final{{0x65e5, 0x0448, 0xfffd}};
        EXPECT_EQ(ConvertToUTF16(invalid), final);
    }
    {
        std::u32string invalid{{0x000065e5, 0x00000448, 0x0011ffff, 0x000065e5, 0x00000448}};
        std::u16string final{{0x65e5, 0x0448, 0xfffd, 0x65e5, 0x0448}};
        EXPECT_EQ(ConvertToUTF16(invalid), final);
    }
    {
        // 0x0011ffff is invalid; beyond last possible code point
        std::u32string invalid{{0x000065e5, 0x00000448, 0x0011ffff}};
        std::u32string final{{0x000065e5, 0x00000448, 0x0000fffd}};
        EXPECT_EQ(ConvertToUTF32(invalid), final);
    }
    {
        std::u32string invalid{{0x000065e5, 0x00000448, 0x0011ffff, 0x000065e5, 0x00000448}};
        std::u32string final{{0x000065e5, 0x00000448, 0x0000fffd, 0x000065e5, 0x00000448}};
        EXPECT_EQ(ConvertToUTF32(invalid), final);
    }
}


TEST(UtilityTests, test_Utility_AttemptConvertToUTF) {
    using namespace jcu::utf;
    std::string valid0{"abcdxyzшницла水手𐌀"};
    std::u8string valid1{u8"abcdxyzшницла水手𐌀"};
    std::u16string valid2{u"abcdxyzшницла水手𐌀"};
    std::u32string valid3{U"abcdxyzшницла水手𐌀"};

    std::u8string out8{};
    EXPECT_EQ((AttemptConvertToUTF(valid0, out8)).error_code, DecodeError::OK);
    EXPECT_EQ(out8, valid1);
    out8.clear();
    EXPECT_EQ((AttemptConvertToUTF(valid1, out8)).error_code, DecodeError::OK);
    EXPECT_EQ(out8, valid1);
    out8.clear();
    EXPECT_EQ((AttemptConvertToUTF(valid2, out8)).error_code, DecodeError::OK);
    EXPECT_EQ(out8, valid1);
    out8.clear();
    EXPECT_EQ((AttemptConvertToUTF(valid3, out8)).error_code, DecodeError::OK);
    EXPECT_EQ(out8, valid1);

    std::u16string out16{};
    EXPECT_EQ((AttemptConvertToUTF(valid1, out16)).error_code, DecodeError::OK);
    EXPECT_EQ(out16, valid2);
    out16.clear();
    EXPECT_EQ((AttemptConvertToUTF(valid2, out16)).error_code, DecodeError::OK);
    EXPECT_EQ(out16, valid2);
    out16.clear();
    EXPECT_EQ((AttemptConvertToUTF(valid3, out16)).error_code, DecodeError::OK);
    EXPECT_EQ(out16, valid2);

    std::u32string out32{};
    EXPECT_EQ((AttemptConvertToUTF(valid1, out32)).error_code, DecodeError::OK);
    EXPECT_EQ(out32, valid3);
    out32.clear();
    EXPECT_EQ((AttemptConvertToUTF(valid2, out32)).error_code, DecodeError::OK);
    EXPECT_EQ(out32, valid3);
    out32.clear();
    EXPECT_EQ((AttemptConvertToUTF(valid3, out32)).error_code, DecodeError::OK);
    EXPECT_EQ(out32, valid3);

    // \xfa is invalid (unexpected continuation byte)
    std::u8string invalid1{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa}};
    std::u8string invalid2{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa, 0xe6, 0x97, 0xa5}};

    // Test failure
    out8.clear();
    auto result = AttemptConvertToUTF(invalid1, out8);
    EXPECT_NE(result.error_code, DecodeError::OK);
    EXPECT_EQ(result.index, 5);

    std::u8string invalid1_test{{0xe6, 0x97, 0xa5, 0xd1, 0x88}};
    out8.clear();
    result = AttemptConvertToUTF(invalid2, out8);
    EXPECT_NE(result.error_code, DecodeError::OK);
    EXPECT_EQ(result.index, 5);
    EXPECT_EQ(out8, invalid1_test);

    // Test failure and continuation, removing bad code point rather than replacement.  Use ConvertToUTF if
    // simple replacement is desired.
    std::u8string invalid2_test1{{0xe6, 0x97, 0xa5, 0xd1, 0x88}};
    std::u8string invalid2_test2{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xe6, 0x97, 0xa5}};
    out8.clear();
    result = AttemptConvertToUTF(invalid2, out8);
    EXPECT_NE(result.error_code, DecodeError::OK);
    EXPECT_EQ(result.index, 5);
    EXPECT_EQ(out8, invalid1_test);
    std::u8string_view invalid2a{invalid2.begin() + result.index + result.consumed, invalid2.end()};
    result = AttemptConvertToUTF(invalid2a, out8);
    EXPECT_EQ(result.error_code, DecodeError::OK);
    EXPECT_EQ(result.index, std::ssize(invalid2a));
    EXPECT_EQ(out8, invalid2_test2);
}


TEST(UtilityTests, test_Utility_IsValid) {
    using namespace jcu::utf;
    std::string valid0{"abcdxyzшницла水手𐌀"};
    std::u8string valid1{u8"abcdxyzшницла水手𐌀"};
    std::u16string valid2{u"abcdxyzшницла水手𐌀"};
    std::u32string valid3{U"abcdxyzшницла水手𐌀"};

    // \xfa is invalid (unexpected continuation byte)
    std::u8string invalid1{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa}};
    std::u8string invalid2{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa, 0xe6, 0x97, 0xa5}};
    // 0xdc07 is invalid (unexpected lead surrogate)
    std::u16string invalid3{{0x65e5, 0x0448, 0xdc07}};
    std::u16string invalid4{{0x65e5, 0x0448, 0xdc07, 0x65e5, 0x0448}};
    // 0x0011ffff is invalid; beyond last possible code point
    std::u32string invalid5{{0x000065e5, 0x00000448, 0x0011ffff}};
    std::u32string invalid6{{0x000065e5, 0x00000448, 0x0011ffff, 0x000065e5, 0x00000448}};

    EXPECT_TRUE(IsValid(valid0));
    EXPECT_TRUE(IsValid(valid1));
    EXPECT_TRUE(IsValid(valid2));
    EXPECT_TRUE(IsValid(valid3));
    EXPECT_FALSE(IsValid(invalid1));
    EXPECT_FALSE(IsValid(invalid2));
    EXPECT_FALSE(IsValid(invalid3));
    EXPECT_FALSE(IsValid(invalid4));
    EXPECT_FALSE(IsValid(invalid5));
    EXPECT_FALSE(IsValid(invalid6));
}


TEST(UtilityTests, test_Utility_FindFirstInvalid) {
    using namespace jcu::utf;
    std::string valid0{"abcdxyzшницла水手𐌀"};
    std::u8string valid1{u8"abcdxyzшницла水手𐌀"};
    std::u16string valid2{u"abcdxyzшницла水手𐌀"};
    std::u32string valid3{U"abcdxyzшницла水手𐌀"};

    // \xfa is invalid (unexpected continuation byte)
    std::u8string invalid1{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa}};
    std::u8string invalid2{{0xe6, 0x97, 0xa5, 0xd1, 0x88, 0xfa, 0xe6, 0x97, 0xa5}};
    // 0xdc07 is invalid (unexpected lead surrogate)
    std::u16string invalid3{{0x65e5, 0x0448, 0xdc07}};
    std::u16string invalid4{{0x65e5, 0x0448, 0xdc07, 0x65e5, 0x0448}};
    // 0x0011ffff is invalid; beyond last possible code point
    std::u32string invalid5{{0x000065e5, 0x00000448, 0x0011ffff}};
    std::u32string invalid6{{0x000065e5, 0x00000448, 0x0011ffff, 0x000065e5, 0x00000448}};

    EXPECT_EQ(FindFirstInvalid(valid0).index, std::ssize(valid0));
    EXPECT_EQ(FindFirstInvalid(valid1).index, std::ssize(valid1));
    EXPECT_EQ(FindFirstInvalid(valid2).index, std::ssize(valid2));
    EXPECT_EQ(FindFirstInvalid(valid3).index, std::ssize(valid3));

    EXPECT_EQ(FindFirstInvalid(invalid1).index, 5);
    EXPECT_EQ(FindFirstInvalid(invalid2).index, 5);
    EXPECT_EQ(FindFirstInvalid(invalid3).index, 2);
    EXPECT_EQ(FindFirstInvalid(invalid4).index, 2);
    EXPECT_EQ(FindFirstInvalid(invalid5).index, 2);
    EXPECT_EQ(FindFirstInvalid(invalid6).index, 2);
}
