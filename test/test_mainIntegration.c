#include "unity.h"
#include "string.h"
#include "main.h"

#include "bowlingframes.h"
#include "projectconfig.h"
#include "game.h"
#include "formatting.h"
#include "scorecard.h"

#define SAME_STRING 0

char scoreCard[200] = {0};
char *const score_ptr = scoreCard;
static void expectedScoreCard(char* string);

void setUp(void)
{
    MAIN_Init();
}

void tearDown(void)
{
}

void test_drawEmptyScoreCard(void)
{
    char expectedString[] = 
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|       |       |       |       |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_drawShortGame(void)
{
    MAIN_Roll(6);
    MAIN_Roll(2);
    MAIN_Roll(1);
    char expectedString[] = 
"| 6 | 2 | 1 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |       |       |       |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);

}

void test_RollSecondFrame(void)
{
    test_drawShortGame();
    MAIN_Roll(0);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |       |       |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_RollMisses(void)
{
    test_RollSecondFrame();
    MAIN_Roll(0);
    MAIN_Roll(0);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |       |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_RollIntoDoubleDigitScore(void)
{
    test_RollMisses();
    MAIN_Roll(1);
    MAIN_Roll(8);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_RollSpare(void)
{
    test_RollIntoDoubleDigitScore();
    MAIN_Roll(1);
    MAIN_Roll(9);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_CalculateSpare(void)
{
    test_RollSpare();
    MAIN_Roll(3);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / | 3 |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |  31   |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_RollAStrike(void)
{
    test_CalculateSpare();
    MAIN_Roll(4);
    MAIN_Roll(10);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / | 3 | 4 |   | X |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |  31   |  38   |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_ComputeStrike(void)
{
    test_RollAStrike();
    MAIN_Roll(3);
    MAIN_Roll(4);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / | 3 | 4 |   | X | 3 | 4 |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |  31   |  38   |  55   |  62   |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_FinishGame(void)
{
    test_ComputeStrike();
    MAIN_Roll(10);
    MAIN_Roll(2);
    MAIN_Roll(3);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / | 3 | 4 |   | X | 3 | 4 |   | X | 2 | 3 |\n"
"|   8   |   9   |   9   |  18   |  31   |  38   |  55   |  62   |  77   |  82   |\n";
    expectedScoreCard(expectedString);
}

void test_MultipleStrikes(void)
{
    MAIN_Roll(10);
    MAIN_Roll(10);
    MAIN_Roll(10);
    MAIN_Roll(2);
    MAIN_Roll(3);
    char expectedString[] = 
"|   | X |   | X |   | X | 2 | 3 |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|  30   |  52   |  67   |  72   |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

static void expectedScoreCard(char* string)
{
    MAIN_DrawScoreCard(score_ptr);
    if (SAME_STRING != strcmp(score_ptr,string))
    {
        char failMessage[2 * sizeof(scoreCard)];
        sprintf(failMessage, "\nExpected:\n");
        strcat(failMessage, string);
        strcat(failMessage, "\nReceived:\n");
        strcat(failMessage, scoreCard);
        printf( "%s", failMessage );
        TEST_FAIL();
    }
}