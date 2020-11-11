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

//PWM interface for setting up of the duty cycle.
void PWM_v_SetDutyCycle(uint16_t u_PDCx, uint16_t u_DutyRatio, uint8_t u_UpdateDisable);

//PWM initialization interface.
void PWM_v_Init(void);

#endif	/* PWM_H */

