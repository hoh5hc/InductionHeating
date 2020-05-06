#ifndef _function_h
#define _function_h

#include "stm32f0xx.h"
void pad_sense(uint16_t in,uint16_t out);
void Cap_track(void);
void slew_rate_limit(void);
void sampletime(void);


#endif
