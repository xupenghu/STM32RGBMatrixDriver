#include "my_fft.h"


u32 lBUFIN[NPT] ;        /* Complex input vector */
u32 lBUFOUT[NPT] ;       /* Complex output vector */
u32 lBUFMAG[NPT/2];        /* Magnitude vector */

u8 table_0_31[NPT];

const u32  piecewise[33] =
{ 0, 		6, 		9, 		12, 		15, 		18, 		21, 	 23, 
	33, 	43, 	53, 	63, 	73, 	83,  		93, 	 103, 
	123, 	143,		163, 	183,		203,		223,			243,	 263,
	300, 	350,	450, 	600,		850,		1000,			1200,	 4800, 0xffffffff

};

/*
*********************************************************************************************************
*    函 数 名: PowerMag
*    功能说明: 求模值
*    形    参：_usFFTPoints  FFT点数
*    返 回 值: 无
*********************************************************************************************************
*/
void PowerMag(uint16_t _usFFTPoints)     
{
     int16_t lX,lY;
     uint16_t i;
		 u8 j;
     float mag;
		 u32 sum = 0 ;

     /* 计算幅值 */
     for (i=0; i < _usFFTPoints/2; i++)
     {
         lX= (lBUFOUT[i]<<16)>>16;          /* 实部*/
         lY= (lBUFOUT[i]>> 16);             /* 虚部 */   
         mag = sqrt(lX*lX+ lY*lY);      		/* 求模 */
         lBUFMAG[i]= (mag*2);                 /* 求模后乘以2才是实际模值，直流分量不需要乘2 */
				 sum +=		lBUFMAG[i];
			 
				for(j = 0 ; j < 32 ; j ++)
				{
					if((piecewise[j] < lBUFMAG[i]) && (lBUFMAG[i]) < piecewise[j+1])
					{
						table_0_31[i] = j;
						continue;
					}
				}
			  
		 }
		 
		 i = 0;
    
     /* 由于上面多乘了2，所以这里直流分量要除以2 */
     lBUFMAG[0] = ((lBUFMAG[1]<<16) + ((u16)lBUFMAG[7]<<8) + (u8)(lBUFMAG[13]));
}





//40KHz的采样率 256点fft 频率间隔是156.25Hz

void my_fft_test()
{
	
	u16 i=0;
  for(i=0;i<NPT;i++)
  {
		 /* 波形是由直流分量，156.25*2 Hz正弦波和156.25*5 Hz正弦波 组成，波形采样率Fs */
		//输出结果应该是 lBUFMAG[0] = 25600; lBUFMAG[2] = 640; lBUFMAG[5] = 1280
		lBUFIN[i] = 0 ;
    lBUFIN[i]  = (1024 + 1024 * sin(PI2*i*2/Fs) + 512 * sin(PI2*i*18/Fs));
  }
	
  cr4_fft_256_stm32(lBUFOUT, lBUFIN, NPT);
	PowerMag(NPT);
	
}

void my_fft()
{

	
	cr4_fft_256_stm32(lBUFOUT, lBUFIN, NPT);
	PowerMag(NPT);
//	select();
}







