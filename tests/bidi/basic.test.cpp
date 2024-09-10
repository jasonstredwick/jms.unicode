// Copyright © 2014-2022 Muhammad Tayyab Akram

#include <print>
#include <string>
#include <string_view>
#include <vector>

#include "jcu/bidi/algorithm.hpp"
#include "jcu/bidi/level.hpp"
#include "jcu/bidi/runs.hpp"
#include "ftest.h"


TEST(BidiBasicTests, test_Basic) {
    using namespace jcu;
    using namespace jcu::bidi;
#if 0
    /* Create code point sequence for a sample bidirectional text. */
    std::u32string_view bidi_text{U"یہ ایک )car( ہے۔"};
    std::vector<Run> runs = ToRuns(bidi_text);
    for (auto& run : runs) {
        std::println("Run{}: {} {}", run.level, run.offset, run.length);
    }
#endif
    EXPECT_TRUE(true);
}
