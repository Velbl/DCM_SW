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

  t_MeasuredValues Measured = 
  {
    0u,         //Initial measured current is zero.
    0u,         //Initial measured DC voltage is zero.
    0u,         //Initial measured speed.
  };

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
     32767,
     -32767,
   },
   //Speed PI regulator parameters.
   {
     0,
     0,
     0,
     0,
     SPEED_PROPORTIONAL_GAIN, 
     SPEED_INTEGRAL_GAIN,
     32767,
     -32767,
   }
 };

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


float f_SetReferentCurrent(float f_ReferentCurrent)
{
  //PIReg.s_CurrentReg.ReferentCurrent = f_ReferentCurrent;
  
  float f_ScaledCurrentValue = 0.0f;
  
  //Entered value is in the range.
  if ( (f_ReferentCurrent >= -(NOMINAL_CURRENT) ) && (f_ReferentCurrent <= NOMINAL_CURRENT) )
  {
    //Linearly scale entered current value.
    // (0 - NOMINAL_CURRENT) -> (0 - 1023)
    f_ScaledCurrentValue = ((float)(OUTPUT_MAX_VALUE/NOMINAL_CURRENT)*f_ReferentCurrent);
    PIReg.s_CurrentReg.ReferentCurrent = f_ScaledCurrentValue;
  }
  
  return f_ScaledCurrentValue;
};

float f_SetReferentSpeed(float ReferentSpeed)
{
  float f_ScaledSpeedValue = 0.0f;
  
  //Entered value is in the range.
  if ( (ReferentSpeed >= -(NOMINAL_SPEED) ) && (ReferentSpeed <= NOMINAL_SPEED) )
  {
    //Linearly scale entered speed value.
    // (0 - NOMINAL_SPEED) -> (0 - 1023)
    f_ScaledSpeedValue = (OUTPUT_MAX_VALUE/NOMINAL_SPEED)*ReferentSpeed;
    PIReg.s_SpeedReg.ReferentSpeed = f_ScaledSpeedValue;
  }

  return f_ScaledSpeedValue;
};

void v_CalculatePIRegOutput(e_RegulatorTypes RegulatorType)
{
  float   LastError      = 0.0f;
  float   ErrorDiference = 0.0f;
  float   Increment      = 0.0f;
  
  //We want to calculate output of current PI regulator.
  if ( RegulatorType == CURRENT_REGULATOR )
  {
    //Catch last error value.
    LastError = PIReg.s_CurrentReg.Error; 
    
    //Current error value.
    PIReg.s_CurrentReg.Error = PIReg.s_CurrentReg.ReferentCurrent - PIReg.s_CurrentReg.MeasuredCurrent; 
    
    //Deference between these two errors.
    ErrorDiference = PIReg.s_CurrentReg.Error - LastError; 
    
    //Calculate increment value.
    Increment = (PIReg.s_CurrentReg.Kpi*ErrorDiference + PIReg.s_CurrentReg.Kii*PIReg.s_CurrentReg.Error );
    
    /* IN FIXED POINT ARITHMETIC (1.15 format)
     * Increment = ( (int)((long)PIReg.s_CurrentReg.Kpi*(long)ErrorDiference           >> 15) + 
                  (int)((long)PIReg.s_CurrentReg.Kii*(long)PIReg.s_CurrentReg.Error >> 15)
                );
    */
    
    //Set regulator output.
    PIReg.s_CurrentReg.Output += Increment;
    
#ifdef CURRENT_REGULATOR_TEST 
    
    //Print measured current.
    UART_v_Print(PIReg.s_CurrentReg.Output);    
    //Insert new line.
    UART_v_NewLine();
    
    LATBbits.LATB3 = PIReg.s_CurrentReg.MeasuredCurrent;
#endif
    
    //Limit regulator output.
    if (PIReg.s_CurrentReg.Output > OUTPUT_MAX_VALUE)
    {
      PIReg.s_CurrentReg.Output = OUTPUT_MAX_VALUE;
    }
    else if (PIReg.s_CurrentReg.Output < OUTPUT_MIN_VALUE)
    { 
      PIReg.s_CurrentReg.Output = OUTPUT_MIN_VALUE;
    }
  }
  
  //We want to calculate output of speed PI regulator.
  if ( RegulatorType == SPEED_REGULATOR )
  {
    //Catch last error value.
    LastError = PIReg.s_SpeedReg.Error;
    
    //Current error value.
    PIReg.s_SpeedReg.Error = PIReg.s_SpeedReg.ReferentSpeed - PIReg.s_SpeedReg.MeasuredSpeed;
    
    //Deference between these two errors.
    ErrorDiference = LastError - PIReg.s_SpeedReg.Error;
    
    //Calculate increment value.
    Increment = ( PIReg.s_SpeedReg.Kpw*ErrorDiference + PIReg.s_SpeedReg.Kiw*PIReg.s_SpeedReg.Error );
    /*Increment = ( (int)((long)PIReg.s_SpeedReg.Kpw*(long)ErrorDiference           >> 13) + 
                  (int)((long)PIReg.s_SpeedReg.Kiw*(long)PIReg.s_SpeedReg.Error   >> 15)
                );
    */
    //Set regulator output.
    PIReg.s_SpeedReg.Output += Increment;
    
#ifdef SPEED_REGULATOR_TEST   
    
    //Print measured speed.
    UART_v_Print(PIReg.s_SpeedReg.Output);    
    //Insert new line.
    UART_v_NewLine();

    LATBbits.LATB3 = PIReg.s_SpeedReg.Output;
#endif
    
    //Limit regulator output.
    if (PIReg.s_SpeedReg.Output > OUTPUT_MAX_VALUE)
    {
      PIReg.s_SpeedReg.Output = OUTPUT_MAX_VALUE;
    }
    if (PIReg.s_SpeedReg.Output < OUTPUT_MIN_VALUE)
    { 
      PIReg.s_SpeedReg.Output = OUTPUT_MIN_VALUE;
    }
  }
}