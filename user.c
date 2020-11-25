/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>         
#include <stdbool.h>         

#include "user.h"            
#include "system.h"

//Include all platform specific files.
#include "IO_ports.h"
#include "timers.h"
#include "pwm.h"
#include "adc.h"
#include "uart.h"

//Initialization of all dsPIC30F4011 peripherals.
void dsPIC30F4011_v_Init()
{
  Ports_v_Init();
  Timers_v_Init();
  PWM_v_Init();
  ADC_v_Init();
  UART_v_Init();
}

//Initialization of all sensor values.
void Sensor_v_Init()
{
  t_Sensorvalues Sensorvalues = 
  {
    0u,         //Initial current is zero.
    0u,         //Initial DC voltage is zero.
    0u,         //Initial speed.
    PWM_PERIOD, //Initial duty cycle is 50%
    PWM_PERIOD  //Initial duty cycle is 50%
  };
}

//Initialization of current & speed PI regulator parameters.
void PIReg_v_Init()
{
 t_PIRegulatorData  PIReg =
 {
   //Current PI regulator parameters.
   {
     0,
     0,
     0,
     0,
     CURRENT_PROPORTIONAL_GAIN, 
     CUREENT_INTEGRAL_GAIN,
     CURRENT_LOOP_TIME_CONSTANT
   },
   //Speed PI regulator parameters.
   {
     0,
     0,
     0,
     0,
     SPEED_PROPORTIONAL_GAIN, 
     SPEED_INTEGRAL_GAIN,
     SPEED_LOOP_TIME_CONSTANT
   }
 };
}

//Initialization of DC machine parameters.
void DCM_v_Init()
{
  t_DCMInfo DCMInfo =
  {
    ARMATURE_RESISTANCE,
    ARMATURE_INDUCTANCE,
    ELECTRICAL_TIME_CONSTANT,
    NOMINAL_FLUX,
    MOMENT_OF_INERTIA,
    NOMINAL_ANGULAR_FREQUENCY,
    NOMINAL_SPEED,
    NOMINAL_DC_BUS_VOLTAGE,
    NOMINAL_VOLTAGE,
    NOMINAL_CURRENT,
    {0u} //Reserved bytes.
  };
}

float f_SetReferentCurrent(float f_ReferentCurrent)
{
  float f_ScaledCurrentValue = 0;
  
  //Entered value is in the range.
  if ( (f_ReferentCurrent >= -(NOMINAL_CURRENT) ) && (f_ReferentCurrent <= NOMINAL_CURRENT) )
  {
    //Linearly scale the entered current value.
    // (0 - NOMINAL_CURRENT) -> (0 - 1023)
    f_ScaledCurrentValue = (MAX_VALUE/NOMINAL_CURRENT)*f_ReferentCurrent;
  }
  
  return f_ScaledCurrentValue;
};

float f_SetReferentSpeed(int ReferentSpeed)
{
  float f_ScaledSpeedValue = 0;
  
  //Entered value is in the range.
  if ( (ReferentSpeed >= -(NOMINAL_SPEED) ) && (ReferentSpeed <= NOMINAL_SPEED) )
  {
    //Linearly scale the entered speed value.
    // (0 - NOMINAL_SPEED) -> (0 - 1023)
    f_ScaledSpeedValue = (float)((MAX_VALUE/NOMINAL_SPEED)*ReferentSpeed);
  }

  return f_ScaledSpeedValue;
};
