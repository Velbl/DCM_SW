/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "user.h"            /* variables/params used by user.c               */
#include "system.h"

/*Include all platform specific files*/
#include "IO_ports.h"
#include "timers.h"
#include "pwm.h"
#include "adc.h"

/*Initialization of all dsPIC30F4011 peripherals*/
void dsPIC30F4011_v_Init()
{
  Ports_v_Init();
  Timers_v_Init();
  PWM_v_Init();
  ADC_v_Init();
}
