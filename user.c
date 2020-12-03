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

//Convert Amps to 1.15 format.
void f_SetReferentCurrent(int ReferentCurrent)
{
  //Entered value is in the range.
  if ( (ReferentCurrent >= MINIMAL_CURRENT ) && (ReferentCurrent <= MAXIMAL_CURRENT) )
  {
    //Linear scale of entered current value. 
    PIReg.s_CurrentReg.ReferentCurrent = (PII_REG_MAX_OUTPUT/MAXIMAL_CURRENT)*ReferentCurrent;
  }
  else if ( ReferentCurrent >= MAXIMAL_CURRENT )
  { 
    //Set referent current to maximal current value.
    PIReg.s_CurrentReg.ReferentCurrent = PII_REG_MAX_OUTPUT; 
  }
  else 
  {
    //Set referent current to minimal current value.
    PIReg.s_CurrentReg.ReferentCurrent = PII_REG_MIN_OUTPUT;
  }
};

//Convert Rpm to 3.13 format.
void f_SetReferentSpeed(int ReferentSpeed)
{
  
  //Entered value is in the range.
  if ( (ReferentSpeed >= MINIMAL_SPEED ) && (ReferentSpeed <= MAXIMAL_SPEED) )
  { 
    //Convert RPM to 3.13 format.
    PIReg.s_SpeedReg.ReferentSpeed = (PIW_REG_MAX_OUTPUT/MAXIMAL_SPEED)*ReferentSpeed;    
  }
  else if ( ReferentSpeed >= MAXIMAL_SPEED )
  { 
    //Set referent speed to maximal speed value.
    PIReg.s_SpeedReg.ReferentSpeed = PIW_REG_MAX_OUTPUT; 
  }
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
  
  //We want to calculate output of current PI regulator.
  if ( RegulatorType == CURRENT_REGULATOR )
  {
    //Catch last error value.
    LastError = PIReg.s_CurrentReg.Error; 
    
    //Current error value.
    PIReg.s_CurrentReg.Error = PIReg.s_CurrentReg.ReferentCurrent - PIReg.s_CurrentReg.MeasuredCurrent; 
    
    //Deference between these two errors.
    ErrorDiference = PIReg.s_CurrentReg.Error - LastError; 

    //Calculate increment (1.15 format)
    Increment = ( (int)((long)PIReg.s_CurrentReg.Kpi*(long)ErrorDiference           >> 15) + 
                  (int)((long)PIReg.s_CurrentReg.Kii*(long)PIReg.s_CurrentReg.Error >> 15)
                );
    
    //Set regulator output.
    PIReg.s_CurrentReg.Output += Increment;
   
    //Limit regulator output.
    if (PIReg.s_CurrentReg.Output      > PII_REG_MAX_OUTPUT)
    {
      PIReg.s_CurrentReg.Output        = PII_REG_MAX_OUTPUT;
    }
    else if (PIReg.s_CurrentReg.Output < PII_REG_MIN_OUTPUT)
    { 
      PIReg.s_CurrentReg.Output        = PII_REG_MIN_OUTPUT;
    }
   
#ifdef CURRENT_REGULATOR_TEST 

    //Print measured current.
    UART_v_Print(PIReg.s_CurrentReg.Output);    
    //Insert new line.
    UART_v_NewLine();
    
#endif
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
    
    //Calculate increment (3.13 format)
    Increment = ( (int)((long)PIReg.s_SpeedReg.Kpw*(long)ErrorDiference           >> 13) + 
                  (int)((long)PIReg.s_SpeedReg.Kiw*(long)PIReg.s_SpeedReg.Error   >> 13)
                );
    
    //Set regulator output.
    PIReg.s_SpeedReg.Output += Increment;
    
    //Limit regulator output.
    if (PIReg.s_SpeedReg.Output > PIW_REG_MAX_OUTPUT)
    {
      PIReg.s_SpeedReg.Output   = PIW_REG_MAX_OUTPUT;
    }
    if (PIReg.s_SpeedReg.Output < PIW_REG_MIN_OUTPUT)
    { 
      PIReg.s_SpeedReg.Output   = PIW_REG_MIN_OUTPUT;
    }
    
#ifdef SPEED_REGULATOR_TEST   
    
    //Print measured speed.
    UART_v_Print(PIReg.s_SpeedReg.Output);    
    //Insert new line.
    UART_v_NewLine();

#endif
  }
}

int i_ConvertToFixedPoint(int ADCBuffer, e_FormatTypes FormatType)
{ 
  int ToReturn = 0;
  
  if ( FormatType == FORMAT_1_15 )
  {
    // Covert to fixed point format 1.15. 
    // 0 - 1023 -> 0 - 32768
    ToReturn = (PII_REG_MAX_OUTPUT/1023)*ADCBuffer;
  }
  if ( FormatType == FORMAT_3_13 )
  {
    // Convert to fixed point format 3.13. 
    // 0 - 1023 -> 0 - 8192
    ToReturn = (PIW_REG_MAX_OUTPUT/1023)*ADCBuffer;
  }
  
  return ToReturn;
}