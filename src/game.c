#include "stdbool.h" //bool


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


static struct tBowlingFrame currentFrame;
static struct tBowlingFrame previousFrame;

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
static int getReportCardRollOffset(void);
static int getReportCardFrameResultOffset(struct tBowlingFrame frame);

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


void GME_CheckAndUpdatePreviousFrame(void)
{
    if( BWLNGFRMS_FrameIsASpare(previousFrame) )
    {
        BWLNGFRMS_CalculateBonus(&previousFrame, currentFrame);
        int totalScore = gameState.runningTotal + previousFrame.bonus;
        SCRCRD_WriteScoreForFrame(gameState.frameNumber-1, totalScore);
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
    return (! BWLNGFRMS_FrameIsASpare(currentFrame) );
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
    int totalScore = gameState.runningTotal + frame.bonus;
    SCRCRD_WriteScoreForFrame(frameNumber, totalScore);

}

static void markScoreCardForRoll(int pins)
{
    if (BWLNGFRMS_FrameIsASpare(currentFrame) )
    {
        SCRCRD_WriteSpare(gameState.frameNumber);
    }
    else
    {
        SCRCRD_WriteRoll(gameState.frameNumber, 
            gameState.rollInFrame,
            pins );
    }
}


