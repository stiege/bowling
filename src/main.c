#include "main.h"
#include "stdbool.h" //bool

#include "game.h"



/*
   _____ __________.___
  /  _  \\______   \   |
 /  /_\  \|     ___/   |
/    |    \    |   |   |
\____|__  /____|   |___|
        \/
*/
void MAIN_Init( void )
{
    GME_Init();
}

void MAIN_DrawScoreCard( char* scoreCard )
{
    GME_DrawScoreCard(scoreCard);
}

void MAIN_Roll(int pins)
{

    GME_ProcessRoll(pins);

    GME_CheckAndUpdatePreviousFrame();

    if ( GME_FrameIsComplete() )
    {
        GME_UpdateTotal();

        if ( GME_FrameScoreKnown() )
        {
            GME_WriteFrameScore();
        }

        GME_NextFrame();
    }
    else
    {
        GME_NextRoll();
    }

}