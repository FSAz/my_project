#include "Sys_clock.h"
#include "HT66F004.h"

void SysClockDivision_Init()
{
    #if SYSCLOCK_FH
    _hlclk = 1;										//set Fsys as FH
    #endif
}