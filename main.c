/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdlib.h>
#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include "uart.h"
#include "adc.h"

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
//Variable of PI regulator type.
t_PIRegulatorData PIReg;

static void v_Delay(uint16_t u_NumberOfCounts);

int main(void)
{ 
  uint16_t u_MeasuredValue = 0u;
  
  //Set current referent value in Amps.
  float f_ReferentCurrent = f_SetReferentCurrent(NOMINAL_CURRENT);

  //Set referent speed value in round per minutes.
  float f_ReferentSpeed   = f_SetReferentSpeed(NOMINAL_SPEED);
  
  //Initialize all peripherals for dsPIC30F4011.
  dsPIC30F4011_v_Init();
  
  //Initialize DC machine parameters.
  DCM_v_Init();
  
  //Initialize sensor values.
  Sensor_v_Init();
  
  //Set current & speed PI regulator parameters.
  PIReg_v_Init();
  
  while(1)
  {
/********************************WRITING ON TERMINAL***************************/
    //Catch measurement value.
    u_MeasuredValue = ADC_v_Read(1u);
    
    //Print referent current.
    UART_v_Print(f_ReferentCurrent);		
    //Insert new line.
    UART_v_NewLine();		
    
    //Print referent speed.
    UART_v_Print(f_ReferentSpeed);
    //Insert new line.
    UART_v_NewLine();	
    
    //Print measured value.
    UART_v_Print(u_MeasuredValue);    
    //Insert new line.
    UART_v_NewLine();	
    
    //Delay writing on terminal.
    v_Delay(NUMBER_OF_COUNTS);
    

/******************************************************************************/
  }//while loop
}//main loop

void v_CalculatePIRegOutput(char Character)
{
 
  int LastError,ErrorDiference = 0;
  float Increment              = 0;
  
  //We want to calculate output of current PI regulator.
  if ( Character == 'i')
  {
    //Catch last error value.
    LastError = PIReg.s_CurrentReg.Error; 
 
    //Current error value.
    PIReg.s_CurrentReg.Error = PIReg.s_CurrentReg.ReferentCurrent - PIReg.s_CurrentReg.MeasuredCurrent; 
 
    //Deference between these two errors.
    ErrorDiference = PIReg.s_CurrentReg.Error - LastError; 
 
    //Calculate increment value.
    Increment = (PIReg.s_CurrentReg.Kpi*ErrorDiference) + (PIReg.s_CurrentReg.Kii*PIReg.s_CurrentReg.Error);
    
    //Set regulator output.
    PIReg.s_CurrentReg.Output += Increment;
 
    //Limit regulator output.
    if (PIReg.s_CurrentReg.Output > MAX_VALUE)
    {
      PIReg.s_CurrentReg.Output = MAX_VALUE;
    }
    if (PIReg.s_CurrentReg.Output > MIN_VALUE)
    { 
      PIReg.s_CurrentReg.Output = MIN_VALUE;
    }
  }
  
  //We want to calculate output of speed PI regulator.
  if ( Character == 'w')
  {
    //Catch last error value.
    LastError = PIReg.s_SpeedReg.Error;
    
    //Current error value.
    PIReg.s_SpeedReg.Error = PIReg.s_SpeedReg.ReferentSpeed - PIReg.s_SpeedReg.MeasuredSpeed;
    
    //Deference between these two errors.
    ErrorDiference = LastError - PIReg.s_SpeedReg.Error;
    
    //Calculate increment value.
    Increment = (PIReg.s_SpeedReg.Kpw*ErrorDiference) + (PIReg.s_SpeedReg.Kiw*PIReg.s_SpeedReg.Error);
    
    //Set regulator output.
    PIReg.s_SpeedReg.Output += Increment;
    
    //Limit regulator output.
    if (PIReg.s_SpeedReg.Output > MAX_VALUE)
    {
      PIReg.s_SpeedReg.Output = MAX_VALUE;
    }
    if (PIReg.s_SpeedReg.Output > MIN_VALUE)
    { 
      PIReg.s_SpeedReg.Output = MIN_VALUE;
    }
  }
}

static void v_Delay(uint16_t u_NumberOfCounts)
{
    uint16_t u_Counts = u_NumberOfCounts;
    while ( u_Counts > 0)
    {
      u_Counts--;
    }
}
/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */