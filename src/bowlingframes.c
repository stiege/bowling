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
    return (frame.firstRowScore + frame.secondRowScore == 10);
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
    previousFrame->bonus = currentFrame.firstRowScore;
}

int BWLNGFRMS_GetScore( struct tBowlingFrame frame )
{
    return (frame.firstRowScore
        + frame.secondRowScore
        + frame.bonus);
}