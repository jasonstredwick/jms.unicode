// Copyright © 2024 Jason Stredwick

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

#include "jcu/utf/format.hpp"
#include "ftest.h"


TEST(FormatTests, test_DefaultFuncs) {
    using namespace jcu::utf;

    char8_t c8 = 0x41;
    char16_t c16 = 0x42;
    char32_t c32 = 0x0001F600;

    EXPECT_EQ(std::format("{}", c8), std::format("{}", static_cast<uint8_t>(c8)));
    EXPECT_EQ(std::format("{}", c16), std::format("{}", static_cast<uint16_t>(c16)));
    EXPECT_EQ(std::format("{}", c32), std::format("{}", static_cast<uint32_t>(c32)));
    EXPECT_EQ(std::format("{:#010x}", c32), std::format("{:#010x}", static_cast<uint32_t>(c32)));

    std::string str{"abcdxyzшницла水手𐌀😀"};
    char8_t ca8[] = u8"abcdxyzшницла水手𐌀😀";
    char16_t ca16[] = u"abcdxyzшницла水手𐌀😀";
    char32_t ca32[] = U"abcdxyzшницла水手𐌀😀";
    std::u8string s8{u8"abcdxyzшницла水手𐌀😀"};
    std::u16string s16{u"abcdxyzшницла水手𐌀😀"};
    std::u32string s32{U"abcdxyzшницла水手𐌀😀"};
    std::u8string_view sv8{s8};
    std::u16string_view sv16{s16};
    std::u32string_view sv32{s32};

    EXPECT_EQ(std::format("{}", u8"abcdxyzшницла水手𐌀😀"), std::format("{}", str));
    EXPECT_EQ(std::format("{}", ca8), std::format("{}", str));
    EXPECT_EQ(std::format("{}", s8), std::format("{}", str));
    EXPECT_EQ(std::format("{}", sv8), std::format("{}", str));

    EXPECT_EQ(std::format("{}", u"abcdxyzшницла水手𐌀😀"), std::format("{}", str));
    EXPECT_EQ(std::format("{}", ca16), std::format("{}", str));
    EXPECT_EQ(std::format("{}", s16), std::format("{}", str));
    EXPECT_EQ(std::format("{}", sv16), std::format("{}", str));

    EXPECT_EQ(std::format("{}", U"abcdxyzшницла水手𐌀😀"), std::format("{}", str));
    EXPECT_EQ(std::format("{}", ca32), std::format("{}", str));
    EXPECT_EQ(std::format("{}", s32), std::format("{}", str));
    EXPECT_EQ(std::format("{}", sv32), std::format("{}", str));
}
