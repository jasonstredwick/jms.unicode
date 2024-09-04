// Copyright © 2024 Jason Stredwick

#include <cstdint>
#include <format>
#include <print>

#include "jcu/utf/format.hpp"
#include "ftest.h"


TEST(ViewTests, test_DefaultFuncs) {
    using namespace jcu::utf;
    char8_t c8 = 0x41;
    char16_t c16 = 0x42;
    char32_t c32 = 0x0001F600;

    EXPECT_EQ(std::format("{}", c8), std::format("{}", static_cast<uint8_t>(c8)));
    EXPECT_EQ(std::format("{}", c16), std::format("{}", static_cast<uint16_t>(c16)));
    EXPECT_EQ(std::format("{}", c32), std::format("{}", static_cast<uint32_t>(c32)));

    std::string str{"hello😀"};
    std::u8string s8{u8"hello😀"};
    std::u16string s16{u"hello😀"};
    std::u32string s32{U"hello😀"};
    std::u8string_view sv8{s8};
    std::u16string_view sv16{s16};
    std::u32string_view sv32{s32};

    EXPECT_EQ(std::format("{}", s8), std::format("{}", str));
    EXPECT_EQ(std::format("{}", sv8), std::format("{}", str));
    EXPECT_EQ(std::format("{}", u8"hello😀"), std::format("{}", str));

    EXPECT_EQ(std::format("{}", s16), std::format("{}", str));
    EXPECT_EQ(std::format("{}", sv16), std::format("{}", str));
    EXPECT_EQ(std::format("{}", u"hello😀"), std::format("{}", str));

    EXPECT_EQ(std::format("{}", s32), std::format("{}", str));
    EXPECT_EQ(std::format("{}", sv32), std::format("{}", str));
    EXPECT_EQ(std::format("{}", U"hello😀"), std::format("{}", str));
}

#if 0
int main() {
    char8_t c8 = 0x41;
    char16_t c16 = 0x42;
    char32_t c32 = 0x0001F600;
    std::u8string s8{u8"hello😀"};
    std::u16string s16{u"hello😀"};
    std::u32string s32{U"hello😀"};
    std::u8string_view sv8{s8};

    std::println("Normal char strings");
    std::println("{}", "hello😀");
    std::println("{}", "\U0001F600");
    std::println("");

    std::println("char8_t");
    std::println("{}", c8);
    std::println("{:#x}", c8);
    std::println("{:05d}", c8);
    std::println("");

    std::println("char16_t");
    std::println("{}", c16);
    std::println("{:#x}", c16);
    std::println("{:05d}", c16);
    std::println("");

    std::println("char32_t");
    std::println("{}", c32);
    std::println("{:#x}", c32);
    std::println("{:#010x}", c32);
    std::println("{:d}", c32);
    std::println("{:08d}", c32);
    std::println("");

    std::println("utf8-strings: raw u8 string, std::u8string, and std::u8string_view");
    std::println("{}", u8"hello 😀 there");
    std::println("{}", s8);
    std::println("{}", sv8);
    std::println("");

    return 0;
}
#endif
