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
struct tFrameState 
{
    int firstRowScore;
    int secondRowScore;
    int bonus;
};

static struct tFrameState currentFrame;
static struct tFrameState previousFrame;

static const struct tFrameState frameInitState = 
{
    .firstRowScore = 0,
    .secondRowScore = 0,
    .bonus = 0,
};

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
static void markScoreCardForFrameResult(struct tFrameState frame);
static char pinsToChar(int pins);
static int getReportCardRollOffset(void);
static int getReportCardFrameResultOffset(struct tFrameState frame);
static void scoreToString( char *writeTo , int score );
static char intToChar();
static bool frameIsASpare( struct tFrameState frame );
static void prepareForNextRoll(void);
static void calculateBonus(void);


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
    currentFrame = frameInitState;
    previousFrame = frameInitState;
}

void SCRNG_DrawScoreCard( char* scoreCard )
{
    sprintf(scoreCard, "%s", currentCard);
}

void SCRNG_Roll(int pins)
{

    markScoreCardForRoll(pins);

    if ( frameIsASpare(previousFrame) )
    {
        calculateBonus();
        markScoreCardForFrameResult(previousFrame);
    }

    if ( frameIsComplete() )
    {
        gameState.runningTotal += 
            currentFrame.secondRowScore
            + currentFrame.firstRowScore;

        if ( ! frameIsASpare(currentFrame) )
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

    currentFrame = frameInitState;

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
    currentCard[getReportCardRollOffset()] = pinsToChar(pins);
}

static void resetCard(void)
{
    sprintf(currentCard, "%s", blankCard);
}

static char pinsToChar(int pins)
{
    char writeOut = 0;
    if ( frameIsASpare(currentFrame) )
    {
        writeOut = '/';
    }
    else
    {
        writeOut = intToChar(pins);
    }
    return writeOut;
}

static char intToChar(int val)
{
    return (val + 0x30);
}

static bool frameIsASpare( struct tFrameState scoreCard )
{
    return (scoreCard.firstRowScore + scoreCard.secondRowScore == 10);
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

static void markScoreCardForFrameResult(struct tFrameState frame)
{
    int totalScore = gameState.runningTotal + frame.bonus;
    scoreToString( &currentCard[getReportCardFrameResultOffset(frame)] , totalScore );
}

static int getReportCardFrameResultOffset(struct tFrameState frame)
{
    int offset = sizeof(blankCard)/2 + SIZE_OF_FRAME/2;
    int targetFrame = gameState.frameNumber;

    if ( frameIsASpare(frame) )
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

static void scoreToString( char *writeTo , int score )
{
    int tens = score/10;
    int ones = score - tens * 10;

    if (score < 10)
    {
        *writeTo = intToChar(ones);
    }
    else if (score < 100)
    {
        *(writeTo - 1) = intToChar(tens);
        *writeTo = intToChar(ones);
    }
}

static void prepareForNextRoll(void)
{
    gameState.rollInFrame++;
}

static void calculateBonus(void)
{
    previousFrame.bonus = currentFrame.firstRowScore;
}