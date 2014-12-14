#include "bowlingframes.h"

#include "stdbool.h" //bool


/*
________          __           ________                .__
\______ \ _____ _/  |______    \______ \   ____   ____ |  |
 |    |  \\__  \\   __\__  \    |    |  \_/ __ \_/ ___\|  |
 |    `   \/ __ \|  |  / __ \_  |    `   \  ___/\  \___|  |__
/_______  (____  /__| (____  / /_______  /\___  >\___  >____/
        \/     \/          \/          \/     \/     \/
*/

static const struct tBowlingFrame frameInitState = 
{
    .firstRowScore = 0,
    .secondRowScore = 0,
    .bonus = 0,
};

/*
   _____ __________.___
  /  _  \\______   \   |
 /  /_\  \|     ___/   |
/    |    \    |   |   |
\____|__  /____|   |___|
        \/
*/

bool BWLNGFRMS_FrameIsASpare( struct tBowlingFrame frame )
{
    return (frame.firstRowScore != 10 
    && frame.firstRowScore + frame.secondRowScore == 10);
}

bool BWLNGFRMS_FrameIsAStrike( struct tBowlingFrame frame )
{
    return (frame.firstRowScore == 10);
}

void BWLNGFRMS_Init( struct tBowlingFrame* frame )
{
    *frame = frameInitState;
}

void BWLNGFRMS_CalculateBonus(
    struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame,
    struct tBowlingFrame currentFrame
    )
{

    if ( BWLNGFRMS_ContextSingleStrike(priorPreviousFrame, previousFrame) )
    {
        priorPreviousFrame->bonus = 
        previousFrame->firstRowScore
        + previousFrame->secondRowScore;
    }

    else if ( BWLNGFRMS_ContextMultiStrike(priorPreviousFrame, previousFrame) )
    {
        priorPreviousFrame->bonus = 
        previousFrame->firstRowScore
        + currentFrame.firstRowScore;
    }
    
    if (BWLNGFRMS_ContextSingleStrike(previousFrame, &currentFrame) )
    {
        previousFrame->bonus =
            currentFrame.firstRowScore
            + currentFrame.secondRowScore;
    }

    else if ( BWLNGFRMS_ContextSpare(previousFrame) )
    {
        previousFrame->bonus = currentFrame.firstRowScore;
    }

}

int BWLNGFRMS_GetScore( struct tBowlingFrame frame )
{
    return (frame.firstRowScore
        + frame.secondRowScore
        + frame.bonus);
}

bool BWLNGFRMS_ContextMultiStrike(struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame)
{
    return (BWLNGFRMS_FrameIsAStrike(*priorPreviousFrame)
        && BWLNGFRMS_FrameIsAStrike(*previousFrame));
}

bool BWLNGFRMS_ContextSingleStrike(struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame)
{
    return (BWLNGFRMS_FrameIsAStrike(*priorPreviousFrame)
        && ! BWLNGFRMS_FrameIsAStrike(*previousFrame));
}

bool BWLNGFRMS_ContextSpare(struct tBowlingFrame* previousFrame)
{
    return BWLNGFRMS_FrameIsASpare(*previousFrame);
}
