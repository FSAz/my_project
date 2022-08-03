#include "HT66F004.h"
#include "TB.h"

void TimeBase_Init()
{
    #if TB_CLOCK_FSYS_DIV4
    _tbck = 1;
    #endif
    #if TB0_Period_2_8
    _tb02=1; _tb01=1; _tb00=1;
    #endif
}