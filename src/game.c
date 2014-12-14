#include "game.h"
#include "stdbool.h" //bool

#include "projectconfig.h"
#include "bowlingframes.h"
#include "scorecard.h"



/*
________          __           ________                .__
\______ \ _____ _/  |______    \______ \   ____   ____ |  |
 |    |  \\__  \\   __\__  \    |    |  \_/ __ \_/ ___\|  |
 |    `   \/ __ \|  |  / __ \_  |    `   \  ___/\  \___|  |__
/_______  (____  /__| (____  / /_______  /\___  >\___  >____/
        \/     \/          \/          \/     \/     \/
*/


static struct tBowlingFrame currentFrame;
static struct tBowlingFrame previousFrame;
static struct tBowlingFrame priorPreviousFrame;

struct tGame
{
    int runningTotal;
    int frameNumber;
    int rollInFrame;
};

static struct tGame gameState;

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
static void markScoreCardForFrameResult(int frameNumber, struct tBowlingFrame frame);
static void markScoreCardForRoll(int pins);

/*
   _____ __________.___
  /  _  \\______   \   |
 /  /_\  \|     ___/   |
/    |    \    |   |   |
\____|__  /____|   |___|
        \/
*/

void GME_DrawScoreCard(char *scoreCard)
{
    SCRCRD_Print(scoreCard);
}

void GME_Init()
{
    SCRCRD_Init();
    gameState = gameInitState;
    BWLNGFRMS_Init(&currentFrame);
    BWLNGFRMS_Init(&previousFrame);
    BWLNGFRMS_Init(&priorPreviousFrame);
}

void GME_NextFrame(void)
{
    priorPreviousFrame = previousFrame;
    previousFrame = currentFrame;
    gameState.frameNumber++;
    gameState.rollInFrame = FRAME_FIRST_ELEMENT;
    BWLNGFRMS_Init(&currentFrame);
}

bool GME_FrameIsComplete(void)
{
    return ( ROLLS_PER_FRAME == gameState.rollInFrame
        || currentFrame.firstRowScore == 10);
}


void GME_CheckAndUpdatePreviousFrame(void)
{
    BWLNGFRMS_CalculateBonus(&priorPreviousFrame, &previousFrame, currentFrame);
    
    if ( BWLNGFRMS_ContextMultiStrike(&priorPreviousFrame , &previousFrame)
        && gameState.rollInFrame == ROLL_FIRST_ELEMENT)
    {
        gameState.runningTotal += priorPreviousFrame.bonus;
        SCRCRD_WriteScoreForFrame(
            gameState.frameNumber-2, 
            gameState.runningTotal
            - previousFrame.firstRowScore
        );
    }

    if (BWLNGFRMS_ContextSingleStrike(&previousFrame, &currentFrame) 
        && gameState.rollInFrame == ROLLS_PER_FRAME )
    {
        gameState.runningTotal += previousFrame.bonus;
        SCRCRD_WriteScoreForFrame(gameState.frameNumber - 1, gameState.runningTotal);
    }

    else if( BWLNGFRMS_ContextSpare(&previousFrame) 
            && gameState.rollInFrame == ROLL_FIRST_ELEMENT)
    {
        gameState.runningTotal += previousFrame.bonus;
        SCRCRD_WriteScoreForFrame(gameState.frameNumber - 1, gameState.runningTotal);
    }

}

void GME_ProcessRoll(int pins)
{
    switch (gameState.rollInFrame)
    {
        case 1:
            currentFrame.firstRowScore = pins;
        break;
        case 2:
            currentFrame.secondRowScore = pins;
        default:
        break;
    }

    markScoreCardForRoll(pins);

}

void GME_UpdateTotal(void)
{
    gameState.runningTotal += BWLNGFRMS_GetScore(currentFrame);
}

bool GME_FrameScoreKnown(void)
{
    return ( 
        ! (BWLNGFRMS_FrameIsASpare(currentFrame) 
        || BWLNGFRMS_FrameIsAStrike(currentFrame)) );
}

void GME_WriteFrameScore(void)
{
    markScoreCardForFrameResult(gameState.frameNumber, currentFrame);
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

static void markScoreCardForFrameResult(int frameNumber, struct tBowlingFrame frame)
{
    SCRCRD_WriteScoreForFrame(frameNumber, gameState.runningTotal);
}

static void markScoreCardForRoll(int pins)
{
    if (BWLNGFRMS_FrameIsASpare(currentFrame) )
    {
        SCRCRD_WriteSpare(gameState.frameNumber);
    }
    else if (BWLNGFRMS_FrameIsAStrike(currentFrame))
    {
        SCRCRD_WriteStrike(gameState.frameNumber);
    }
    else
    {
        SCRCRD_WriteRoll(gameState.frameNumber, 
            gameState.rollInFrame,
            pins );
    }
}


