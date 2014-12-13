#include "formatting.h"

static char intToChar(int val);

void FRMTNG_ScoreToString( char *writeTo , const int score )
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

char FRMTNG_PinsToChar(int pins, bool isSpare)
{
    char writeOut = 0;
    if ( isSpare )
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