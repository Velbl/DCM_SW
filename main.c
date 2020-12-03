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
#include <string.h>

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */
#include "uart.h"
#include "adc.h"

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int           a_CurrentMeasurements[NUMBER_OF_MEASUREMENTS];
int           a_SpeedMeasurements[NUMBER_OF_MEASUREMENTS];
unsigned long SystemTime            = 0u;  //50us  <- PWM period
unsigned long SystemTimeMs          = 0u;  //20 -> 1ms
long          MeasuredCurrentOffset = 0;
int           MeasuredCurrent       = 0;
int           MeasuredSpeed         = 0;
uint16_t      MeasurementCurrentIdx = 0u;   
uint16_t      MeasurementSpeedIdx   = 0u; 
bool          OffsetIsSpecified     = false;

static void v_Delay(long Duration);
  
int main(void)
{ 
  //Set current referent value.
  //f_SetReferentCurrent(9);                        //Enter value between -18[A]  to 18[A].

  //Set referent speed value.
  //f_SetReferentSpeed(2500);                       //Enter value from -3370[rpm] to 3370[rpm] .
  
  //Initialize all peripherals for dsPIC30F4011.
  dsPIC30F4011_v_Init();
  
  //Initialize speed measurement values.
  memset((void*)&a_SpeedMeasurements[0], 0 , sizeof(a_SpeedMeasurements));
  
  //Initialize current measurement values.
  memset((void*)&a_CurrentMeasurements[0], 0 , sizeof(a_CurrentMeasurements));
  
  v_Delay(ONE_SECOND);
  
  while(1)
  {
/********************************WRITING ON TERMINAL***************************/
   //Catch measurement value.
    //MeasuredCurrent = ADC_v_Read(1u);
    
#ifndef CURRENT_REGULATOR_TEST
    
    //Measured.Current = i_ConvertToFixedPoint(MeasuredCurrent, FORMAT_1_15);
    
    //Update measured current value.
    //PIReg.s_CurrentReg.MeasuredCurrent = Measured.Current;
    
    //Print referent current.
    UART_v_Print( PIReg.s_CurrentReg.ReferentCurrent);		
    //Insert new line.
    UART_v_NewLine();		
    
    //Print measured current.
    UART_v_Print( PIReg.s_CurrentReg.MeasuredCurrent);    
    //Insert new line.
    UART_v_NewLine();	
    
    //Print measured current.
    UART_v_Print( PIReg.s_CurrentReg.Output);    
    //Insert new line.
    UART_v_NewLine();	
    
    //Print measured current.
    UART_v_Print( PDC1 );    
    //Insert new line.
    UART_v_NewLine();	
        
    //v_CalculatePIRegOutput(CURRENT_REGULATOR);
#endif
    
#ifdef SPEED_REGULATOR_TEST

    //Measured.Speed =(8192/1023)*ADC_v_Read(1u);
    
    Measured.Speed = i_ConvertToFixedPoint(ADCBuffer, FORMAT_3_13);
    
    //Update measured current value.
    PIReg.s_SpeedReg.MeasuredSpeed = Measured.Speed;
    
    //Print referent speed.
    UART_v_Print(PIReg.s_SpeedReg.ReferentSpeed);		
    //Insert new line.
    UART_v_NewLine();		
    
    //Print measured current.
    UART_v_Print(PIReg.s_SpeedReg.MeasuredSpeed);    
    //Insert new line.
    UART_v_NewLine();	
        
    v_CalculatePIRegOutput(SPEED_REGULATOR);
#endif
    //Delay writing on terminal.
    v_Delay(ONE_SECOND);
  
/******************************************************************************/ 
 /*   if ( SystemTimeMs == SIX_SECONDS)
    {
      int MeasurementIndex;
      for (MeasurementIndex = 0; MeasurementIndex < NUMBER_OF_MEASUREMENTS; MeasurementIndex++)
      {
        UART_v_NewLine();
        UART_v_Print(a_MeasuredCurrents[MeasurementIndex]);     
      }
      CloseUART1();
     }
  * */
  }//while loop
}//main loop

static void v_Delay(long Duration)
{
  unsigned long StartTime; 
  StartTime = SystemTimeMs;
  while ( (SystemTimeMs - StartTime) < Duration);
}
/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */