#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

#include "stdlib.h"
#include "oled.h" 
#include "adc.h"

int main(void)
{ 
    uint16_t Advalue;   //AD转换值
	delay_init(168);		  //初始化延时函数
	LED_Init();		        //初始化LED端口
    Adc_Init();             //初始化ADC
	OLED_Init();			//初始化OLED  

	OLED_ShowString(0,0,"   ADC TEST",16);
    OLED_ShowString(0,16,"Advalue:",16);
    while(1)
    {
        Advalue = Get_Adc_Average(0,10);
        OLED_ShowNum(64, 16, Advalue, 4, 16);
        LED2 = 0;
        LED3 = 1;
        delay_ms(500);
        LED2 = 1;
        LED3 = 0;
        delay_ms(500);
     }
}


	

 



