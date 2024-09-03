// Copyright © 2024 Nemanja Trifunovic

#include "jcu/utf/core.hpp"
#include "ftest.h"


TEST(CoreTests, test_IsTrailUTF8) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsTrailUTF8('a'));
    EXPECT_FALSE(IsTrailUTF8('\x7F'));

    EXPECT_TRUE(IsTrailUTF8(static_cast<char8_t>('\x80')));
    EXPECT_TRUE(IsTrailUTF8(static_cast<char8_t>('\x99')));
}


TEST(CoreTests, test_IsLeadSurrogateUTF16) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsLeadSurrogateUTF16(u'\u0000'));
    EXPECT_FALSE(IsLeadSurrogateUTF16(u'z'));
    EXPECT_FALSE(IsLeadSurrogateUTF16(u'\ud7ff'));

    EXPECT_TRUE(IsLeadSurrogateUTF16(u'\xd800'));
    EXPECT_TRUE(IsLeadSurrogateUTF16(u'\xdab0'));
    EXPECT_TRUE(IsLeadSurrogateUTF16(u'\xdbff'));

    EXPECT_FALSE(IsLeadSurrogateUTF16(u'\xdc00'));
    EXPECT_FALSE(IsLeadSurrogateUTF16(u'\xdfff'));

    EXPECT_FALSE(IsLeadSurrogateUTF16(u'\uefff'));
}


TEST(CoreTests, test_IsTrailSurrogateUTF16) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsTrailSurrogateUTF16(u'\u0000'));
    EXPECT_FALSE(IsTrailSurrogateUTF16(u'z'));
    EXPECT_FALSE(IsTrailSurrogateUTF16(u'\ud7ff'));

    EXPECT_FALSE(IsTrailSurrogateUTF16(u'\xd800'));
    EXPECT_FALSE(IsTrailSurrogateUTF16(u'\xdbff'));

    EXPECT_TRUE(IsTrailSurrogateUTF16(u'\xdc00'));
    EXPECT_TRUE(IsTrailSurrogateUTF16(u'\xde02'));
    EXPECT_TRUE(IsTrailSurrogateUTF16(u'\xdfff'));
    EXPECT_FALSE(IsTrailSurrogateUTF16(u'\xe000'));

    EXPECT_FALSE(IsLeadSurrogateUTF16(u'\uefff'));
}


TEST(CoreTests, test_IsSurrogateUTF16) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsSurrogateUTF16(u'\u0000'));
    EXPECT_FALSE(IsSurrogateUTF16(U'z'));
    EXPECT_FALSE(IsSurrogateUTF16(u'\ud7ff'));

    EXPECT_TRUE(IsSurrogateUTF16(u'\xd800'));
    EXPECT_TRUE(IsSurrogateUTF16(u'\xdbff'));
    EXPECT_TRUE(IsSurrogateUTF16(u'\xdc00'));
    EXPECT_TRUE(IsSurrogateUTF16(u'\xde02'));
    EXPECT_TRUE(IsSurrogateUTF16(u'\xdfff'));
    EXPECT_FALSE(IsSurrogateUTF16(u'\xe000'));

    EXPECT_FALSE(IsSurrogateUTF16(u'\uefff'));
    EXPECT_FALSE(IsSurrogateUTF16(U'\U001012af'));
}


TEST(CoreTests, test_IsCodePointValid) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsCodePointValid(U'\xdbff'));
    EXPECT_FALSE(IsCodePointValid(U'\x11ffff'));

    EXPECT_TRUE(IsCodePointValid(U'\x80'));
    EXPECT_TRUE(IsCodePointValid(U'\x99'));
}


TEST(CoreTests, test_SequenceLength) {
    using namespace jcu::utf;
    EXPECT_EQ(SequenceLength(u8'Z'), 1);
    EXPECT_EQ(SequenceLength(static_cast<char8_t>(0x79)), 1);
    EXPECT_EQ(SequenceLength(static_cast<char8_t>(0xc2)), 2);
    EXPECT_EQ(SequenceLength(static_cast<char8_t>(0xe0)), 3);
    EXPECT_EQ(SequenceLength(static_cast<char8_t>(0xf0)), 4);
}
