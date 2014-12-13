#include "scoring.h"
#include "stdio.h" //strings
#include "stdbool.h" //bool
#include "bowlingframes.h"


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

#define SIZE_OF_ROLL_SLOTS ( sizeof("|   |") - sizeof("|") )
/*
The last '|' in " |   | " belongs to the next row.
We're also implicitly subtracting the size of the termination character.
*/

#define SIZE_OF_FRAME ( sizeof("|       |") - sizeof("|") )
/*
The last '|' in  " |       | " belongs to the next frame
*/


/* 
We have numbered everything above from 1 
*/
#define FIRST_ELEMENT 1 
#define ROLL_FIRST_ELEMENT FIRST_ELEMENT
#define FRAME_FIRST_ELEMENT FIRST_ELEMENT


#define ROLLS_PER_FRAME ROLL_FIRST_ELEMENT + 1
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


static struct tBowlingFrame currentFrame;
static struct tBowlingFrame previousFrame;

static struct tGame
{
    int runningTotal;
    int frameNumber;
    int rollInFrame;
}gameState;

static const struct tGame gameInitState = 
{
    .runningTotal = 0,
    .frameNumber = FRAME_FIRST_ELEMENT,
    .rollInFrame = ROLL_FIRST_ELEMENT
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
static void markScoreCardForFrameResult(struct tBowlingFrame frame);
static int getReportCardRollOffset(void);
static int getReportCardFrameResultOffset(struct tBowlingFrame frame);
static void prepareForNextRoll(void);


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
    gameState = gameInitState;
    BWLNGFRMS_Init(&currentFrame);
    BWLNGFRMS_Init(&previousFrame);
}

void SCRNG_DrawScoreCard( char* scoreCard )
{
    sprintf(scoreCard, "%s", currentCard);
}

void SCRNG_Roll(int pins)
{

    markScoreCardForRoll(pins);

    if ( BWLNGFRMS_FrameIsASpare(previousFrame) )
    {
        BWLNGFRMS_CalculateBonus(&previousFrame, currentFrame);
        markScoreCardForFrameResult(previousFrame);
    }

    if ( frameIsComplete() )
    {
        gameState.runningTotal += 
            currentFrame.secondRowScore
            + currentFrame.firstRowScore;

        if ( ! BWLNGFRMS_FrameIsASpare(currentFrame) )
        {
            markScoreCardForFrameResult(currentFrame);
        }

        progressToNextFrame();
    }
    else
    {
        prepareForNextRoll();
    }

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
    previousFrame = currentFrame;

    gameState.frameNumber++;
    gameState.rollInFrame = FRAME_FIRST_ELEMENT;

    BWLNGFRMS_Init(&currentFrame);

}

static bool frameIsComplete(void)
{
    return ( ROLLS_PER_FRAME == gameState.rollInFrame);
}

static void markScoreCardForRoll(int pins)
{
    switch (gameState.rollInFrame)
    {
        case 1:
            currentFrame.firstRowScore = pins;
        break;
        case 2:
            currentFrame.secondRowScore = pins;
        default:
        break;
    }
    currentCard[getReportCardRollOffset()] = FRMTNG_PinsToChar(pins, BWLNGFRMS_FrameIsASpare(currentFrame));
}

static void resetCard(void)
{
    sprintf(currentCard, "%s", blankCard);
}


static int getReportCardRollOffset()
{
    int offset = SIZE_OF_ROLL_SLOTS / 2 ; //offset of first roll
    //offset for additional roll
    offset += (gameState.rollInFrame - ROLL_FIRST_ELEMENT) * SIZE_OF_ROLL_SLOTS;
    //offset for frame
    offset += (gameState.frameNumber - FRAME_FIRST_ELEMENT) * SIZE_OF_FRAME;

    return offset;
}

static void markScoreCardForFrameResult(struct tBowlingFrame frame)
{
    int totalScore = gameState.runningTotal + frame.bonus;
    FRMTNG_ScoreToString( &currentCard[getReportCardFrameResultOffset(frame)] , totalScore );
}

static int getReportCardFrameResultOffset(struct tBowlingFrame frame)
{
    int offset = sizeof(blankCard)/2 + SIZE_OF_FRAME/2;
    int targetFrame = gameState.frameNumber;

    if ( BWLNGFRMS_FrameIsASpare(frame) )
    {
        /*
        This frame was a spare so we're putting in its score halfway through
        the frame after.
        */
        targetFrame -=1;
    }
    offset += (targetFrame - FRAME_FIRST_ELEMENT ) * SIZE_OF_FRAME;

    return offset;
}

static void prepareForNextRoll(void)
{
    gameState.rollInFrame++;
}