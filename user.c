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

// Convert Amps to 1.15 format.
void f_SetReferentCurrent(int ReferentCurrent)
{
  //Entered value is in the range.
  if ( (ReferentCurrent >= MINIMAL_CURRENT ) && (ReferentCurrent <= MAXIMAL_CURRENT) )
  {
    //Linearly scale entered current value. 
    PIReg.s_CurrentReg.ReferentCurrent = (32768/MAXIMAL_CURRENT)*ReferentCurrent;
  }
  else if ( ReferentCurrent >= MAXIMAL_CURRENT )
  { 
    // Set referent current to maximal current value.
    PIReg.s_CurrentReg.ReferentCurrent = 32768; 
  }
  else 
  {
    // Set referent current to minimal current value.
    PIReg.s_CurrentReg.ReferentCurrent = -32768;
  }
};

// Convert Rpm to 3.13 format.
void f_SetReferentSpeed(int ReferentSpeed)
{
  
  //Entered value is in the range.
  if ( (ReferentSpeed >= MINIMAL_SPEED ) && (ReferentSpeed <= MAXIMAL_SPEED) )
  { 
    // Convert RPM to 3.13 format.
    PIReg.s_SpeedReg.ReferentSpeed = (8192/3370)*ReferentSpeed;    
  }
  else if ( ReferentSpeed >= MAXIMAL_SPEED )
  { 
    // Set referent speed to maximal speed value.
    PIReg.s_SpeedReg.ReferentSpeed = 8192; 
  }
  else 
  {
    // Set referent speed to minimal speed value.
    PIReg.s_SpeedReg.ReferentSpeed = -8192;
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
    
    //Calculate increment value.
    //Increment = (PIReg.s_CurrentReg.Kpi*ErrorDiference + PIReg.s_CurrentReg.Kii*PIReg.s_CurrentReg.Error );
    
    //IN FIXED POINT ARITHMETIC (1.15 format)
    Increment = ( (int)((long)PIReg.s_CurrentReg.Kpi*(long)ErrorDiference           >> 15) + 
                  (int)((long)PIReg.s_CurrentReg.Kii*(long)PIReg.s_CurrentReg.Error >> 15)
                );
    
    //Set regulator output.
    PIReg.s_CurrentReg.Output += Increment;
   
    //Limit regulator output.
    if (PIReg.s_CurrentReg.Output > 32768)
    {
      PIReg.s_CurrentReg.Output = 32768;
    }
    else if (PIReg.s_CurrentReg.Output < -32768)
    { 
      PIReg.s_CurrentReg.Output = -32768;
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
    
    //Calculate increment value.
    Increment = ( (int)((long)PIReg.s_SpeedReg.Kpw*(long)ErrorDiference           >> 13) + 
                  (int)((long)PIReg.s_SpeedReg.Kiw*(long)PIReg.s_SpeedReg.Error   >> 13)
                );
    
    //Set regulator output.
    PIReg.s_SpeedReg.Output += Increment;
    
    //Limit regulator output.
    if (PIReg.s_SpeedReg.Output > 8192)
    {
      PIReg.s_SpeedReg.Output = 8192;
    }
    if (PIReg.s_SpeedReg.Output < -8192)
    { 
      PIReg.s_SpeedReg.Output = -8192;
    }
    
#ifdef SPEED_REGULATOR_TEST   
    
    //Print measured speed.
    UART_v_Print(PIReg.s_SpeedReg.Output);    
    //Insert new line.
    UART_v_NewLine();

#endif
  }
}