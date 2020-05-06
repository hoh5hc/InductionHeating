#include "Driver_init.h"

//-----------------IO init struct--------------------
GPIO_InitTypeDef GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;
EXTI_InitTypeDef   EXTI_InitStructure;
USART_InitTypeDef USART_InitStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
SPI_InitTypeDef  SPI_InitStructure;
DMA_InitTypeDef  DMA_InitStructure;

//-----------------variable declare--------------------
#define BufferSize       5
#define SPI_SLAVE_DR_Base            0x4001300C
uint16_t PrescalerValue;
extern uint8_t SPI_SLAVE_Buffer_Rx[BufferSize];
extern uint8_t SPI_SLAVE_Buffer_Tx[BufferSize];

//-----------------init function--------------------
void spi_init_slave()
{
/* PCLK2 = HCLK/2 */
  RCC_PCLK2Config(RCC_HCLK_Div2); 

  /* Enable peripheral clocks --------------------------------------------------*/
  /* Enable SPI_SLAVE DMA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);


  /* Enable SPI_MASTER clock and GPIO clock for SPI_MASTER and SPI_SLAVE */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);

  /* Enable SPI_SLAVE Periph clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	

  /* Configure SPI_SLAVE pins:  SCK and MOSI*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
/* Configure SPI_SLAVE pins:  MISO*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

 /* SPI_SLAVE_Rx_DMA_Channel configuration ---------------------------------------------*/
	DMA_DeInit(DMA1_Channel2);
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_SLAVE_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI_SLAVE_Buffer_Rx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = BufferSize;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular ;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
	
	/* SPI_SLAVE_Tx_DMA_Channel configuration ---------------------------------------------*/
	DMA_DeInit(DMA1_Channel3);  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SPI_SLAVE_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SPI_SLAVE_Buffer_Tx;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_Init(DMA1_Channel3, &DMA_InitStructure);
	
  /* SPI_SLAVE configuration ------------------------------------------------------*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI1, &SPI_InitStructure);

  /* Enable SPI_SLAVE Rx request */
  SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
 SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
 
 DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);
  /* Enable SPI_SLAVE */
  SPI_Cmd(SPI1, ENABLE);


  /* Enable DMA1 Channel4 */
  DMA_Cmd(DMA1_Channel2, ENABLE);
	//DMA_Cmd(DMA1_Channel3, ENABLE);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* Enable the USARTy_DMA1_IRQn Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	  /* Enable the USARTy_DMA1_IRQn Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void PWM_init(void)
{
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 /* TIM3 clock enable */
  TIM_OCStructInit(&TIM_OCInitStructure);

 PrescalerValue = (uint16_t) (SystemCoreClock / 72000000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 10000;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

  TIM_OC1Init(TIM2, &TIM_OCInitStructure);

  TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

 
  TIM_ARRPreloadConfig(TIM2, ENABLE);

  /* TIM3 enable counter */
  TIM_Cmd(TIM2, ENABLE);
//	TIM_CtrlPWMOutputs(TIM2, ENABLE);
}
void Enc_config()
{
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
 NVIC_Init(&NVIC_InitStructure);	
TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
TIM_TimeBaseStructure.TIM_Period = 349*4*34-1;//349
TIM_TimeBaseStructure.TIM_Prescaler = 0;
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
/* Configure the timer */
TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);/* TIM4 counter enable */


TIM_Cmd(TIM1, ENABLE);
TIM_UpdateRequestConfig(TIM1,TIM_UpdateSource_Regular);

}
void Basic_timer_init()
{
	RCC_PCLK1Config(RCC_HCLK_Div1);
	PrescalerValue = (uint16_t) ((SystemCoreClock) / 1000000) - 1;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel =TIM3_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 5000;
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_PrescalerConfig(TIM3,PrescalerValue, TIM_PSCReloadMode_Immediate);
	TIM_ITConfig(TIM3,TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

