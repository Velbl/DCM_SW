/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif      

#include "user.h"  
#include "user_cfg.h"

//Include all platform specific files.
#include "IO_ports.h"
#include "timers.h"
#include "pwm.h"
#include "adc.h"
#include "uart.h"

void dsPIC30F4011_v_Config()
{
  Ports_v_Config();
  Timers_v_Config();
  PWM_v_Config();
  ADC_v_Config();
  UART_v_Config();
}

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
    PII_REG_MAX_OUTPUT,
    PII_REG_MIN_OUTPUT,
  },
  //Speed PI regulator parameters.
  {
    0,
    0,
    0,
    0,
    SPEED_PROPORTIONAL_GAIN, 
    SPEED_INTEGRAL_GAIN,
    PIW_REG_MAX_OUTPUT,
    PIW_REG_MIN_OUTPUT,
  }
};

void f_SetReferentCurrent(int SpeedOutput)
{
  int ReferentCurrent = SpeedOutput;
  
  // Entered value is in the range.
  if ( ( ReferentCurrent >= MINIMAL_CURRENT ) && ( ReferentCurrent <= MAXIMAL_CURRENT ) )
  {
    // Scale entered referent value.
    // 0    ->   0
    // -18A -> - 32768
    // 18A  ->   32768
    PIReg.s_CurrentReg.ReferentCurrent = (int)(((float)PII_REG_MAX_OUTPUT / (float)MAXIMAL_CURRENT ) * ReferentCurrent);
  }
  // Entered referent value is over the maximal limit.
  else if ( ReferentCurrent >= MAXIMAL_CURRENT )
  { 
    // Set referent current to maximal current value.
    PIReg.s_CurrentReg.ReferentCurrent = PII_REG_MAX_OUTPUT; 
  }
  // Entered referent value is over the negative limit.
  else 
  {
    //Set referent current to minimal current value.
    PIReg.s_CurrentReg.ReferentCurrent = PII_REG_MIN_OUTPUT;
  }
};

void f_SetReferentSpeed(int ReferentSpeed)
{
  
  // Entered value is in the range.
  if ( (ReferentSpeed >= MINIMAL_SPEED ) && (ReferentSpeed <= MAXIMAL_SPEED) )
  { 
    // Scale entered referent value.
    // 0     ->   0
    // -3370 ->  -8192
    // 3370  ->   8192
    PIReg.s_SpeedReg.ReferentSpeed = (int)( PIW_REG_MAX_OUTPUT * ((float)ReferentSpeed / (float)MAXIMAL_SPEED));    
  }
  // Entered referent value is over the maximal limit.
  else if ( ReferentSpeed >= MAXIMAL_SPEED )
  { 
    //Set referent speed to maximal speed value.
    PIReg.s_SpeedReg.ReferentSpeed = PIW_REG_MAX_OUTPUT; 
  }
  // Entered referent value is over the negative limit.
  else 
  {
    //Set referent speed to minimal speed value.
    PIReg.s_SpeedReg.ReferentSpeed = PIW_REG_MIN_OUTPUT;
  }
};

void v_CalculatePIRegOutput(e_RegulatorTypes RegulatorType)
{
  int   LastError      = 0;
  int   ErrorDiference = 0;
  int   Increment      = 0;
  
  // The user has selected calculation of current PI regulator  output.
  if ( RegulatorType == CURRENT_REGULATOR )
  {
    // Catch last error value.
    LastError = PIReg.s_CurrentReg.Error; 
    
    // Calculate current error value.
    PIReg.s_CurrentReg.Error = PIReg.s_CurrentReg.ReferentCurrent - PIReg.s_CurrentReg.MeasuredCurrent; 
    
    // Calculate deference between last and current error.
    ErrorDiference = PIReg.s_CurrentReg.Error - LastError; 

    // Calculate increment in 1.15 format.
    Increment = ( (int)((long)PIReg.s_CurrentReg.Kpi*(long)ErrorDiference           >> SHIFT_AMOUNT_1_15 ) + 
                  (int)((long)PIReg.s_CurrentReg.Kii*(long)PIReg.s_CurrentReg.Error >> SHIFT_AMOUNT_1_15 )
                );
    
    // Set regulator output.
    PIReg.s_CurrentReg.Output += Increment;
   
    // Calculated output value is over the maximal limit.
    if (PIReg.s_CurrentReg.Output      > PII_REG_MAX_OUTPUT)
    {
      // Set output to maximal value.
      PIReg.s_CurrentReg.Output        = PII_REG_MAX_OUTPUT;
    }
    // Calculated output value is over the minimal limit.
    else if (PIReg.s_CurrentReg.Output < PII_REG_MIN_OUTPUT)
    { 
      // Set output to minimal value.
      PIReg.s_CurrentReg.Output        = PII_REG_MIN_OUTPUT;
    }
  }
  
  // The user has selected calculation of speed PI regulator  output.
  if ( RegulatorType == SPEED_REGULATOR )
  {
    // Catch last error value.
    LastError = PIReg.s_SpeedReg.Error;
    
    // Calculate current error value.
    PIReg.s_SpeedReg.Error = PIReg.s_SpeedReg.ReferentSpeed - PIReg.s_SpeedReg.MeasuredSpeed;
    
    // Calculate deference between last and current error.
    ErrorDiference = LastError - PIReg.s_SpeedReg.Error;
    
    // Calculate increment in 3.13 format.
    Increment = ( (int)((long)PIReg.s_SpeedReg.Kpw*(long)ErrorDiference           >> SHIFT_AMOUNT_3_13 ) + 
                  (int)((long)PIReg.s_SpeedReg.Kiw*(long)PIReg.s_SpeedReg.Error   >> SHIFT_AMOUNT_3_13 )
                );
    
    // Set regulator output.
    PIReg.s_SpeedReg.Output += Increment;
    
    // Calculated output value is over the maximal limit.
    if (PIReg.s_SpeedReg.Output > PIW_REG_MAX_OUTPUT)
    {
      // Set output to maximal value.
      PIReg.s_SpeedReg.Output   = PIW_REG_MAX_OUTPUT;
    }
    // Calculated output value is over the minimal limit.
    if (PIReg.s_SpeedReg.Output < PIW_REG_MIN_OUTPUT)
    { 
      // Set output to minimal value.
      PIReg.s_SpeedReg.Output   = PIW_REG_MIN_OUTPUT;
    }
  }
}

int i_ConvertToFixedPoint(int MeasuredValue, e_FormatTypes FormatType)
{ 
  int ToReturn = 0;
  
  // The user has selected conversion to 1.15 format.
  if ( FormatType == FORMAT_1_15 )
  {
    // 0 - 1023 -> 0 - 32768
    ToReturn = (PII_REG_MAX_OUTPUT/1023)*MeasuredValue;
  }
  // The user has selected conversion to 3.13 format.
  if ( FormatType == FORMAT_3_13 )
  {
    // 0 - 1023 -> 0 - 8192
    ToReturn = (PIW_REG_MAX_OUTPUT/1023)*MeasuredValue;
  }
  
  return ToReturn;
}