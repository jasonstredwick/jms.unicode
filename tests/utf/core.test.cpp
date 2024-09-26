// Copyright © 2024 Nemanja Trifunovic

#include "jcu/utf/core.hpp"
#include "ftest.h"


TEST(CoreTests, test_IsTrailUTF8) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsTrailUTF8(U'a'));
    EXPECT_FALSE(IsTrailUTF8(U'\x7f'));

    EXPECT_TRUE(IsTrailUTF8(U'\x80'));
    EXPECT_TRUE(IsTrailUTF8(U'\x99'));
}


TEST(CoreTests, test_IsLeadSurrogateUTF16) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsLeadSurrogateUTF16(U'\x0000'));
    EXPECT_FALSE(IsLeadSurrogateUTF16(U'z'));
    EXPECT_FALSE(IsLeadSurrogateUTF16(U'\xd7ff'));

    EXPECT_TRUE(IsLeadSurrogateUTF16(U'\xd800'));
    EXPECT_TRUE(IsLeadSurrogateUTF16(U'\xdab0'));
    EXPECT_TRUE(IsLeadSurrogateUTF16(U'\xdbff'));

    EXPECT_FALSE(IsLeadSurrogateUTF16(U'\xdc00'));
    EXPECT_FALSE(IsLeadSurrogateUTF16(U'\xdfff'));

    EXPECT_FALSE(IsLeadSurrogateUTF16(U'\xefff'));
}


TEST(CoreTests, test_IsTrailSurrogateUTF16) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsTrailSurrogateUTF16(U'\x0000'));
    EXPECT_FALSE(IsTrailSurrogateUTF16(U'z'));
    EXPECT_FALSE(IsTrailSurrogateUTF16(U'\xd7ff'));

    EXPECT_FALSE(IsTrailSurrogateUTF16(U'\xd800'));
    EXPECT_FALSE(IsTrailSurrogateUTF16(U'\xdbff'));

    EXPECT_TRUE(IsTrailSurrogateUTF16(U'\xdc00'));
    EXPECT_TRUE(IsTrailSurrogateUTF16(U'\xde02'));
    EXPECT_TRUE(IsTrailSurrogateUTF16(U'\xdfff'));
    EXPECT_FALSE(IsTrailSurrogateUTF16(U'\xe000'));

    EXPECT_FALSE(IsLeadSurrogateUTF16(U'\xefff'));
}


TEST(CoreTests, test_IsSurrogateUTF16) {
    using namespace jcu::utf;
    EXPECT_FALSE(IsSurrogateUTF16(U'\x0000'));
    EXPECT_FALSE(IsSurrogateUTF16(U'z'));
    EXPECT_FALSE(IsSurrogateUTF16(U'\xd7ff'));

    EXPECT_TRUE(IsSurrogateUTF16(U'\xd800'));
    EXPECT_TRUE(IsSurrogateUTF16(U'\xdbff'));
    EXPECT_TRUE(IsSurrogateUTF16(U'\xdc00'));
    EXPECT_TRUE(IsSurrogateUTF16(U'\xde02'));
    EXPECT_TRUE(IsSurrogateUTF16(U'\xdfff'));
    EXPECT_FALSE(IsSurrogateUTF16(U'\xe000'));

    EXPECT_FALSE(IsSurrogateUTF16(U'\xefff'));
    EXPECT_FALSE(IsSurrogateUTF16(U'\x001012af'));
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
    EXPECT_EQ(SequenceLength8(U'Z'), 1);
    EXPECT_EQ(SequenceLength8(static_cast<char32_t>(0x79)), 1);
    EXPECT_EQ(SequenceLength8(static_cast<char32_t>(0xc2)), 2);
    EXPECT_EQ(SequenceLength8(static_cast<char32_t>(0xe0)), 3);
    EXPECT_EQ(SequenceLength8(static_cast<char32_t>(0xf0)), 4);
    EXPECT_EQ(SequenceLength16(U'\x15'), 1);
    EXPECT_EQ(SequenceLength16(U'\xd915'), 2);
}
