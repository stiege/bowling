#include "unity.h"
#include "scoring.h"
#include "string.h"
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
    SCRNG_Roll(4);
    char expectedString[] = 
"| 6 | 2 | 4 |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|   8   |       |       |       |       |       |       |       |       |       |\n";
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