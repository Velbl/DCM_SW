/* 
 * File:   pwm.h
 * Author: Stefan Velbl
 *
 * Created on 21 October 2020, 22:46
 */
#ifndef PWM_H
#define	PWM_H
#include "user.h"            /* variables/params used by user.c               */

extern uint16_t u_DutyCycle;

void PWM_v_Init(void);
#endif	/* PWM_H */

