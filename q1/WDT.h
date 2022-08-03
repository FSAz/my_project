#include "HT66F004.h"

#define	WDT_ENABLE()			_we4=0;_we3=1;_we2=0;_we1=1;_we0=0
#define	WDT_DISABLE()			_wdtc = 0b10101000

//WDT overflow time select
#define	WDT_OF_TIME_2_8			0	//WDT overflow time 2^8 *Tlirc
#define	WDT_OF_TIME_2_9			0	//WDT overflow time 2^9 *Tlirc
#define	WDT_OF_TIME_2_10		0	//WDT overflow time 2^10*Tlirc
#define	WDT_OF_TIME_2_11		1	//WDT overflow time 2^11*Tlirc
#define	WDT_OF_TIME_2_12		0	//WDT overflow time 2^12*Tlirc
#define	WDT_OF_TIME_2_13		0	//WDT overflow time 2^13*Tlirc
#define	WDT_OF_TIME_2_14		0	//WDT overflow time 2^14*Tlirc
#define	WDT_OF_TIME_2_15		0	//WDT overflow time 2^15*Tlirc
/****** end of WDT overflow time select ******/
