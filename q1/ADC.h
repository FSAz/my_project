#include "HT66F004.h"

#define		ADC_ENABLE()			_enadc = 1
#define		ADC_DISABLE()			_enadc = 0

#define		ADC_ISR_ENABLE()		_ade = 1
#define		ADC_ISR_DISABLE()		_ade = 0
#define		ADC_CLEAR_ISR_FLAG()	_adf = 0
#define		ADC_SET_ISR_FLAG()		_adf = 1

#define		ADC_START()				{_start = 0; _start = 1; _start = 0;}

#define		ADC_CLOCK_FSYS_DIV8			1

#define		ADC_REF_VOLTAGE_VDD			1

#define		ADC_VALUE_ALIGN_RIGHT		1

#if ADC_VALUE_ALIGN_RIGHT
    #define	ADC_READ_VALUE()	(((0x0000) | (_sadoh))<<8) | _sadol
#endif