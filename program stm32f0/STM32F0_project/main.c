#include "main.h"
#include "delay.h"
#include "IO_init.h"
#include "function.h"
uint8_t Padout[2];
uint8_t padnum=0,sign;
uint32_t  TimerPeriod,Channel1Pulse;
void TIMp_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* GPIOA, GPIOB and GPIOE Clocks enable */
  RCC_AHBPeriphClockCmd( RCC_AHBPeriph_GPIOA , ENABLE);
  
  /* GPIOA Configuration: Channel 1, 2, 3, 4 and Channel 1N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 |GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
 GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);


  
  /* TIM1 Configuration ---------------------------------------------------
   Generate 7 PWM signals with 4 different duty cycles:
   TIM1 input clock (TIM1CLK) is set to APB2 clock (PCLK2)    
    => TIM1CLK = PCLK2 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock is set to 48 MHz for STM32F0xx devices
   
   The objective is to generate 7 PWM signal at 17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
   The channel 1 and channel 1N duty cycle is set to 50%
   The channel 2 and channel 2N duty cycle is set to 37.5%
   The channel 3 and channel 3N duty cycle is set to 25%
   The channel 4 duty cycle is set to 12.5%
   The Timer pulse is calculated as follows:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
   
   Note: 
    SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f0xx.c file.
    Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
    function to update SystemCoreClock variable value. Otherwise, any configuration
    based on this variable will be incorrect. 
  ----------------------------------------------------------------------- */
  /* Compute the value to be set in ARR regiter to generate signal frequency at 17.57 Khz */
  TimerPeriod = (SystemCoreClock / 33000 ) - 1;
  /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
  Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);


  /* TIM1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15 , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod ;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;

	
  TIM_OC1Init(TIM15, &TIM_OCInitStructure);
	TIM_OC1Init(TIM15, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM15, TIM_OCPreload_Enable);
	
	
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;


  TIM_OC2Init(TIM15, &TIM_OCInitStructure);
	TIM_OC2Init(TIM15, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM15, TIM_OCPreload_Enable);


  TIM_ARRPreloadConfig(TIM15, ENABLE);
  /* TIM1 counter enable */
  TIM_Cmd(TIM15, ENABLE);

  /* TIM1 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM15, ENABLE);
}
int main(void)
{
	if (SysTick_Config(SystemCoreClock / 66000))
  { 
    /* Capture error */ 
    while (1);
  }
led_init();
//TIMp_Config();
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
  while (1)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		GPIO_SetBits(GPIOA,GPIO_Pin_3);
		delay_ms(1);
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		delay_ms(1);
		
	
	}
}

