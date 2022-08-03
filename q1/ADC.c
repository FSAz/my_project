#include "HT66F004.h"
#include "ADc.h"

void ADC_Init()
{
    #if ADC_CLOCK_FSYS_DIV8	
    _sacks2 = 0; _sacks1 = 1; _sacks0 = 1;
    #endif

    #if ADC_REF_VOLTAGE_VDD
    _savrs3 = 0; _savrs2 = 0; _savrs1 = 0; _savrs0 = 0;
    #endif

    #if ADC_VALUE_ALIGN_RIGHT
    _adrfs = 1;
    #endif
}

void ADC_SelectChannel(uint8_t ADC_chanel)
{
    //clear ADC chanel
	_sadc0 &= 0b11111000;
		
	//selection external ADC chanel
	_sadc0 |= ADC_Chanel;

    if(!ADC_chanel)
      _pbs0 = 1;

}
