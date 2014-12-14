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

void test_Strike(void)
{
    const int frame = FRAME_FIRST_ELEMENT;
    int roll = ROLL_FIRST_ELEMENT;
    const int firstRollPins = 10;

    SCRCRD_WriteStrike_Expect(frame);
    GME_ProcessRoll(firstRollPins);

    TEST_ASSERT( GME_FrameIsComplete() );
    GME_UpdateTotal();
    TEST_ASSERT( !GME_FrameScoreKnown() );

    GME_NextFrame();

    SCRCRD_WriteRoll_Ignore();
    GME_ProcessRoll(3);

    TEST_ASSERT( !GME_FrameIsComplete() );

    roll++;
    GME_NextRoll();

    SCRCRD_WriteRoll_Ignore();
    GME_ProcessRoll(2);

    SCRCRD_WriteScoreForFrame_Expect(1,15);
    GME_CheckAndUpdatePreviousFrame();

    GME_UpdateTotal();

    TEST_ASSERT( GME_FrameIsComplete() );
    TEST_ASSERT( GME_FrameScoreKnown() );
    SCRCRD_WriteScoreForFrame_Expect(2, 20);
    GME_WriteFrameScore();

}

void test_MultiStriks(void)
{
    int frame = FRAME_FIRST_ELEMENT;

    /*strike*/
    SCRCRD_WriteStrike_Expect(frame++);
    GME_ProcessRoll(10);
    TEST_ASSERT( GME_FrameIsComplete() );
    GME_UpdateTotal();
    GME_NextFrame();

    /*2nd strike*/
    SCRCRD_WriteStrike_Expect(frame++);
    GME_ProcessRoll(10);
    TEST_ASSERT( GME_FrameIsComplete() );
    GME_UpdateTotal();
    GME_NextFrame();

    /*frame 3, first roll*/
    SCRCRD_WriteRoll_Ignore();
    GME_ProcessRoll(2);

    /*First strike score is now known*/
    SCRCRD_WriteScoreForFrame_Expect(1,22);
    GME_CheckAndUpdatePreviousFrame();

    /*frame 3, second roll*/
    GME_NextRoll();
    GME_ProcessRoll(2);

    /*Second strike score is now known*/
    SCRCRD_WriteScoreForFrame_Expect(2,36);
    GME_CheckAndUpdatePreviousFrame();

}
