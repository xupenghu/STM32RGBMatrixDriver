#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "stdlib.h"
#include "oled.h" 
#include "adc.h"
#include "timer.h"
#include "adc_timer_dma.h"


int main(void)
{ 
    uint32_t Advalue;   //AD转换值
    float v;
    uint16_t i,j;
    uint8_t res;
	delay_init(168);		  //初始化延时函数
    uart_init(115200);
	LED_Init();		        //初始化LED端口
    adc_timer_dma_init();             //初始化ADC
	OLED_Init();			//初始化OLED  
    TIM3_Int_Init(5000, 8400-1);  //定时器时钟84M，分频系数8400，所以84M/8400=10Khz的计数频率，计数5000次为500ms   
	OLED_ShowString(0,0," Timer ADC DMA ",16);
    OLED_ShowString(0,16,"Advalue:",16);
    while(1)
    {   
        res = DMA_GetCurrentMemoryTarget(DMA2_Stream0) ;
        if(res)
        {
            for(i = 0 ; i < 256; i ++)
            {
                Advalue += ADC3ConvertedValue1[i];
            }
            Advalue >>= 8;
        }
        else
         {
            for(i = 0 ; i < 256; i ++)
            {
                Advalue += ADC3ConvertedValue2[i];
            }
            Advalue >>= 8;
        }           
         v = Advalue/4.0960 * 3.3;
        Advalue = (uint32_t)v;
         OLED_ShowNum(64, 16, Advalue, 4, 16);   
        Advalue = 0;
    }
    
}


	

 



