#include "unity.h"
#include "formatting.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_ScoreToStringLT10(void)
{
    char output[10] = {0};
    char *const str_ptr = &output[5];
    FRMTNG_ScoreToString(str_ptr, 5);
    TEST_ASSERT(output[4] == 0);
    TEST_ASSERT(output[5] == '5');
    TEST_ASSERT(output[6] == 0);
}

void test_ScoreToStringLT100(void)
{
    char output[10] = {0};
    char *str_ptr = &output[5];
    FRMTNG_ScoreToString(str_ptr, 53);
    TEST_ASSERT(output[4] == '5');
    TEST_ASSERT(output[5] == '3');
    TEST_ASSERT(output[6] == 0);
}

void test_ScoreToStringLT1000(void)
{
    char output[10] = {0};
    char *str_ptr = &output[5];
    FRMTNG_ScoreToString(str_ptr, 253);
    TEST_ASSERT(output[4] == '2');
    TEST_ASSERT(output[5] == '5');
    TEST_ASSERT(output[6] == '3');
}
