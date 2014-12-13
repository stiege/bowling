#ifndef game_H
#define game_H

#include "bowlingframes.h"

struct tGame
{
    int runningTotal;
    int frameNumber;
    int rollInFrame;
};

void GME_DrawScoreCard(char *scoreCard);
void GME_Init();
void GME_NextFrame(void);
bool GME_FrameIsComplete(void);
void GME_CheckAndUpdatePreviousFrame(void);
void GME_ProcessRoll(int pins);
void GME_UpdateTotal(void);
bool GME_FrameScoreKnown(void);
void GME_WriteFrameScore(void);
void GME_NextRoll(void);


#endif // game_H
