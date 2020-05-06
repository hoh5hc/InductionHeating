#include "func.h"
#include "delay.h"
#include "Driver_init.h"
#include <stdlib.h>
#include <string.h> 
#include <stdlib.h> 

/////////////////////// Variable define///////////////////////////////////
uint8_t Timer_IT,ramp_start,ticking,sethome=0;
extern __IO int32_t ENC_Cycles;
int32_t Pulse=0,time,risetime;
int32_t PrePulse=0;
float  SpeedPulse=0;
int32_t PWM=0;
float Err=0,rise_err=0;
float PreE=0;
int32_t U=0;
float Kp=0,Omega,Ki=0,Kd=0,P,I,D,old_omega;
float real=0,oldreal=0,ci=0,ramp_time=400,ramp_count=0,rxpos=0;;
float datarx;
#define BufferSize       5
uint8_t SPI_SLAVE_Buffer_Rx[BufferSize];
uint8_t SPI_SLAVE_Buffer_Tx[BufferSize];
#define Sampletime       0.01
///////////////////////////////////////////////////////////////////////////////////
////////////////////////  Normal function  ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
float hinhthang(double x,float L,float C1,float C2,float R)
{
	float y;
if (x<L) 
{
y=0;
}
else if (x<C1)
{
y =(x-L)/(C1-L);
}
else if (x<C2)
{
y=1;
}
else if (x<R)
{
y=(R-x)/(R-C2);
}
else y=0;
return(y);

}
void PID_Fuzzy_SelfTuning()
{
	int i,j;
double e_scale,de_scale;	
float E[7],DE[5];
float P0,P1,P2,P3,P4,P5,P6,P7,P8,P9;
float beta[35];
	
e_scale= Err/90;
de_scale=(Err - PreE)*3/0.01;
// Declare input e fuzzy //
E[0]=hinhthang(e_scale,-10,-9,-0.7,-0.4);
E[1]=hinhthang(e_scale,-0.7,-0.4,-0.4,0.05);
E[2]=hinhthang(e_scale,-0.4,-0.05,-0.05,0);
E[3]=hinhthang(e_scale,-0.05,0,0,0.05);
E[4]=hinhthang(e_scale,0,0.05,0.05,0.5);
E[5]=hinhthang(e_scale,0.05,0.5,0.5,0.7);
E[6]=hinhthang(e_scale,0.5,0.7,9,10.0);
	
	
// Declare input de fuzzy //
DE[0]=hinhthang(de_scale,-10.0,-9,-0.7,-0.3);
DE[1]=hinhthang(de_scale,-0.7,-0.3,-0.3,0);
DE[2]=hinhthang(de_scale,-0.3,0,0,0.3);
DE[3]=hinhthang(de_scale,0,0.3,0.3,0.7);
DE[4]=hinhthang(de_scale,0.3,0.7,9,10.0);

// Declare output Kp & Ki fuzzy //
	
P0=0.05;
P1=0.1;
P2=0.2;
P3=0.3;
P4=0.4;
P5=0.5;
P6=0.6;
P7=0.7;
P8=0.9;
P9=1;


// Calculate Beta with MAX PROD //

for (i=0;i<=6;i++)
{
	for (j=0;j<=4;j++)
	{
	beta[i*5+j]=E[i]*DE[j];
	}
	j=0;
}

Kp=1+700*(
beta[0]*P9+ beta[1]*P8+ beta[2]*P7+ beta[3]*P6+ beta[4]*P5+
beta[5]*P7+ beta[6]*P6+ beta[7]*P5+ beta[8]*P4+ beta[9]*P3+
beta[10]*P6+ beta[11]*P5+ beta[12]*P4+ beta[13]*P3+ beta[14]*P2+ 
beta[15]*P2+ beta[16]*P1+ beta[17]*P0+ beta[18]*P1+ beta[19]*P2+
beta[20]*P2+ beta[21]*P3+ beta[22]*P4+ beta[23]*P5+ beta[24]*P6+
beta[25]*P3+ beta[26]*P4+ beta[27]*P5+ beta[28]*P6+ beta[29]*P7+
beta[30]*P5+ beta[31]*P6+ beta[32]*P7+ beta[33]*P8+ beta[34]*P9);


Ki=70*(
beta[0]*P9+ beta[1]*P8+ beta[2]*P7+ beta[3]*P6+ beta[4]*P5+
beta[5]*P7+ beta[6]*P6+ beta[7]*P5+ beta[8]*P4+ beta[9]*P3+
beta[10]*P7+ beta[11]*P6+ beta[12]*P5+ beta[13]*P4+ beta[14]*P3+ 
beta[15]*P2+ beta[16]*P1+ beta[17]*P0+ beta[18]*P1+ beta[19]*P2+
beta[20]*P3+ beta[21]*P4+ beta[22]*P5+ beta[23]*P6+ beta[24]*P7+
beta[25]*P3+ beta[26]*P4+ beta[27]*P5+ beta[28]*P6+ beta[29]*P7+
beta[30]*P5+ beta[31]*P6+ beta[32]*P7+ beta[33]*P8+ beta[34]*P9);


	
}
void rise_time()
{
	rise_err= real-old_omega;
	time++;
if (absd(Err)<= absd(rise_err*0.02) & ticking==0)
{
	risetime=time;
	ticking=1;
}
if (absd(Err)> absd(rise_err*0.02) & ticking==1)
{
	
	ticking=0;
}

}
void nelder_mead()
{





}
double absd(double a)
{
	if (a<0)
		a=-a;

return(a);
}

void PID_Verlocity(void)
{
	
//		E= 100 - SpeedPulse;
//		Kp=50*E;
//		Ki=PreKi+5*(E*0.01);
//	//	Kd=(0.005*(E-PreE))/0.01;
//		U=(int)1*(Kp+Ki+Kd) ;
//		PreE=E ;
//		PreKi=Ki;
	
}	
void PID_Position(void)
{
		Pulse=((TIM1->CNT)+ 349*4*34*ENC_Cycles);
		SpeedPulse= -(((((double)(Pulse - PrePulse))*60/349/34)/4)/0.01);
		PrePulse= Pulse;
	  Omega = (double)(Pulse/4)*360/349/34;
		Err= (double)(real- Omega);
		P=Kp*Err;
		I+=Ki*(Err*0.01);
	//	Kd=(80*(E-PreE))/0.01;
		U=(int)1*(P+I) ;
		PreE=Err ;
	
}
int sign(int a)
{
	int b;
	if (a>0) 
		{
			b=1;
		}
		else if (a<0)
		{
			b=-1 ;
		}
		else
		{
		 b=0;
		}
	return b;
}
 void UToPWM()
{
	int temp;
	temp=sign(U);
	
		if (temp>=0)
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_RESET);
		}
		else
		{
			GPIO_WriteBit(GPIOB,GPIO_Pin_13,Bit_SET);			
		}
		PWM=abs(U);
		if (PWM>=9950) 
		{
			PWM=9950;
		}
//		if (Omega>=90 | Omega<=-90)
//		{
//			PWM=0;
//		}

		
		
	
}
void SampleTime_0_1s()
{
		if (sethome==1)
		{
			U=7000;
			UToPWM();
			TIM_SetCompare1(TIM2,PWM);
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==1)
			{
				U=0;
				PWM=0;
				TIM_SetCompare1(TIM2,0);
				Omega=0;
				TIM1->CNT=0;
				ENC_Cycles=0;
				real=0;		
				sethome=0;
			}
			
		}
	else
	{
	if (Timer_IT==1)
	{
		//------- calculate every input things----------

		//------ get that to the controller----------
		PID_Fuzzy_SelfTuning();
		PID_Position();
		//------ output to plant------------------
		UToPWM();		
		TIM_SetCompare1(TIM2,PWM);
		//rise_time();
		Timer_IT=0;
	}
	}
}



///////////////////////////////////////////////////////////////////////////////////
////////////////////////  Interupt routine  ///////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
void DMA1_Channel2_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC2))
  {

		 // tinh du lieu truyen di
		 	memcpy(SPI_SLAVE_Buffer_Tx, &Omega, sizeof Omega);
			SPI_SLAVE_Buffer_Tx[4]=10;    // opcode slave
		if (ramp_start==0)
			SPI_SLAVE_Buffer_Tx[4]=SPI_SLAVE_Buffer_Tx[4]+16;
		
		
			SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
		DMA_Cmd(DMA1_Channel3, ENABLE);
		
		
		//nhan du lieu
    DMA_ClearITPendingBit(DMA1_IT_GL2);
		memcpy(&datarx, SPI_SLAVE_Buffer_Rx, sizeof datarx); 
		if (SPI_SLAVE_Buffer_Rx[4]==1) // of code ramp time
		{
		if (datarx<=0)
			datarx=20;
		ramp_time=datarx;
		}
			if (SPI_SLAVE_Buffer_Rx[4]==2) // ofcode reference pos
		{
			if (rxpos!=datarx)
			{
				if ( datarx==180)
					sethome=1;
				else
				{
		 ramp_start=1;
			rxpos=datarx;
				}
			}
		}
		 
  }
}
void DMA1_Channel3_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA1_IT_TC3))
  {
    DMA_ClearITPendingBit(DMA1_IT_GL3);
		SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
		DMA_Cmd(DMA1_Channel3, DISABLE);
  }
}

void TIM3_IRQHandler()
{
	Timer_IT=1;
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	
}
void SysTick_Handler()
{
	ci++;
	if (ci>10 & ramp_start==1)
		{
			ci=0;
				ramp_count++;
		real=(rxpos-oldreal)*(ramp_count/ramp_time)+oldreal;
	if (ramp_count==ramp_time)
	{
	ramp_start=0;
	ramp_count=0;
	oldreal=rxpos;
	}	
		}
	TimingDelay_Decrement();
}

void TIM1_UP_IRQHandler(void)
{
	//// Find UDF & OVF////
	int TIM_UDF=0,TIM_OVF=0;
	if (TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	if (TIM1->CNT < (349*34*4/2))
	{
	TIM_OVF=1;
	}
	else
	{
	TIM_UDF=1;
	}
	///  config the cycle  ///
	if ( TIM_OVF==1)
	{
		ENC_Cycles++;
	}
	else if (TIM_UDF==1)
	{
		ENC_Cycles--;	
	}	
	
	
	}		


}

