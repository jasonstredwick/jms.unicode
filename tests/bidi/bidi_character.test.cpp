// Copyright © 2024 Jason Stredwick

#include <format>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "ftest.h"

#include "jcu/bidi/algorithm.hpp"
#include "jcu/bidi/runs.hpp"
#include "jcu/ucd/bidi_character_test.hpp"


TEST(BidiTests, test_BidiCharacterTest) {
    /* Create code point sequence for a sample bidirectional text. */
    std::u32string_view bidi_text{U"یہ ایک )car( ہے۔"};
    std::vector<jcu::bidi::Run> runs = jcu::bidi::ToRuns(bidi_text);
    //for (auto& run : runs) {
    //    std::cout << std::format("Run{}: {} {}", run.level, run.offset, run.length);
    //}
    EXPECT_TRUE(true);
}


#if 0
    /* Extract the first bidirectional paragraph. */
    SBAlgorithmRef bidiAlgorithm = SBAlgorithmCreate(&codepointSequence);
    SBParagraphRef firstParagraph = SBAlgorithmCreateParagraph(bidiAlgorithm, 0, INT32_MAX, SBLevelDefaultLTR);
    SBUInteger paragraphLength = SBParagraphGetLength(firstParagraph);

    /* Create a line consisting of whole paragraph and get its runs. */
    SBLineRef paragraphLine = SBParagraphCreateLine(firstParagraph, 0, paragraphLength);
    SBUInteger runCount = SBLineGetRunCount(paragraphLine);
    const SBRun *runArray = SBLineGetRunsPtr(paragraphLine);

    /* Log the details of each run in the line. */
    for (SBUInteger i = 0; i < runCount; i++) {
        printf("Run Offset: %ld\n", (long)runArray[i].offset);
        printf("Run Length: %ld\n", (long)runArray[i].length);
        printf("Run Level: %ld\n\n", (long)runArray[i].level);
    }

    /* Create a mirror locator and load the line in it. */
    SBMirrorLocatorRef mirrorLocator = SBMirrorLocatorCreate();
    SBMirrorLocatorLoadLine(mirrorLocator, paragraphLine, (void *)bidiText);
    const SBMirrorAgent *mirrorAgent = SBMirrorLocatorGetAgent(mirrorLocator);

    /* Log the details of each mirror in the line. */
    while (SBMirrorLocatorMoveNext(mirrorLocator)) {
        printf("Mirror Index: %ld\n", (long)mirrorAgent->index);
        printf("Actual Code Point: %ld\n", (long)mirrorAgent->codepoint);
        printf("Mirrored Code Point: %ld\n\n", (long)mirrorAgent->mirror);
    }

    /* Release all objects. */
    SBMirrorLocatorRelease(mirrorLocator);
    SBLineRelease(paragraphLine);
    SBParagraphRelease(firstParagraph);
    SBAlgorithmRelease(bidiAlgorithm);
#endif
