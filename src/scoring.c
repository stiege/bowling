#include "scoring.h"
#include "stdio.h"

void SCRNG_DrawScoreCard( char* scoreCard )
{
    const char stringReturn[] = 
"|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |\n"
"|       |       |       |       |       |       |       |       |       |       |\n";

    sprintf(scoreCard, "%s", stringReturn);
}

void SCRNG_Roll(int pins)
{

}