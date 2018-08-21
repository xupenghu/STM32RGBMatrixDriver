
#include <string.h>
#include "bsp_matrix.h"
#include "delay.h"
#include "image.h"
//#include "intrins.h"
//#include "stm32f10x.h"



/*
 *  PB9   R0  | G0   PA12
 *  PB8   B0  | GND  GND
 *  PB7   R1  | G1   PA11
 *  PB6   B1  | GND   /
 *  PB5   A   | B    PA10
 *  PB4   C   | D    PA8
 *  PB3  CLK  | LE   PA9
 *  PA15  OE  | GND   /
 *
 * 
 */

u8 Control = 1;
/*
*/
/*
 *    7    6    5    4     3     2       1        0
 *                       left  right  灰度模式  字符显示
 */


//const u16 waits[] = { 5, 10, 20, 40, 80, 160, 320, 640, 1280, 2560 };
//const u16 waits[] =   { 1, 5, 20, 40, 80, 160, 280, 640, 980};
//const u16 waits[] =   { 1, 2, 6, 24, 80, 160, 340, 760, 3500};
const u16 waitb[] =   { 0, 1, 4, 32, 128, 256, 512, 1024, 2048};

const int waits[] = { 5, 10, 20, 40, 80, 160, 320, 640 };

const u8 scan = MATRIX_HEIGHT / 2;
uint32_t bufferA[MATRIX_SIZE];
//uint32_t bufferB[MATRIX_SIZE];



void setupRGBMatrixPorts(void)
{
	//LED初始化 led OE 
	GPIO_InitTypeDef GPIO_InitStructure;
  /* enable GPIO port Clock */

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12  ;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15| GPIO_Pin_9 | GPIO_Pin_8 |  GPIO_Pin_7 |   GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_14 | GPIO_Pin_12 ;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOB, GPIO_Pin_15);	  // OE初始化为不显示

}

/**
  * @brief  点阵屏初始化函数
  * @param  None
  * @retval None
  */
void Matrix_Init(void)
{
	setupRGBMatrixPorts();
}


/**
  * @brief  sets the row on the row gpio ports
  * @param  None
  * @retval None
  */
void setRow(u8 row) 
{
	// todo: perhaps a lookup table could give us a tiny boost here.
	if ((row & 0x01)) 
	{
		m1_a_0;	//MTX2_PORTc->BSRR  = MTX2_PA;
	}
	else 
	{
		m1_a_1;	//MTX2_PORTc->BRR = MTX2_PA;
	}

	if ((row & 0x02)) 
	{
		m1_b_1;	//MTX2_PORTc->BSRR = MTX2_PB;
	}
	else 
	{
		m1_b_0;	//MTX2_PORTc->BRR = MTX2_PB;
	}

	if ((row & 0x04)) 
	{
		m1_c_1;	//MTX2_PORTc->BSRR = MTX2_PC;
	}
	else 
	{
		m1_c_0;	//MTX2_PORTc->BRR = MTX2_PC;
	}
	
	if ((row & 0x08)) 
	{
		m1_d_1;	//MTX2_PORTc->BSRR = MTX2_PC;
	}
	else 
	{
		m1_d_0;	//MTX2_PORTc->BRR = MTX2_PC;
	}
}

/**
  * @brief  loads rgb1 and rgb2 gpio ports with the given bitplane
  * @param  None
  * @retval None
  */
void setRGB(uint32_t rgb1, uint32_t rgb2, uint8_t plane) 
{
	// using bitshifting seems to be faster due to gcc optimization
	// than using a bitmask lookup table here.
	
	if (rgb1 & (1 << plane))        
	{
		m1_r0_1;	//MTX2_PORTd->BSRR = MTX2_PR0;
	}
	else                            
	{
		m1_r0_0;	//MTX2_PORTd->BRR  = MTX2_PR0;
	}

	if (rgb1 & (1 << (plane + 8))) 	
	{
		m1_g0_1;	//MTX2_PORTd->BSRR = MTX2_PG0;
	}
	else                            
	{
		m1_g0_0;	//MTX2_PORTd->BRR  = MTX2_PG0;
	}

	if (rgb1 & (1 << (plane + 16))) 
	{
		m1_b0_1;	//MTX2_PORTd->BSRR = MTX2_PB0;
	}
	else                           
	{
		m1_b0_0;	//MTX2_PORTd->BRR  = MTX2_PB0;
	}

	if (rgb2 & (1 << plane))        
	{
		m1_r1_1;	//MTX2_PORTd->BSRR = MTX2_PR1;
	}
	else                            
	{
		m1_r1_0;	//MTX2_PORTd->BRR  = MTX2_PR1;
	}

	if (rgb2 & (1 << (plane + 8))) 
	{
		m1_g1_1;	//MTX2_PORTd->BSRR = MTX2_PG1;
	}
	else                            
	{
		m1_g1_0;	//MTX2_PORTd->BRR  = MTX2_PG1;
	}

	if (rgb2 & (1 << (plane + 16))) 
	{
		m1_b1_1;	//MTX2_PORTd->BSRR = MTX2_PB1;
	}
	else                            
	{
		m1_b1_0;	//MTX2_PORTd->BRR  = MTX2_PB1;
	}
	
}

int drawPixel(s8 x, s8 y, u32 Color)
{
	s8 hwx,hwy;
	u8 tmp  ;

	tmp = y%2;
	if(tmp)
	{
		y = y -1;
	}
	else
	{
		y = y + 1;
	}

	//屏幕反转的  暂时没用到
	#if 0
	if(y >= 0 && y < 8)
	{
		hwy = y + 16;
	}
	else if(y >= 8 && y < 16)
	{
		hwy = y - 8;
	}
	else if(y >= 16 && y < 24)
	{
		hwy = y + 8;
	}
	else if(y >= 24 && y < 32)
	{
		hwy = y - 16;
	}

#endif
	
	
	if(x < 0 || y < 0 || x > MATRIX_WIDTH || y > MATRIX_HEIGHT) return -1;
	
	hwx = x ;
	hwy = 31-y;
	
	bufferA[MATRIX_WIDTH*hwy + hwx] = Color;
	return 0;

}



/**
 * strobes / shows a line for a n*nop amount of time.
 */
void showLine(u16 amount) 
{
	u16 c;
	DISP2_ON;
	for (c=0; c<amount; c++);// __nop();//__NOP /*asm("nop")*/;
	DISP2_OFF;

}

/**
  * @brief  灰度显示主函数 RGB888
  * @param  None
  * @retval None
  */
void display_PWM(uint32_t buffer[]) 
{
	u8 s = 0;
	u32 offset1 = 0;
	u32 offset2 = 0;
	u8 x = 0, plane = 0;
	
	for (s=0; s < scan; s++)
	{

		offset1 = MATRIX_WIDTH * (s);
		offset2 = MATRIX_WIDTH * (s+scan);		
		setRow(s);
		
		for (plane=0; plane < 8; plane ++) 
		{
			m1_le_0;
			for (x=0; x<MATRIX_WIDTH; x++) 
			{
				setRGB(buffer[offset1+x], buffer[offset2+x], plane);
		//		setRGB(0x00ff00ff, 0x0000ff0f, plane);
				CLK2_TOGGLE;
			}
			STROBE2;//锁存
			showLine(waits[plane]);	
		}
		
	}
}


 void lcd_draw_hline(u8 x1, u8 x2, u8 y, u32 color)
{
	u32 x;
	
	for (x = x1; x<x2; x++)
	{
		drawPixel(x, y, color);
	}
}


 void lcd_draw_vline(u8 x, u8 y1, u8 y2)
{
	u8 y;
	u32 color;
	
	for (y = y1; y < y2; y++)
	{
		color = (gImage_image[(64*y+x)*3]<<16) + (gImage_image[(64*y+x)*3+1]<<8) + (gImage_image[(64*y+x)*3+2]);
		drawPixel(x, y, color);
		//delay_ms(1000);
	}
}


void lcd_draw_image(const unsigned char *pic)
 {
 	const unsigned char *p = pic ;
	u32 x, y;

	for (y=0; y<32; y++)     
	{
		for (x=0; x<64; x++)     
		{
			bufferA[(64*y+x)] = (gImage_image[(64*y+x)*3]<<16) + (gImage_image[(64*y+x)*3+1]<<8) + (gImage_image[(64*y+x)*3+2]);
		}
	}

}






