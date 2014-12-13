#include "unity.h"
#include "stdio.h"
#include "scorecard.h"
#include "formatting.h"

#define SAME_STRING 0

char scoreCard[200] = {0};
char *const score_ptr = scoreCard;
static void expectedScoreCard(char* string);

void setUp(void)
{
    SCRCRD_Init();
}

void tearDown(void)
{
}

void test_SimpleScoreCardWrite(void)
{

    SCRCRD_WriteRoll(1,2,3); //frame,roll,score
    SCRCRD_WriteRoll(2,1,3); //frame,roll,score
    SCRCRD_WriteSpare(3); //frame
    SCRCRD_WriteStrike(6); //frame
    SCRCRD_WriteScoreForFrame(5,50); //frame, score
    char expectedString[] = 
"|   | 3 | 3 |   |   | / |   |   |   |   |   | X |   |   |   |   |   |   |   |   |\n"
"|       |       |       |       |  50   |       |       |       |       |       |\n";
    expectedScoreCard(expectedString);
}


static void expectedScoreCard(char* string)
{
    SCRCRD_Print(score_ptr);
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