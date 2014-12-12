#include "scoring.h"
#include "stdio.h" //strings
#include "stdbool.h" //bool


/*
_________                _____.__
\_   ___ \  ____   _____/ ____\__| ____
/    \  \/ /  _ \ /    \   __\|  |/ ___\
\     \___(  <_> )   |  \  |  |  / /_/  >
 \______  /\____/|___|  /__|  |__\___  /
        \/            \/        /_____/
*/

#define SCORECARD_FRAME_SLOTS 10
/*
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |"
"|   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   |   10  |"
*/

#define SCORE_ROLL_SLOTS 20
/*
"| 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |   |   |   |   |   |   |   |   |   |   |20 |"
"|       |       |       |       |       |       |       |       |       |       |";
*/

#define SIZE_OF_ROLL_SLOTS 4
/*
|   |   // <- last '|' belongs to the next row
*/

#define SIZE_OF_FRAME 8
/*
|       |    // <- last '|' belongs to the next frame
*/


/* 
We have numbered everything above from 1 
*/
#define FIRST_ELEMENT 1 
#define ROLL_FIRST_ELEMENT FIRST_ELEMENT
#define FRAME_FIRST_ELEMENT FIRST_ELEMENT


#define ROLLS_PER_FRAME 2
/*
| 1 | 2 |
|   1   |
*/


/*
________          __           ________                .__
\______ \ _____ _/  |______    \______ \   ____   ____ |  |
 |    |  \\__  \\   __\__  \    |    |  \_/ __ \_/ ___\|  |
 |    `   \/ __ \|  |  / __ \_  |    `   \  ___/\  \___|  |__
/_______  (____  /__| (____  / /_______  /\___  >\___  >____/
        \/     \/          \/          \/     \/     \/
*/
static struct tScoreCardState
{
    int rollInFrame;
    int frameNumber;
    int lastRowScore;
    int runningTotal;
    int thisRowScore;
}scoreCardState;

static const struct tScoreCardState cardInitState = 
{
    .rollInFrame = ROLL_FIRST_ELEMENT,
    .frameNumber = FRAME_FIRST_ELEMENT,
    .lastRowScore = 0,
    .runningTotal = 0,
    .thisRowScore = 0
};

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

static void resetCard(void);
static void progressToNextFrame(void);
static bool frameIsComplete(void);
static void markScoreCardForRoll(int);
static void markScoreCardForFrameResult(void);
static char pinsToChar(int val);
static int getReportCardRollOffset(void);
static int getReportCardFrameResultOffset(void);
static void scoreToString( char *writeTo , int score );


/*
   _____ __________.___
  /  _  \\______   \   |
 /  /_\  \|     ___/   |
/    |    \    |   |   |
\____|__  /____|   |___|
        \/
*/
void SCRNG_Init( void )
{
    resetCard();
}

void SCRNG_DrawScoreCard( char* scoreCard )
{
    sprintf(scoreCard, "%s", currentCard);
}

void SCRNG_Roll(int pins)
{

    markScoreCardForRoll(pins);
    scoreCardState.rollInFrame++;

    if ( frameIsComplete() )
    {
        markScoreCardForFrameResult();
        progressToNextFrame();
    }

    scoreCardState.lastRowScore = scoreCardState.thisRowScore;

}

/*
  ___ ___         .__
 /   |   \   ____ |  | ______   ___________  ______
/    ~    \_/ __ \|  | \____ \_/ __ \_  __ \/  ___/
\    Y    /\  ___/|  |_|  |_> >  ___/|  | \/\___ \
 \___|_  /  \___  >____/   __/ \___  >__|  /____  >
       \/       \/     |__|        \/           \/
*/

static void progressToNextFrame(void)
{
    scoreCardState.frameNumber++;
    scoreCardState.rollInFrame = ROLL_FIRST_ELEMENT;

    scoreCardState.runningTotal += 
        scoreCardState.lastRowScore
        + scoreCardState.thisRowScore;
}

static bool frameIsComplete(void)
{
    return ( (ROLL_FIRST_ELEMENT + ROLLS_PER_FRAME) == scoreCardState.rollInFrame);
}

static void markScoreCardForRoll(int pins)
{
    scoreCardState.thisRowScore = pins;
    currentCard[getReportCardRollOffset()] = pinsToChar(scoreCardState.thisRowScore);
}

static void resetCard(void)
{
    scoreCardState = cardInitState;
    sprintf(currentCard, "%s", blankCard);
}

static char pinsToChar(int val)
{
    return (val + 0x30);
}


static int getReportCardRollOffset()
{
    int offset = SIZE_OF_ROLL_SLOTS / 2 ; //offset of first roll
    //offset for additional roll
    offset += (scoreCardState.rollInFrame - ROLL_FIRST_ELEMENT) * SIZE_OF_ROLL_SLOTS;
    //offset for frame
    offset += (scoreCardState.frameNumber - FRAME_FIRST_ELEMENT) * SIZE_OF_FRAME;

    return offset;
}

static void markScoreCardForFrameResult(void)
{
    int frameScore = scoreCardState.thisRowScore 
        + scoreCardState.lastRowScore
        + scoreCardState.runningTotal;
    scoreToString( &currentCard[getReportCardFrameResultOffset()] , frameScore );
}

static int getReportCardFrameResultOffset(void)
{
    int offset = sizeof(blankCard)/2 + SIZE_OF_FRAME/2;
    offset += (scoreCardState.frameNumber - FRAME_FIRST_ELEMENT ) * SIZE_OF_FRAME;
    return offset;
}

static void scoreToString( char *writeTo , int score )
{
    if (score < 10)
    {
        *writeTo = pinsToChar(score);
    }
    else if (score < 100)
    {
        int tens = score/10;
        int ones = score - tens * 10;
        *(writeTo - 1) = pinsToChar(tens);
        *writeTo = pinsToChar(ones);
    }
}