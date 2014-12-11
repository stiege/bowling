#include "scoring.h"
#include "stdio.h" //strings
#include "stdbool.h" //bool

#define SCORECARD_FRAME_SLOTS 10
#define SCORE_ROLL_SLOTS 20
#define SIZE_OF_ROLL_SLOTS 4
#define SIZE_OF_FRAME 8

#define FIRST_ELEMENT 1
#define ROLL_FIRST_ELEMENT FIRST_ELEMENT
#define FRAME_FIRST_ELEMENT FIRST_ELEMENT
#define CARD_FIRST_ELEMENT FIRST_ELEMENT
#define ROLLS_PER_FRAME 2

static int rollInFrame;
static int frameNumber;
static int lastResult;


static const char blankCard[] = 
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|       |       |       |       |       |       |       |       |       |       |\n";

static char currentCard[sizeof(blankCard)];

static void resetCard(void);
static void progressToNextFrame(void);
static bool frameIsComplete(void);
static void markScoreCardForRoll(int pins);
static void markScoreCardForFrameResult(int pins);
static char pinsToChar(int val);
static int getReportCardRollOffset(void);
static int getReportCardFrameResultOffset(void);

void SCRNG_Init( void )
{
    resetCard();
}

void SCRNG_DrawScoreCard( char* scoreCard )
{
    sprintf(scoreCard, "%s", currentCard);
}

void SCRNG_Roll(int pins)
{

    markScoreCardForRoll(pins);
    rollInFrame++;

    if ( frameIsComplete() )
    {
        markScoreCardForFrameResult(pins);
        progressToNextFrame();
    }

    lastResult = pins;

}

static void progressToNextFrame(void)
{
    frameNumber++;
    rollInFrame = ROLL_FIRST_ELEMENT;
}

static bool frameIsComplete(void)
{
    return ( (ROLL_FIRST_ELEMENT + ROLLS_PER_FRAME) == rollInFrame);
}

static void markScoreCardForRoll(int pins)
{
    currentCard[getReportCardRollOffset()] = pinsToChar(pins);
}

static void resetCard(void)
{
    lastResult = 0;
    rollInFrame = ROLL_FIRST_ELEMENT;
    frameNumber = FRAME_FIRST_ELEMENT;
    sprintf(currentCard, "%s", blankCard);
}

static char pinsToChar(int val)
{
    return (val + 0x30);
}


static int getReportCardRollOffset()
{
    int offset = SIZE_OF_ROLL_SLOTS / 2 ; //offset of first roll
    //offset for additional roll
    offset += (rollInFrame - ROLL_FIRST_ELEMENT) * SIZE_OF_ROLL_SLOTS;
    //offset for frame
    offset += (frameNumber - FRAME_FIRST_ELEMENT) * SIZE_OF_FRAME;

    return offset;
}

static void markScoreCardForFrameResult(int pins)
{
    int frameScore = pins + lastResult;
    currentCard[getReportCardFrameResultOffset()] = pinsToChar(frameScore);
}

static int getReportCardFrameResultOffset(void)
{
    int offset = sizeof(blankCard)/2;
    offset += (frameNumber - FRAME_FIRST_ELEMENT + 1) * SIZE_OF_FRAME/2;

    return offset;
}