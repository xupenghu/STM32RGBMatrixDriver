
#include "adc_timer_dma.h"
#include "led.h"

/* define ---------------------------------------------------------------------*/
#define ADC3_DR_ADDRESS    ((uint32_t)0x4001224C)

/* 变量 ----------------------------------------------------------------------*/
__IO uint16_t ADC3ConvertedValue1[FFT_NUM];
__IO uint16_t ADC3ConvertedValue2[FFT_NUM];
__IO uint32_t ADC3ConvertedVoltage = 0;

/* 供本文件使用 --------------------------------------------------------------*/
static void TIM2_Config(void);
/*
*********************************************************************************************************
*	函 数 名: bsp_InitADC
*	功能说明: ADC初始化
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void adc_timer_dma_init(void)
{  
    ADC_InitTypeDef       ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    DMA_InitTypeDef       DMA_InitStructure;
    GPIO_InitTypeDef      GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    /* 使能 ADC3, DMA2 和 GPIO 时钟 ****************************************/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

    /* DMA2 Stream0 channel2 配置 **************************************/
    DMA_InitStructure.DMA_Channel = DMA_Channel_2;  //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC3_DR_ADDRESS;   //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)ADC3ConvertedValue1;   //DMA存储器地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory; //外设到存储器模式
    DMA_InitStructure.DMA_BufferSize = FFT_NUM;     //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;    //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据长度 16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //存储器数据长度 16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;          
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);
    
    /*DMA double buffer mode*/
    DMA_DoubleBufferModeConfig(DMA2_Stream0,(uint32_t)ADC3ConvertedValue2,DMA_Memory_0);  //DMA_Memory_0先开始传输
    DMA_DoubleBufferModeCmd(DMA2_Stream0,ENABLE);

    	//中断配置    
	NVIC_InitStructure.NVIC_IRQChannel =DMA2_Stream0_IRQn;    
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;   
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
    DMA_ITConfig(DMA2_Stream0 , DMA_IT_TC, ENABLE);//使能传输完成中断  
    DMA_Cmd(DMA2_Stream0, ENABLE);

    /* 配置ADC3通道0引脚为模拟输入模式******************************/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /****************************************************************************   
	  PCLK2 = HCLK / 2 
	  下面选择的是2分频
	  ADCCLK = PCLK2 /2 = HCLK / 4 = 168 / 4 = 42M
      ADC采样频率： Sampling Time + Conversion Time = 3 + 12 cycles = 15cyc
                    Conversion Time = 42MHz / 15cyc = 2.8Mbps. 
	*****************************************************************************/
    /* ADC公共部分初始化**********************************************************/
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
    ADC_CommonInit(&ADC_CommonInitStructure);

    /* ADC3 初始化 ****************************************************************/
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_CC2; //
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfConversion = 1;
    ADC_Init(ADC3, &ADC_InitStructure);
    
    /* ADC3 规则 channel0 配置 *************************************/
    ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);

    /* 使能DMA请求(单ADC模式) */
    ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE);

    /* 使能 ADC3 DMA */
    ADC_DMACmd(ADC3, ENABLE);

    /* 使能 ADC3 */
    ADC_Cmd(ADC3, ENABLE);

    TIM2_Config();
}

/*
*********************************************************************************************************
*	函 数 名: TIM1_Config
*	功能说明: 配置定时器1，用于触发ADC, 触发频率1Msps
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TIM2_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			   			//使能定时器1时钟
    
    TIM_Cmd(TIM2, DISABLE);	
    
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 				   //初始化定时器1的寄存器为复位值
    TIM_TimeBaseStructure.TIM_Period = 24;        // ARR自动重装载寄存器周期的值(定时时间）到设置频率后产生个更新或者中断(也是说定时时间到)
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;   						   //PSC时钟预分频数 例如：时钟频率=TIM1CLK/(时钟预分频+1)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    				   //CR1->CKD时间分割值
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	   //CR1->CMS[1:0]和DIR定时器模式 向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				   //CCMR2在向上计数时，一旦TIMx_CNT<TIMx_CCR1时通道1为有效电平，否则为无效电平
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;      //CCER 输出使能          
    TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2;//CCR3同计数器TIMx_CNT的比较，并在OC4端口上产生输出信号 
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;    	   //CCER输出极性设置	高电平有效     
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_Cmd(TIM2, ENABLE);											   
}

//中断处理函数  
void  DMA2_Stream0_IRQHandler(void)  
{  
    uint8_t res = 0;
        if(DMA_GetITStatus( DMA2_Stream0, DMA_IT_TCIF0)!=RESET)
		 {  
            
            DMA_ClearITPendingBit( DMA2_Stream0,  DMA_IT_TCIF0);  
		 }  
 }  


