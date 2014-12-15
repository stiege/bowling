#include "formatting.h"

void FRMTNG_ScoreToString( char *const writeTo , int score )
{
    int hundreds = score/100;
    int tens = (score - hundreds * 100)/10;
    int ones = score - tens * 10 - hundreds * 100;

    if (score < 10)
    {
        *writeTo = FRMTNG_IntToChar(ones);
    }
    else if (score < 100)
    {
        *(writeTo - 1) = FRMTNG_IntToChar(tens);
        *writeTo = FRMTNG_IntToChar(ones);
    }
    else if (score < 1000)
    {
        *(writeTo - 1) = FRMTNG_IntToChar(hundreds);
        *writeTo = FRMTNG_IntToChar(tens);
        *(writeTo + 1) = FRMTNG_IntToChar(ones);
    }
}

char FRMTNG_IntToChar(int val)
{
    return (val + 0x30);
}