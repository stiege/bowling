#ifndef game_H
#define game_H

#include "bowlingframes.h"

struct tGame
{
    int runningTotal;
    int frameNumber;
    int rollInFrame;
};

void GME_Init(void);
void GME_NextFrame(void);
bool GME_FrameIsComplete(void);

void GME_SetFrameScore(struct tBowlingFrame* frame, int pins);
bool GME_BonusAchievedLastRound(void);
void GME_UpdatePreviousFrame(void);

#endif // game_H
