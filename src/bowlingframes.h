
#ifndef bowlingframes_H
#define bowlingframes_H

#include "stdbool.h" //bool

struct tBowlingFrame 
{
    int firstRowScore;
    int secondRowScore;
    int bonus;
};

void BWLNGFRMS_Init(struct tBowlingFrame* frame);
bool BWLNGFRMS_FrameIsASpare( struct tBowlingFrame frame );
bool BWLNGFRMS_FrameIsAStrike( struct tBowlingFrame frame );
void BWLNGFRMS_CalculateBonus(
    struct tBowlingFrame* priorPreviousFrame,
    struct tBowlingFrame* previousFrame,
    struct tBowlingFrame currentFrame
    );
int BWLNGFRMS_GetScore( struct tBowlingFrame frame );


#endif // bowlingframes_H
