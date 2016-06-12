#ifndef __BSP_PWM_H__
#define __BSP_PWM_H__

#include "includes.h"

extern void PWM_Init(void);
extern void SetMotorPwm(u8 ucDir, u8 ucFreq);

#endif 
// end of file
