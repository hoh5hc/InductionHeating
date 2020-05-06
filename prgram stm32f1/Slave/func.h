#ifndef _func_H
#define _func_H
#include "STM32F10x.h"

void EXTI15_10_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void SampleTime_0_1s(void);
void SysTick_Handler(void);
void UART_send(char *c);
void TIM4_IRQHandler(void);
void PID_Verlocity(void);
void PID_Position(void);
void UToPWM(void);
int sign(int a);
float hinhthang(double x,float L,float C1,float C2,float R);
void PID_Fuzzy_SelfTuning(void);
void rise_time(void);
double absd(double a);

//////////////////////////////////////////////////
///////// Defines const here///////////////////
//////////////////////////////////////////////////
#define pi 3.141592654







//////////////////////////////////////////////////
///////// Defines variable here///////////////////
//////////////////////////////////////////////////




#endif
