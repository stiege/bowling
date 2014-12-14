#include "scorecard.h"
#include "stdio.h" //strings

#include "projectconfig.h"

/*
________          __           ________                .__
\______ \ _____ _/  |______    \______ \   ____   ____ |  |
 |    |  \\__  \\   __\__  \    |    |  \_/ __ \_/ ___\|  |
 |    `   \/ __ \|  |  / __ \_  |    `   \  ___/\  \___|  |__
/_______  (____  /__| (____  / /_______  /\___  >\___  >____/
        \/     \/          \/          \/     \/     \/
*/
static const char blankCard[] = 
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|       |       |       |       |       |       |       |       |       |       |\n";

static char currentCard[sizeof(blankCard)];

/*
__________                __
\______   \_______  _____/  |_  ____  ______
 |     ___/\_  __ \/  _ \   __\/  _ \/  ___/
 |    |     |  | \(  <_> )  | (  <_> )___ \
 |____|     |__|   \____/|__|  \____/____  >
                                         \/
*/
static int getReportCardFrameResultOffset(int frame);
static int getReportCardRollOffset(int frame, int roll);

/*
   _____ __________.___
  /  _  \\______   \   |
 /  /_\  \|     ___/   |
/    |    \    |   |   |
\____|__  /____|   |___|
        \/
*/

void SCRCRD_Init()
{
    sprintf(currentCard, "%s", blankCard);
}

void SCRCRD_WriteScoreForFrame(int frame, int score)
{
    FRMTNG_ScoreToString( &currentCard[getReportCardFrameResultOffset(frame)] , score );
}

void SCRCRD_WriteRoll(int frame, int roll, int pins)
{
    currentCard[getReportCardRollOffset(frame, roll)] = FRMTNG_IntToChar(pins);
}

void SCRCRD_Print(char* scoreCard)
{
    sprintf(scoreCard, "%s", currentCard);
}

void SCRCRD_WriteSpare(int frame)
{
    currentCard[getReportCardRollOffset(frame, ROLLS_PER_FRAME)] = '/';
}

void SCRCRD_WriteStrike(int frame)
{
    currentCard[getReportCardRollOffset(frame, ROLLS_PER_FRAME)] = 'X';
}
/*
  ___ ___         .__
 /   |   \   ____ |  | ______   ___________  ______
/    ~    \_/ __ \|  | \____ \_/ __ \_  __ \/  ___/
\    Y    /\  ___/|  |_|  |_> >  ___/|  | \/\___ \
 \___|_  /  \___  >____/   __/ \___  >__|  /____  >
       \/       \/     |__|        \/           \/
*/
static int getReportCardFrameResultOffset(int frame)
{
    int offset = sizeof(blankCard)/2 + SIZE_OF_FRAME/2;
    offset += (frame - FRAME_FIRST_ELEMENT ) * SIZE_OF_FRAME;

    return offset;
}

static int getReportCardRollOffset(int frame, int roll)
{
    int offset = SIZE_OF_ROLL_SLOTS / 2 ; //offset of first roll
    //offset for additional roll
    offset += (roll - ROLL_FIRST_ELEMENT) * SIZE_OF_ROLL_SLOTS;
    //offset for frame
    offset += (frame - FRAME_FIRST_ELEMENT) * SIZE_OF_FRAME;

    return offset;
}