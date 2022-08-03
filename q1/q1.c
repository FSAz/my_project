#include "HT66F004.h"
#include "WDT.h"
#include "TB.h"
#include "STM.h"
#include "inter.h"
#include "ADC.h"

static volatile unsigned int adc_val;
static volatile unsigned char last_zc;
static volatile unsigned int trig;
static volatile unsigned int lastper;
static volatile unsigned char adc_ch0 = 0x00;
static volatile unsigned char cntr;

void main()
{
    //WDT_init()
	_ws2=0;
	_ws1=1;
	_ws0=1;
	
    WDT_ENABLE();
	
    _hlclk = 1; //set system clock at FH

    //input for potentiometer
    _pbc0 = 1;

    //input for ZC
    _pbc1 = 1;

    //output for driving the gate
    _pcc0 = 0;
    _pc0 = 0;

    //two outputs for two LEDs
    _pac3 = 0;  //LED1
    _pa3 = 0;

    _pac4 = 0;  //LED2
    _pa4 = 0;

    last_zc = _pb1;

    trig = 625;  //5ms time for driving triac
	lastper = 626;  
	adc_val = 0;
	cntr = 8;
	//TimeBase_Init();
	_tbck = 1;   //TB_CLOCK_FSYS_DIV4

	_tb02=1;  //TB0_Period_2_8
	_tb01=1; 
	_tb00=1;

	//STM_Init()
    _pt0m1 = 1;  //STM_TIMER_COUNTER_MODE
	_pt0m0 = 1;

	_pt0ck2 = 0;  //STM_FH_DIV64
	_pt0ck1 = 1; 
	_pt0ck0 = 1;

	 _pt0cclr = 1;  //STM_CCRA_MATCH

    //enable TimeBase0 interrupt
	TB0_ISR_ENABLE();
	
	MF0F_CLEAR_FLAG();		//clear multi-function 0 interrupt flag
	MF0E_ENABLE();	

	STM_CLEAR_FLAG_A();		//clear STM CCRA interrupt flag
	STM_CCRA_ISR_ENABLE();	//enable STM CCRA interrupt
	
	//enable global interrupt
	EMI_ENABLE();

	//enable TimeBase IP
	TB_ENABLE();

	//ADC_Init();	
	//ADC_CLOCK_FSYS_DIV8
	_sacks2 = 0;
	_sacks1 = 1;  
	_sacks0 = 1;     
	           	
    //ADC_REF_VOLTAGE_VDD
	_savrs3 = 0;
	_savrs2 = 0;  
	_savrs1 = 0; 
	_savrs0 = 0;  

    //ADC_VALUE_ALIGN_RIGHT
	_adrfs = 1;                                          

	//ADC channel select
	//ADC_SelectChannel(ADC_CH0);
	//clear ADC chanel
	_sadc0 &= 0b11111000;

	//selection external ADC chanel
	_sadc0 |= adc_ch0;

	_pbs0 = 1; //AN0

	//enable ADC IP
	ADC_ENABLE();
	
	//enable ADC interrupt
	ADC_CLEAR_ISR_FLAG();
	ADC_ISR_ENABLE();
	ADC_START();

    while(1)
    {
        if(!cntr)
		{
			if(adc_val<1024)
            {
               lastper = 939;
               _pa3 = 0;
               _pa4 = 0;
            }
            else if(adc_val<2048)
            {
                lastper =689;
                _pa3 = 1;
                _pa4 = 0;
            }
            else if(adc_val<3072)
            {
                lastper = 314;
                _pa3 = 0;
                _pa4 = 1;
            }
            else
            {  
               lastper = 189;
               _pa3 = 1;
               _pa4 = 1;
            }
		}
        
        if(_adbz == 0)
		  ADC_START();
    }
}

/* 128us Time Interval For Detecting ZC and Driving Triac. */
void __attribute((interrupt(0x08))) TB0_ISR(void)
{

	if(_pb1 ^ last_zc)
	{
		last_zc = _pb1;	
		if((trig > lastper) && _pb1)
			trig -= 10;  //Trig_Var.Step;	
						
		else if(_pb1)
			trig += 10;  //Trig_Var.Step;	

		_ptm0al = trig & 0x00ff;
		_ptm0ah = trig >> 8;
	
		STM_ENABLE();
	}		
}

//128 us
void __attribute((interrupt(0x10))) Timer_ISR(void)
{				
	if(!_pc0)
	{
		_pc0 = 1;
		STM_CLEAR_FLAG_A();
		STM_DISABLE();
		_ptm0al = 1;
		_ptm0ah = 0;
		STM_ENABLE();
	}
	else
	{
		_pc0 = 0;
		STM_CLEAR_FLAG_A();
		STM_DISABLE();
	}
}



void __attribute((interrupt(0x18))) ADC_ISR(void)
{
	ADC_CLEAR_ISR_FLAG();	
	
	if(cntr)
	{
		adc_val = (adc_val) + (ADC_READ_VALUE());
		cntr--;
	}
	else
	{
		cntr = 8;
		adc_val = adc_val >> 3;
	}
	    
}