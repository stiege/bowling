#include "bowlingframes.h"

#include "stdbool.h" //bool

static const struct tBowlingFrame frameInitState = 
{
    .firstRowScore = 0,
    .secondRowScore = 0,
    .bonus = 0,
};

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
    struct tBowlingFrame* previousFrame,
    struct tBowlingFrame currentFrame
    )
{
    if (previousFrame->firstRowScore == 10)
    {
        previousFrame->bonus =
            currentFrame.firstRowScore
            + currentFrame.secondRowScore;
    }
    else if (previousFrame->firstRowScore + previousFrame->secondRowScore == 10)
    {
        previousFrame->bonus = currentFrame.firstRowScore;
    }
    else
    {
        previousFrame->bonus = 0;
    }
}

int BWLNGFRMS_GetScore( struct tBowlingFrame frame )
{
    return (frame.firstRowScore
        + frame.secondRowScore
        + frame.bonus);
}