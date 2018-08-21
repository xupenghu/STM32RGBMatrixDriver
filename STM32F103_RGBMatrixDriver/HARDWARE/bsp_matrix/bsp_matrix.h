
/**
  ******************************************************************************
  * @file    matrix_config.h
  * @author  Makt
  * @version V1.0
  * @date    2016-04-04
  * @brief   全彩点阵显示屏配置文件
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef __BSP_MAR_H
#define __BSP_MAR_H

#include "sys.h"

/*
 *  PB9   R0  | G0   PA12
 *  PB8   B0  | GND  GND
 *  PB7   R1  | G1   PA11
 *  PB6   B1  | GND   /
 *  PB5   A   | B    PA10
 *  PB14   C   | D    PA8
 *  PB12  CLK  | LE   PA9
 *  PA15  OE  | GND   /
 *
 * 
 */


// matrix1 rgb
#define m1_r0_0			PBout(9) = 0
#define m1_r0_1			PBout(9) = 1
#define m1_r1_0			PBout(7) = 0
#define m1_r1_1			PBout(7) = 1

#define m1_g0_0			PAout(12)	 = 0
#define m1_g0_1			PAout(12)	 = 1
#define m1_g1_0			PAout(11)	 = 0
#define m1_g1_1			PAout(11)	 = 1

#define m1_b0_0			PBout(8)	 = 0
#define m1_b0_1			PBout(8)	 = 1
#define m1_b1_0			PBout(6)	 = 0
#define m1_b1_1			PBout(6)	 = 1

// matrix1 abcd
#define m1_a_0			PBout(5) 		= 0
#define m1_a_1			PBout(5) 		= 1
#define m1_b_0			PAout(10) 	= 0
#define m1_b_1			PAout(10) 	= 1
#define m1_c_0			PBout(14)		= 0
#define m1_c_1			PBout(14)	  = 1
#define m1_d_0			PAout(8)		= 0
#define m1_d_1			PAout(8)	  = 1


// matrix1 clk le oe
#define m1_clk_0			PBout(12) 		= 0
#define m1_clk_1			PBout(12) 		= 1
#define m1_le_0				PAout(9)		= 0
#define m1_le_1				PAout(9)	  = 1
#define m1_oe_0				PBout(15)	  = 0
#define m1_oe_1				PBout(15)	  = 1



#define DISP2_ON         m1_oe_0
#define DISP2_OFF        m1_oe_1

#define CLK2_TOGGLE      m1_clk_0;m1_clk_1;
#define STROBE2          m1_le_1; m1_le_0

#define MATRIX_WIDTH    64
#define MATRIX_HEIGHT   32


#define MATRIX_SIZE     (MATRIX_WIDTH*MATRIX_HEIGHT)



extern uint32_t bufferA[MATRIX_SIZE];
//extern uint32_t bufferB[MATRIX_SIZE];


int drawPixel(s8 x, s8 y, u32 Color);
void Matrix_Init(void);
void display_PWM(uint32_t buffer[]); 
 void lcd_draw_hline(u8 x1, u8 x2, u8 y, u32 color);	
 void lcd_draw_vline(u8 x, u8 y1, u8 y2);
 
void lcd_draw_image(const unsigned char *pic);
#endif



