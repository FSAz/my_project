#include "HT66F004.h"

#define		TB_ENABLE()					_tbon = 1
#define		TB_DISABLE()				_tbon = 0

#define		TB0_ISR_ENABLE()			_tb0e = 1
#define		TB0_ISR_DISABLE()			_tb0e = 0
#define		TB0_SET_FLAG()				_tb0f = 1
#define		TB0_CLEAR_FLAG()			_tb0f = 0
#define		TB0_FLAG()					_tb0f

//128us
#define		TB_CLOCK_FSYS_DIV4		1
#define		TB0_Period_2_8			1   