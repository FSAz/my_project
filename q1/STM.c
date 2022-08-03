#include "HT66F004.h"
#include "STM.h"

void STM_Init()
{
    #if STM_TIMER_COUNTER_MODE
   _pt0m1 = 1; _pt0m0 = 1;
    #endif

    #if STM_FH_DIV64
   	_pt0ck2 = 0; _pt0ck1 = 1; _pt0ck0 = 1;
    #endif

    #if STM_CCRA_MATCH	
    _pt0cclr = 1;
    #endif
}