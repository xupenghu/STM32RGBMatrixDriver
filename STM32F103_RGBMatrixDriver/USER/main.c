#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "bsp_matrix.h"
#include "adc.h"
#include "my_fft.h"
#include <string.h>
#include "image.h"

u8 fft_complet_flag ;


#define TOP_COLOR	0x00ff00ff


 int main(void)
 {	
	u16 x = 0 ,y =0;
	u8 table_power[64] = {0};
	u8 table_top[64] = {0};
	u8 table_cur[64] = {0};
	u8 table_count[64] = {0};
	u8 table_count_cur[64] = {0};
	u32 count;
	
	delay_init();	    	 //延时函数初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	LED_Init();		  	//初始化与LED连接的硬件接口
	ADC_DMA_TIM_Init();
	Matrix_Init();
	TIM3_Int_Init(250,7199);//10Khz的计数频率，计数到500为50ms  
//	my_fft_test();
	fft_complet_flag = 0;
	lcd_draw_image(gImage_image);
	delay_ms(1000);
	delay_ms(1000);
	delay_ms(1000);

	memset(table_cur,32,sizeof(table_cur));
	memset(table_top,32,sizeof(table_top));
	memset(table_power,32,sizeof(table_power));
	
	memset(bufferA,0,sizeof(bufferA));
	
	
  while(1)
	{

	//	if(fft_complet_flag)
		{
			count ++;
			fft_complet_flag = 0;
			my_fft();  //12ms
	//		memset(bufferA,0,sizeof(bufferA));
			for(x = 0 ; x < 64; x ++)
			{
				
				if(0 == x) table_power[x] = table_0_31[1]  ;
				else table_power[x] = (u8)table_0_31[x]  ; 
			//		table_power[x] = ((u8)(table_0_31[2*x]+ table_0_31[2*x+1]))>>1;
				
				if(table_power[x] > 31) table_power[x] = 31;
			}
			//if(count%10 == 4)
			{
				for(x = 0 ; x < 64; x ++)
				{			
					if( table_power[x] >= table_cur[x])
					{
						lcd_draw_vline(x,0,table_power[x]);
						table_cur[x] = table_power[x];
					}
					else 
					{
						table_count_cur[x] ++;
						if(table_count_cur[x] > 1)
						{
								table_count_cur[x] = 0;						
								drawPixel(x,table_cur[x],0x00000000);
								if(table_cur[x] !=0 )
									table_cur[x] --;
						}
						
					}
				}
			}
#if 1   //顶部柱子显示
			{
				for(x = 0 ; x < 64; x ++)
				{
					if(table_top[x] <= (table_power[x]+1))
					{
						drawPixel(x, table_top[x], 0);  //?????
						table_top[x] = table_power[x]+1;	//???????
						drawPixel(x, table_top[x], TOP_COLOR);  //????
					}
					else
					{
						table_count[x] ++;
						if(table_count[x] > 6)
						{
							table_count[x] = 0;
							drawPixel(x, table_top[x], 0);  //?????
							if(table_top[x] !=0 )
								table_top[x] --;	//????
							drawPixel(x, table_top[x], TOP_COLOR);  //????
						}
					}

				}	
			}		
#endif
		}

		
//		for (y = 0 ; y < MATRIX_HEIGHT; y++)
//		{
//			for (x =0; x < MATRIX_WIDTH; x++)
//			{	
//				drawPixel(x, y, 0xfffffff);
//				delay_ms(20);
//				LED0=!LED0;
//			}			
//		}
  
	}
}
