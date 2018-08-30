#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "stdlib.h"
#include "oled.h" 
#include "adc.h"
#include "timer.h"

int main(void)
{ 
    uint16_t Advalue;   //AD转换值
    uint16_t i,j;
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
    Adc_Init();             //初始化ADC
	OLED_Init();			//初始化OLED  
    TIM3_Int_Init(5000-1, 8400-1);  //定时器时钟84M，分频系数8400，所以84M/8400=1000Khz的计数频率，计数5000次为500ms   
	OLED_ShowString(0,0,"   Timer TEST",16);

    while(1)
    {   

            
    }
    
}


	

 



