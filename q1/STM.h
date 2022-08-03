#include "HT66F004.h"

#define	STM_ENABLE()			(_pt0on  = 1)
#define	STM_DISABLE()			(_pt0on  = 0)
#define	STM_CCRA_ISR_ENABLE()	(_ptma0e = 1)
#define	STM_CCRA_ISR_DISABLE()	(_ptma0e = 0)
#define STM_SET_FLAG_A()		(_ptma0f = 1)
#define STM_CLEAR_FLAG_A()		(_ptma0f = 0)
#define	STM_PAUSE()				(_ptm0c0 = 1)
#define	STM_RUN()				(_ptm0c0 = 0)
#define	STM_GET_CCRA_FLAG()		 _ptma0f

#define	STM_TIMER_COUNTER_MODE			1

#define		STM_FH_DIV64				1

#define		STM_CCRA_MATCH				1
