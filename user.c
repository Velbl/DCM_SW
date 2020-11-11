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
#include "uart.h"

#define DCM_INFO {0,0}
#define PIREG_DEFAULTS
/****************************INITIALIZATION OF ALL PARAMETERS******************/
//TODO: SET PROPER PARAMETERS.
//DC machine parameters. 
uint8_t u_Uan = 15u;         //Nominal machine voltage.
uint8_t u_Ian = 4u;          //Nominal armature current.
uint8_t u_Udc = 24u;         //Nominal DC voltage.
float   f_Fn  = 0.0194f;     //Nominal machine flux.
float   f_Ra  = 0.334f;      //Armature resistance.
float   f_La  = 0.000085f;   //Armature inductance.
float   f_wn  = 704.3f;      //Nominal angular frequency.
float   f_nn  = 6725.9f;     //Nominal angular speed.
float   f_J   = 0.00000676f; //Momentum of inertia.

t_SensorValues SensorValues = 
{
  0u,         //Initial current is zero.
  0u,         //Initial DC voltage is zero.
  0u,         //Initial speed.
  PWM_PERIOD, //Initial duty cycle is 50%
  PWM_PERIOD  //Initial duty cycle is 50%
};

t_CurrentReg CurrentReg =
{
    
};

t_SpeedReg SpeedReg =
{
    
};
/******************************************************************************/


/*Initialization of all dsPIC30F4011 peripherals*/
void dsPIC30F4011_v_Init()
{
  Ports_v_Init();
  Timers_v_Init();
  PWM_v_Init();
  ADC_v_Init();
  UART_v_Init();
}
