/* 
 * File:   pwm.h
 * Author: Stefan Velbl
 *
 * Created on 21 October 2020, 22:46
 */
#ifndef PWM_H
#define	PWM_H
     
/// Interface for setting the PWM duty cycle.
void v_SetDutyCyclePWM_v_SetDutyCycle(uint16_t u_PDCx, uint16_t u_DutyRatio, uint8_t u_UpdateDisable);

/// Interface for PWM configuration.
void PWM_v_Config(void);

#endif	/* PWM_H */

