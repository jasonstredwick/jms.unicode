// Copyright © 2024 Nemanja Trifunovic

#include "jcu/utf/core.hpp"
#include "jcu/utf/encode.hpp"
#include "ftest.h"


TEST(EncodeTests, test_EncodeUTF8) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF8(U'a'), u8"a");
    EXPECT_EQ(EncodeUTF8(U'ш'), u8"ш");
    EXPECT_EQ(EncodeUTF8(U'水'), u8"水");
    EXPECT_EQ(EncodeUTF8(U'𐌀'), u8"𐌀");
    char32_t bad_ch = 0x1010ffff;
    EXPECT_EQ(EncodeUTF8(bad_ch, 0), EncodeUTF8(0));
}


TEST(EncodeTests, test_EncodeUTF16) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF16(U'a'), u"a");
    EXPECT_EQ(EncodeUTF16(U'ш'), u"ш");
    EXPECT_EQ(EncodeUTF16(U'水'), u"水");
    EXPECT_EQ(EncodeUTF16(U'𐌀'), u"𐌀");
    char32_t bad_ch = 0x1010ffff;
    EXPECT_EQ(EncodeUTF16(bad_ch, 0), EncodeUTF16(0));
}


TEST(EncodeTests, test_EncodeUTF32) {
    using namespace jcu::utf;
    EXPECT_EQ(EncodeUTF32(U'a'), U"a");
    EXPECT_EQ(EncodeUTF32(U'ш'), U"ш");
    EXPECT_EQ(EncodeUTF32(U'水'), U"水");
    EXPECT_EQ(EncodeUTF32(U'𐌀'), U"𐌀");
    char32_t bad_ch = 0x1010ffff;
    EXPECT_EQ(EncodeUTF32(bad_ch, 0), EncodeUTF32(0));
}


TEST(EncodeTests, test_Encode) {
    using namespace jcu::utf;
    EXPECT_EQ(Encode<char8_t>(U'a'), u8"a");
    EXPECT_EQ(Encode<char16_t>(U'a'), u"a");
    EXPECT_EQ(Encode<char32_t>(U'a'), U"a");

    EXPECT_EQ(Encode<char8_t>(U'𐌀'), u8"𐌀");
    EXPECT_EQ(Encode<char16_t>(U'𐌀'), u"𐌀");
    EXPECT_EQ(Encode<char32_t>(U'𐌀'), U"𐌀");

    char32_t bad_ch = 0x1010ffff;
    EXPECT_EQ(Encode<char8_t>(bad_ch, 0), EncodeUTF8(0));
    EXPECT_EQ(Encode<char16_t>(bad_ch, 0), EncodeUTF16(0));
    EXPECT_EQ(Encode<char32_t>(bad_ch, 0), EncodeUTF32(0));
}
