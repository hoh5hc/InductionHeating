#include "function.h"
#include "delay.h"
#include "IO_init.h"
uint8_t count[2]={1,1};
uint16_t Down_period[2],HighPoint[2];
double Sum[2];
double Base_Cap[2]={3000,3000};
uint16_t TimIT;
extern uint8_t Padout[2];
uint16_t counter[2];
extern uint8_t padnum,sign;
uint32_t dem=0;
uint32_t fre=32;


void sampletime()
{
if (TimIT==1)
{
	sign=1;
	TimIT=0;

	padnum++;
if (padnum>1)
	padnum=0;

}



}
///========================================================
void pad_sense(uint16_t in,uint16_t out)
{
if (count[padnum]==1)
{
	GPIO_SetBits(GPIOA,out);
	count[padnum] =0;
}
if (	GPIO_ReadInputDataBit( GPIOA,in)==1)
{	
delay_us(40);
	HighPoint[padnum]=	TIM_GetCounter(TIM3);
	GPIO_ResetBits(GPIOA,out);
}
}
///========================================================
void Cap_track(void)
{

	if (Padout[padnum]==0)
	{
if (Base_Cap[padnum]> Sum[padnum])
{
Base_Cap[padnum]=Base_Cap[padnum]-10;
}
else if (Base_Cap[padnum]< Sum[padnum])
{
Base_Cap[padnum]++;
}
}


}

void TIM14_IRQHandler()
{
	if (TIM_GetITStatus(TIM14,TIM_IT_Update ) != RESET)
	{
		TimIT=1;
    TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
	}

}


///========================================================
void SysTick_Handler(void)
{
	//	dem++;
	//if( dem==fre/2)
	//{
	//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	//	GPIO_SetBits(GPIOB,GPIO_Pin_7);
	////}
	//if(dem>=fre) 
	//{
	//	dem=0;
	//	GPIO_ResetBits(GPIOB,GPIO_Pin_7);
		//GPIO_SetBits(GPIOA,GPIO_Pin_0);
	//}
  TimingDelay_Decrement();
}
///==================================================
void EXTI4_15_IRQHandler(void)
{
uint16_t temp;
  if(EXTI_GetITStatus(EXTI_Line5) != RESET)
  {
	temp=	TIM_GetCounter(TIM3);
			count[padnum]=1;
		if (temp>= HighPoint[padnum])
		Down_period[padnum]=temp -HighPoint[padnum];
		else
			Down_period[padnum]=temp+(0xFFFF-HighPoint[padnum]);
	
		Sum[padnum]=(Down_period[padnum] - 0.0625*Sum[padnum]) +Sum[padnum];
		slew_rate_limit();
    EXTI_ClearITPendingBit(EXTI_Line5);
  }
	  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  {
	temp=	TIM_GetCounter(TIM3);
			count[padnum]=1;
		if (temp>= HighPoint[padnum])
		Down_period[padnum]=temp -HighPoint[padnum];
		else
			Down_period[padnum]=temp+(0xFFFF-HighPoint[padnum]);
	
		Sum[padnum]=(Down_period[padnum] - 0.0625*Sum[padnum]) +Sum[padnum];
		slew_rate_limit();
    EXTI_ClearITPendingBit(EXTI_Line6);
  }
	
	
}
///==================================================
void slew_rate_limit(void)
{
if (Sum[padnum]>=2*Base_Cap[padnum] & Padout[padnum]==0)
	counter[padnum]++;
else if ((Sum[padnum]<2*Base_Cap[padnum]) & (counter[padnum]>0) )
	counter[padnum]--;
if (counter[padnum]>=1000)
Padout[padnum]=1;
else
Padout[padnum]=0;
}


