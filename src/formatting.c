#include "formatting.h"

void FRMTNG_ScoreToString( char *const writeTo , int score )
{
    int tens = score/10;
    int ones = score - tens * 10;

    if (score < 10)
    {
        *writeTo = FRMTNG_IntToChar(ones);
    }
    else if (score < 100)
    {
        *(writeTo - 1) = FRMTNG_IntToChar(tens);
        *writeTo = FRMTNG_IntToChar(ones);
    }
}

char FRMTNG_IntToChar(int val)
{
    return (val + 0x30);
}