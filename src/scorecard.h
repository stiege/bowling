#ifndef scorecard_H
#define scorecard_H


void SCRCRD_Init();
void SCRCRD_WriteRoll(int frame, int roll, int pins);
void SCRCRD_WriteSpare(int frame);
void SCRCRD_WriteScoreForFrame(int frame, int score);

void SCRCRD_Print();


#endif // scorecard_H
