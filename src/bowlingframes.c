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
    struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame,
    struct tBowlingFrame currentFrame
    )
{

    if (BWLNGFRMS_FrameIsAStrike(*priorPreviousFrame)
        && ! BWLNGFRMS_FrameIsAStrike(*previousFrame) )
    {
        priorPreviousFrame->bonus = 
        previousFrame->firstRowScore
        + previousFrame->secondRowScore;
    }
    if (BWLNGFRMS_FrameIsAStrike(*priorPreviousFrame)
        && BWLNGFRMS_FrameIsAStrike(*previousFrame))
    {
        priorPreviousFrame->bonus = 
        previousFrame->firstRowScore
        + currentFrame.firstRowScore;
    }

    if (BWLNGFRMS_FrameIsAStrike(*previousFrame) 
        && !BWLNGFRMS_FrameIsAStrike(currentFrame) )
    {
        previousFrame->bonus =
            currentFrame.firstRowScore
            + currentFrame.secondRowScore;
    }
    if (BWLNGFRMS_FrameIsASpare(*previousFrame) )
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