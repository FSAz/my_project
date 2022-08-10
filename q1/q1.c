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
static volatile unsigned char adc_ch1 = 0x01;
static volatile unsigned char cntr;
static volatile unsigned int i = 0x0000;

void main()
{
    //WDT_init()
	_ws2=0;
	_ws1=1;
	_ws0=1;
	
    //WDT_ENABLE();
	_we4=0;
	_we3=1;
	_we2=0;
	_we1=1;
	_we0=0;
	
    _hlclk = 1; //set system clock at FH

    //input for potentiometer
    _pbc0 = 1;

    //input for ZC
    _pbc1 = 1;

    //output for driving the gate
    _pcc0 = 0;
    _pc0 = 0;

    //two outputs for two LEDs
    _pcc1 = 0;  //LED1
    _pc1 = 0;

    _pcc2 = 0;  //LED2
    _pc2 = 0;

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
	_tb0e = 1;
	
	_mf0f = 0;	//clear multi-function 0 interrupt flag
	_mf0e = 1;  //enable multifunction interrupt

	_ptma0f = 0;	//clear STM CCRA interrupt flag
	_ptma0e = 1;	//enable STM CCRA interrupt
	
	_emi = 1;  //enable global interrupt

	_tbon = 1;  //enable TimeBase 

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

	//clear ADC chanel
	_sadc0 &= 0b11111000;

	//selection external ADC chanel
	_sadc0 |= adc_ch1;

	_pbs1 = 1; //AN1

	_enadc = 1;  //enable ADC
	
	//enable ADC interrupt
	_adf = 0;
	_ade = 1;

	_start = 0;  //start ADC
	_start = 1;
	_start = 0;
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
		{
		  _start = 0;  //start ADC
	      _start = 1;
	      _start = 0;
		}

		if(!((_we4=0) && (_we3=1) &&(_we2=0) && (_we1=1) && (_we0=0)))
		{
			_we4=0;  //enable WDT
			_we3=1;
			_we2=0;
			_we1=1;
			_we0=0;
		}
    }
}

/* 128us Time Interval For Detecting ZC and Driving Triac. */
void __attribute((interrupt(0x08))) TB0_ISR(void)
{

	if(_pb1 ^ last_zc)
	{
		_tb0f = 0;  //clear TB0 flag
		last_zc = _pb1;	
		if((trig > lastper) && _pb1)
			trig -= 10;  //Trig_Var.Step;	
						
		else if(_pb1)
			trig += 10;  //Trig_Var.Step;	

		_ptm0al = trig & 0x00ff;
		_ptm0ah = trig >> 8;
	
		_pt0on = 1;  //enable STM
	}		
}

//128 us
void __attribute((interrupt(0x10))) Timer_ISR(void)
{				
	if(!_pc0)
	{
		_pc0 = 1;
		_ptma0f = 0;  //clear timer flag
		_pt0on  = 0;  //disable the timer
		_ptm0al = 1;
		_ptm0ah = 0;
		_pt0on  = 1;  //enable the timer
	}
	else
	{
		_pc0 = 0;
		_ptma0f = 0; 
		_pt0on  = 0;
	}
}



void __attribute((interrupt(0x18))) ADC_ISR(void)
{
	_adf = 0;	//clear adc flag
	
	if(cntr)
	{
		adc_val = (adc_val) + (((i | _sadoh)<<8) | _sadol);
		cntr--;
	}
	else
	{
		cntr = 8;
		adc_val = adc_val >> 3;
	}
	    
}
