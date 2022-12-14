#include "HT66F004.h"
#include "HT8_Type.h"

volatile u32 adc_val;
volatile u16 adc_avg;
volatile u8  last_zc;
volatile u16 trig;
volatile u16 lastper;
volatile u8  adc_ch1 = 0x01;
volatile u8  cntr;
volatile u16 i = 0x0000;  //a 16 bits integer in order to shift adc register


void main()
{
    //WDT_init()
	_ws2=0;
	_ws1=1;
	_ws0=1;
	
    //WDT_ENABLE();
	_we4=1;
	_we3=0;
	_we2=1;
	_we1=0;
	_we0=1;
	
    _hlclk = 1; //set system clock at FH

    //input for ZC
    _pbc0 = 1;

    //output for driving the gate
    _pcc0 = 0;
    _pc0 = 0;

    //two outputs for two LEDs
    _pcc1 = 0;  //LED1
    _pc1 = 1;

    _pcc2 = 0;  //LED2
    _pc2 = 1;

    last_zc = 0;//_pb1;

    trig = 625;  //5ms time for driving triac
	lastper = 626;  
	adc_val = 0;
	cntr = 8;

	//TimeBase_Init();
	_tbck = 1;   //TB_CLOCK_FSYS_DIV4

	//TB0_Period_2_8
	_tb02 = 0;  
	_tb01 = 0; 
	_tb00 = 0;

	//STM_Init()
	//STM_TIMER_COUNTER_MODE
    _pt0m1 = 1;  
	_pt0m0 = 1;

	//STM_FH_DIV64
	_pt0ck2 = 0;  
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
	
    //start ADC
	_start = 0;  
	_start = 1;
	_start = 0;
	
    while(1)
    {
        if(!cntr)
		{
			if(adc_avg<1024)
            {
               lastper = 939;   //fire angle on 7.5ms
               _pc1 = 1;
               _pc2 = 1;
            }
            else if(adc_avg<2048)
            {
                lastper =689;   //fire angle on 5.5ms
                _pc1 = 0;
                _pc2 = 1;
            }
            else if(adc_avg<3072)
            {
                lastper = 439;   //fire angle on 3.5ms
                _pc1 = 1;
                _pc2 = 0;
            }
            else
            {  
               lastper = 189;  ////fire angle on 1.5ms
               _pc1 = 0;
               _pc2 = 0;
            }
		}
        


	    //refreshing WDT 
        //asm("CLR WDT");	
    }
}

/* 128us Time Interval For Detecting ZC and Driving Triac. */
void __attribute((interrupt(0x08))) TB0_ISR(void)
{

	if(_pb0 ^ last_zc)
	{
		//_tb0f = 0;(clear TB0 flag) it automatcally happens
		last_zc = _pb0;	
		if((trig > lastper) && _pb0)
			trig -= 10;  //Trig_Var.Step;	
						
		else if(_pb0)
			trig += 10;  //Trig_Var.Step;	

		_ptm0al = trig & 0x00ff;
		_ptm0ah = trig >> 8;
	
		_pt0on = 1;  //enable STM
	}
	
	
	if(cntr && (_adbz == 0))
	{
		adc_val += ((u16)_sadoh <<8) | _sadol;			//(((i | _sadoh)<<8) | _sadol);
		cntr--;
		_start = 0;  
	    _start = 1;
	    _start = 0;
	}
	else
	{
		cntr = 8;
		adc_avg = adc_val >> 3;
		adc_val = 0;
		_start = 0;  
	    _start = 1;
	    _start = 0;
	}		
}

void __attribute((interrupt(0x10))) Timer_ISR(void)
{				
	if(!_pc0)
	{
		_pc0 = 1;
		_ptma0f = 0;  //clear timer flag
		_pt0on  = 0;  //disable the timer

		//make a narrow pulse
		_ptm0al = 12;
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
	//_adf = 0;(clear adc flag) it automatcally happens
	

}
