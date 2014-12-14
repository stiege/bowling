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
__________                __
\______   \_______  _____/  |_  ____  ______
 |     ___/\_  __ \/  _ \   __\/  _ \/  ___/
 |    |     |  | \(  <_> )  | (  <_> )___ \
 |____|     |__|   \____/|__|  \____/____  >
                                         \/
*/
static bool contextMultiStrike(struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame);
static bool contextSingleStrike(struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame);
static bool contextSpare(struct tBowlingFrame* previousFrame);

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

    if ( contextSingleStrike(priorPreviousFrame, previousFrame) )
    {
        priorPreviousFrame->bonus = 
        previousFrame->firstRowScore
        + previousFrame->secondRowScore;
    }

    else if ( contextMultiStrike(priorPreviousFrame, previousFrame) )
    {
        priorPreviousFrame->bonus = 
        previousFrame->firstRowScore
        + currentFrame.firstRowScore;
    }
    
    if (contextSingleStrike(previousFrame, &currentFrame) )
    {
        previousFrame->bonus =
            currentFrame.firstRowScore
            + currentFrame.secondRowScore;
    }

    else if ( contextSpare(previousFrame) )
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

/*
  ___ ___         .__
 /   |   \   ____ |  | ______   ___________  ______
/    ~    \_/ __ \|  | \____ \_/ __ \_  __ \/  ___/
\    Y    /\  ___/|  |_|  |_> >  ___/|  | \/\___ \
 \___|_  /  \___  >____/   __/ \___  >__|  /____  >
       \/       \/     |__|        \/           \/
*/

static bool contextMultiStrike(struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame)
{
    return (BWLNGFRMS_FrameIsAStrike(*priorPreviousFrame)
        && BWLNGFRMS_FrameIsAStrike(*previousFrame));
}

static bool contextSingleStrike(struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame)
{
    return (BWLNGFRMS_FrameIsAStrike(*priorPreviousFrame)
        && ! BWLNGFRMS_FrameIsAStrike(*previousFrame));
}

static bool contextSpare(struct tBowlingFrame* previousFrame)
{
    return BWLNGFRMS_FrameIsASpare(*previousFrame);
}
