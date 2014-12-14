#include "unity.h"
#include "bowlingframes.h"

struct tBowlingFrame testFrame;
struct tBowlingFrame testFramePrevious;
struct tBowlingFrame testFramePriorPrevious;

void setUp(void)
{
    BWLNGFRMS_Init(&testFrame);
    BWLNGFRMS_Init(&testFramePrevious);
    BWLNGFRMS_Init(&testFramePriorPrevious);
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

void test_computesStrike(void)
{
    testFrame.firstRowScore = 10;
    testFrame.secondRowScore = 0;
    TEST_ASSERT( BWLNGFRMS_FrameIsAStrike(testFrame) );
}

void test_computesBonus(void)
{

    /*Spare bonus*/
    testFramePrevious.firstRowScore = 2;
    testFramePrevious.secondRowScore = 8;
    testFrame.firstRowScore = 2;
    testFrame.secondRowScore = 3;

    BWLNGFRMS_CalculateBonus(&testFramePriorPrevious,&testFramePrevious, testFrame);
    TEST_ASSERT( testFramePrevious.bonus == testFrame.firstRowScore );

    /*Strike bonus*/
    testFramePrevious.firstRowScore = 10;
    testFramePrevious.secondRowScore = 0;
    testFrame.firstRowScore = 3;
    testFrame.secondRowScore = 4;
    BWLNGFRMS_CalculateBonus(&testFramePriorPrevious,&testFramePrevious, testFrame);
    TEST_ASSERT( testFramePrevious.bonus == testFrame.firstRowScore + testFrame.secondRowScore);

}

void test_MultiStrike (void)
{
    /*Multi strike bonus*/

    /*Strike*/
    testFramePriorPrevious.firstRowScore = 0;
    testFramePriorPrevious.secondRowScore = 0;
    testFramePrevious.firstRowScore = 0;
    testFramePrevious.secondRowScore = 0;
    testFrame.firstRowScore = 10;
    testFrame.secondRowScore = 0;
    BWLNGFRMS_CalculateBonus(&testFramePriorPrevious, &testFramePrevious, testFrame);
    TEST_ASSERT_EQUAL_INT( 0, testFramePriorPrevious.bonus );
    TEST_ASSERT_EQUAL_INT( 0, testFramePrevious.bonus );
    TEST_ASSERT_EQUAL_INT( 0, testFrame.bonus );

    /*2nd strike*/
    testFramePriorPrevious = testFramePrevious;
    testFramePrevious = testFrame;
    testFrame.firstRowScore = 10;
    testFrame.secondRowScore = 0;
    BWLNGFRMS_CalculateBonus(&testFramePriorPrevious,&testFramePrevious, testFrame);
    TEST_ASSERT_EQUAL_INT( 0, testFramePriorPrevious.bonus );
    TEST_ASSERT_EQUAL_INT( 0, testFramePrevious.bonus );
    TEST_ASSERT_EQUAL_INT( 0, testFrame.bonus );

    /*Roll 2*/
    testFramePriorPrevious = testFramePrevious;
    testFramePrevious = testFrame;
    testFrame.firstRowScore = 2;
    testFrame.secondRowScore = 0;
    BWLNGFRMS_CalculateBonus(&testFramePriorPrevious,&testFramePrevious, testFrame);
    TEST_ASSERT_EQUAL_INT( 12, testFramePriorPrevious.bonus );

    /*Roll 3*/
    testFrame.secondRowScore=3;
    BWLNGFRMS_CalculateBonus(&testFramePriorPrevious,&testFramePrevious, testFrame);
    TEST_ASSERT_EQUAL_INT( 5, testFramePrevious.bonus );
}
