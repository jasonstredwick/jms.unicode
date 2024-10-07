// Copyright © 2014-2022 Muhammad Tayyab Akram

#include <string>
#include <string_view>
#include <vector>

#include "jcu/bidi/algorithm.hpp"
#include "jcu/bidi/level.hpp"
#include "jcu/bidi/runs.hpp"
#include "ftest.h"


TEST(BidiTests, test_Basic) {
    using namespace jcu;
    using namespace jcu::bidi;

    {
        std::u32string_view bidi_text{U"hello"};
        jcu::utf::CodePointView view{bidi_text};
        std::vector<Run> runs = ToRuns(view, LEVEL_TYPE_DEFAULT_LTR);
        if (runs.size() == 1) {
            EXPECT_EQ(runs[0].level, 0);
            EXPECT_EQ(runs[0].offset, 0);
            EXPECT_EQ(runs[0].length, 5);
        } else {
            EXPECT_FALSE(true);
        }
    }

    {
        std::u32string_view bidi_text{U"یہ ایک ہے۔"};
        jcu::utf::CodePointView view{bidi_text};
        std::vector<Run> runs = ToRuns(view, LEVEL_TYPE_DEFAULT_LTR);
        if (runs.size() == 1) {
            EXPECT_EQ(runs[0].level, 1);
            EXPECT_EQ(runs[0].offset, 0);
            EXPECT_EQ(runs[0].length, 10);
        } else {
            EXPECT_FALSE(true);
        }
    }

    {
        std::u32string_view bidi_text{U"یہ ایک car ہے۔"};
        jcu::utf::CodePointView view{bidi_text};
        std::vector<Run> runs = ToRuns(view, LEVEL_TYPE_DEFAULT_LTR);
        if (runs.size() == 3) {
            EXPECT_EQ(runs[0].level, 1);
            EXPECT_EQ(runs[0].offset, 10);
            EXPECT_EQ(runs[0].length, 4);

            EXPECT_EQ(runs[1].level, 2);
            EXPECT_EQ(runs[1].offset, 7);
            EXPECT_EQ(runs[1].length, 3);

            EXPECT_EQ(runs[2].level, 1);
            EXPECT_EQ(runs[2].offset, 0);
            EXPECT_EQ(runs[2].length, 7);
        } else {
            EXPECT_FALSE(true);
        }
    }

    {
        std::u32string_view bidi_text{U"یہ ایک )car( ہے۔"};
        jcu::utf::CodePointView view{bidi_text};
        std::vector<Run> runs = ToRuns(view, LEVEL_TYPE_DEFAULT_LTR);
        if (runs.size() == 3) {
            EXPECT_EQ(runs[0].level, 1);
            EXPECT_EQ(runs[0].offset, 11);
            EXPECT_EQ(runs[0].length, 5);

            EXPECT_EQ(runs[1].level, 2);
            EXPECT_EQ(runs[1].offset, 8);
            EXPECT_EQ(runs[1].length, 3);

            EXPECT_EQ(runs[2].level, 1);
            EXPECT_EQ(runs[2].offset, 0);
            EXPECT_EQ(runs[2].length, 8);
        } else {
            EXPECT_FALSE(true);
        }
    }
}
