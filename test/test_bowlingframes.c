#include "unity.h"
#include "bowlingframes.h"

struct tBowlingFrame testFrame;
struct tBowlingFrame testFramePrevious;

void setUp(void)
{
    BWLNGFRMS_Init(&testFrame);
    BWLNGFRMS_Init(&testFramePrevious);
}

void tearDown(void)
{
}

void test_computesSpare(void)
{
    testFrame.firstRowScore = 2;
    testFrame.secondRowScore = 8;
    TEST_ASSERT( BWLNGFRMS_FrameIsASpare(testFrame) );
}

void test_computesBonus(void)
{
    testFrame.secondRowScore = 8;
    testFrame.firstRowScore = 2;

    BWLNGFRMS_CalculateBonus(&testFramePrevious, testFrame);
    TEST_ASSERT( testFramePrevious.bonus == testFrame.firstRowScore );
}
