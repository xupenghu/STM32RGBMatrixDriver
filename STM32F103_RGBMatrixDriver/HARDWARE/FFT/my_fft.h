#ifndef __MY_FFT_H
#define  __MY_FFT_H


#include "math.h" 
#include "sys.h"


#define PI2  6.28318530717959
#define Fs   512 //模拟采样率40KHz


#define NPT_256		 256 	//选择多少个点

#define NPT NPT_256

extern void cr4_fft_256_stm32(void *pssOUT, void *pssIN, u16 Nbin);//函数中
//pssIN参数一定要指向32位数组，后16位为虚部，前16位为实部。前16位后16位都为带符号的补码。
extern u32 lBUFIN[NPT] ;        /* Complex input vector */
extern u32 lBUFMAG[NPT/2];        /* Magnitude vector */

extern u8 table_0_31[NPT];




void my_fft_test(void);
void my_fft(void);




#endif





