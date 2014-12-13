#include "unity.h"
#include "string.h"
#include "scoring.h"

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
    SCRNG_Init();
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
    SCRNG_Roll(6);
    SCRNG_Roll(2);
    SCRNG_Roll(1);
    char expectedString[] = 
"| 6 | 2 | 1 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |       |       |       |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);

}

void test_RollSecondFrame(void)
{
    test_drawShortGame();
    SCRNG_Roll(0);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |       |       |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_RollMisses(void)
{
    test_RollSecondFrame();
    SCRNG_Roll(0);
    SCRNG_Roll(0);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |       |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_RollIntoDoubleDigitScore(void)
{
    test_RollMisses();
    SCRNG_Roll(1);
    SCRNG_Roll(8);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_RollSpare(void)
{
    test_RollIntoDoubleDigitScore();
    SCRNG_Roll(1);
    SCRNG_Roll(9);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_CalculateSpare(void)
{
    test_RollSpare();
    SCRNG_Roll(3);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / | 3 |   |   |   |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |  31   |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_RollAStrike(void)
{
    test_CalculateSpare();
    SCRNG_Roll(3);
    SCRNG_Roll(10);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / | 3 | 3 |   | X |   |   |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |  31   |  34   |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}

void test_ComputeStrike(void)
{
    test_RollAStrike();
    SCRNG_Roll(3);
    SCRNG_Roll(4);
    char expectedString[] = 
"| 6 | 2 | 1 | 0 | 0 | 0 | 1 | 8 | 1 | / | 3 | 3 |   | X | 3 | 4 |   |   |   |   |\n"
"|   8   |   9   |   9   |  18   |  31   |  34   |  51   |  58   |       |       |\n";
    expectedScoreCard(expectedString);
}

static void expectedScoreCard(char* string)
{
    SCRNG_DrawScoreCard(score_ptr);
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