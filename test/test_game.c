#include "unity.h"
#include "game.h"
#include "projectconfig.h"

#include "bowlingframes.h"
#include "formatting.h"
#include "mock_scorecard.h"

void setUp(void)
{
    SCRCRD_Init_Expect();
    GME_Init();
}

void tearDown(void)
{
}

void test_SimpleGame(void)
{
    const int frame = FRAME_FIRST_ELEMENT;
    int roll = ROLL_FIRST_ELEMENT;
    const int firstRollPins = 3;
    const int secondRollPins = 2;

    SCRCRD_WriteRoll_Expect(frame,roll,firstRollPins);
    GME_ProcessRoll(firstRollPins);

    roll++;
    GME_NextRoll();

    SCRCRD_WriteRoll_Expect(frame,roll,secondRollPins);
    GME_ProcessRoll(secondRollPins);

    TEST_ASSERT( GME_FrameIsComplete() );
    TEST_ASSERT( GME_FrameScoreKnown() );
    GME_UpdateTotal();

    SCRCRD_WriteScoreForFrame_Expect(frame,firstRollPins+secondRollPins);
    GME_WriteFrameScore();

}
