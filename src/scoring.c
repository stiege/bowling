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

static struct tScoreCardState
{
    int rollInFrame;
    int frameNumber;
    int lastResult;
    int lastFrameScore;
    int thisScore;
}scoreCardState;

static const struct tScoreCardState cardInitState = 
{
    .rollInFrame = ROLL_FIRST_ELEMENT,
    .frameNumber = FRAME_FIRST_ELEMENT,
    .lastResult = 0,
    .lastFrameScore = 0,
    .thisScore = 0
};

static const char blankCard[] = 
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|       |       |       |       |       |       |       |       |       |       |\n";

static char currentCard[sizeof(blankCard)];

static void resetCard(void);
static void progressToNextFrame(void);
static bool frameIsComplete(void);
static void markScoreCardForRoll(int);
static void markScoreCardForFrameResult(void);
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
    scoreCardState.rollInFrame++;

    if ( frameIsComplete() )
    {
        markScoreCardForFrameResult();
        progressToNextFrame();
    }

    scoreCardState.lastResult = scoreCardState.thisScore;

}

static void progressToNextFrame(void)
{
    scoreCardState.frameNumber++;
    scoreCardState.rollInFrame = ROLL_FIRST_ELEMENT;

    scoreCardState.lastFrameScore = 
        scoreCardState.lastResult
        + scoreCardState.thisScore;
}

static bool frameIsComplete(void)
{
    return ( (ROLL_FIRST_ELEMENT + ROLLS_PER_FRAME) == scoreCardState.rollInFrame);
}

static void markScoreCardForRoll(int pins)
{
    scoreCardState.thisScore = pins;
    currentCard[getReportCardRollOffset()] = pinsToChar(scoreCardState.thisScore);
}

static void resetCard(void)
{
    scoreCardState = cardInitState;
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
    offset += (scoreCardState.rollInFrame - ROLL_FIRST_ELEMENT) * SIZE_OF_ROLL_SLOTS;
    //offset for frame
    offset += (scoreCardState.frameNumber - FRAME_FIRST_ELEMENT) * SIZE_OF_FRAME;

    return offset;
}

static void markScoreCardForFrameResult(void)
{
    int frameScore = scoreCardState.thisScore 
        + scoreCardState.lastResult
        + scoreCardState.lastFrameScore;
    currentCard[getReportCardFrameResultOffset()] = pinsToChar(frameScore);
}

static int getReportCardFrameResultOffset(void)
{
    int offset = sizeof(blankCard)/2 + SIZE_OF_FRAME/2;
    offset += (scoreCardState.frameNumber - FRAME_FIRST_ELEMENT ) * SIZE_OF_FRAME;
    return offset;
}