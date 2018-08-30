#ifndef ADC_TIMER_DMA__H
#define ADC_TIMER_DMA__H
#include "sys.h"

/* 变量 ----------------------------------------------------------------------*/
//FFT采样点数
#define FFT_NUM     256

extern __IO uint16_t ADC3ConvertedValue1[FFT_NUM];
extern __IO uint16_t ADC3ConvertedValue2[FFT_NUM];

extern __IO uint32_t ADC3ConvertedVoltage ;

void adc_timer_dma_init(void);




#endif



