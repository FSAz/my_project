#include "WDT.h"
#include "HT66F004.h"

void WDT_init()
{
#if	WDT_OF_TIME_2_8
	_ws2=0;_ws1=0;_ws0=0;
	
#elif	WDT_OF_TIME_2_9
	_ws2=0;_ws1=0;_ws0=1;
	
#elif	WDT_OF_TIME_2_10
	_ws2=0;_ws1=1;_ws0=0;
	
#elif	WDT_OF_TIME_2_11
	_ws2=0;_ws1=1;_ws0=1;
	
#elif	WDT_OF_TIME_2_12
	_ws2=1;_ws1=0;_ws0=0;
	
#elif	WDT_OF_TIME_2_13
	_ws2=1;_ws1=0;_ws0=1;
	
#elif	WDT_OF_TIME_2_14
	_ws2=1;_ws1=1;_ws0=0;
	
#elif	WDT_OF_TIME_2_15
	_ws2=1;_ws1=1;_ws0=1;
#endif
}