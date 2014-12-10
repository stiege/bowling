#include "unity.h"
#include "scoring.h"

#define SAME_STRING 0

char scoreCard[200] = {0};
char *const score_ptr = scoreCard;
static void expectedScoreCard(char* string);

void setUp(void)
{

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
    SCRNG_Roll(4);
    char expectedString[] = 
"| 6 | 2 | 4 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |       |       |       |       |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);

}

static void expectedScoreCard(char* string)
{
    SCRNG_DrawScoreCard(score_ptr);
    TEST_ASSERT_MESSAGE( SAME_STRING == strcmp(score_ptr,string),
    "Did not get expected scorecard." );
}