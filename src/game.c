#include "stdbool.h" //bool
#include "stdio.h" //strings

#include "game.h"
#include "projectconfig.h"

#include "bowlingframes.h"

#include "formatting.h"


/*
________          __           ________                .__
\______ \ _____ _/  |______    \______ \   ____   ____ |  |
 |    |  \\__  \\   __\__  \    |    |  \_/ __ \_/ ___\|  |
 |    `   \/ __ \|  |  / __ \_  |    `   \  ___/\  \___|  |__
/_______  (____  /__| (____  / /_______  /\___  >\___  >____/
        \/     \/          \/          \/     \/     \/
*/

static struct tGame gameState;

static struct tBowlingFrame currentFrame;
static struct tBowlingFrame previousFrame;

static const char blankCard[] = 
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|       |       |       |       |       |       |       |       |       |       |\n";

static char currentCard[sizeof(blankCard)];


static const struct tGame gameInitState = 
{
    .runningTotal = 0,
    .frameNumber = FRAME_FIRST_ELEMENT,
    .rollInFrame = ROLL_FIRST_ELEMENT
};

/*
__________                __
\______   \_______  _____/  |_  ____  ______
 |     ___/\_  __ \/  _ \   __\/  _ \/  ___/
 |    |     |  | \(  <_> )  | (  <_> )___ \
 |____|     |__|   \____/|__|  \____/____  >
                                         \/
*/
static void resetCard(void);
static void markScoreCardForRoll(int);
static void markScoreCardForFrameResult(struct tBowlingFrame frame);
static int getReportCardRollOffset(void);
static int getReportCardFrameResultOffset(struct tBowlingFrame frame);

static void markScoreCardForFrameResult(struct tBowlingFrame frame);

/*
   _____ __________.___
  /  _  \\______   \   |
 /  /_\  \|     ___/   |
/    |    \    |   |   |
\____|__  /____|   |___|
        \/
*/

void GME_DrawScoreCard(char* scoreCard)
{
    sprintf(scoreCard, "%s", currentCard);
}

void GME_Init()
{
    resetCard();
    gameState = gameInitState;
    BWLNGFRMS_Init(&currentFrame);
    BWLNGFRMS_Init(&previousFrame);
}

void GME_NextFrame(void)
{
    previousFrame = currentFrame;
    gameState.frameNumber++;
    gameState.rollInFrame = FRAME_FIRST_ELEMENT;
    BWLNGFRMS_Init(&currentFrame);
}

bool GME_FrameIsComplete(void)
{
    return ( ROLLS_PER_FRAME == gameState.rollInFrame);
}

void GME_SetFrameScore(struct tBowlingFrame* frame, int pins)
{
    switch (gameState.rollInFrame)
    {
        case 1:
            frame->firstRowScore = pins;
        break;
        case 2:
            frame->secondRowScore = pins;
        default:
        break;
    }
}

void GME_CheckAndUpdatePreviousFrame(void)
{
    if( BWLNGFRMS_FrameIsASpare(previousFrame) )
    {
        BWLNGFRMS_CalculateBonus(&previousFrame, currentFrame);
        markScoreCardForFrameResult(previousFrame);
    }
}

void GME_ProcessRoll(int pins)
{
    markScoreCardForRoll(pins);
}

void GME_UpdateTotal(void)
{
    gameState.runningTotal += BWLNGFRMS_GetScore(currentFrame);
}

bool GME_FrameScoreKnown(void)
{
    return (! BWLNGFRMS_FrameIsASpare(currentFrame) );
}

void GME_WriteFrameScore(void)
{
    markScoreCardForFrameResult(currentFrame);
}

void GME_NextRoll(void)
{
    gameState.rollInFrame++;
}
/*
  ___ ___         .__
 /   |   \   ____ |  | ______   ___________  ______
/    ~    \_/ __ \|  | \____ \_/ __ \_  __ \/  ___/
\    Y    /\  ___/|  |_|  |_> >  ___/|  | \/\___ \
 \___|_  /  \___  >____/   __/ \___  >__|  /____  >
       \/       \/     |__|        \/           \/
*/

static void markScoreCardForFrameResult(struct tBowlingFrame frame)
{
    int totalScore = gameState.runningTotal + frame.bonus;
    FRMTNG_ScoreToString( &currentCard[getReportCardFrameResultOffset(frame)] , totalScore );
}

static void markScoreCardForRoll(int pins)
{
    GME_SetFrameScore(&currentFrame, pins);

    currentCard[getReportCardRollOffset()] = FRMTNG_PinsToChar(pins, BWLNGFRMS_FrameIsASpare(currentFrame));
}

static void resetCard(void)
{
    sprintf(currentCard, "%s", blankCard);
}


static int getReportCardRollOffset()
{
    int offset = SIZE_OF_ROLL_SLOTS / 2 ; //offset of first roll
    //offset for additional roll
    offset += (gameState.rollInFrame - ROLL_FIRST_ELEMENT) * SIZE_OF_ROLL_SLOTS;
    //offset for frame
    offset += (gameState.frameNumber - FRAME_FIRST_ELEMENT) * SIZE_OF_FRAME;

    return offset;
}

static int getReportCardFrameResultOffset(struct tBowlingFrame frame)
{
    int offset = sizeof(blankCard)/2 + SIZE_OF_FRAME/2;
    int targetFrame = gameState.frameNumber;

    if ( BWLNGFRMS_FrameIsASpare(frame) )
    {
        /*
        This frame was a spare so we're putting in its score halfway through
        the frame after.
        */
        targetFrame -=1;
    }
    offset += (targetFrame - FRAME_FIRST_ELEMENT ) * SIZE_OF_FRAME;

    return offset;
}