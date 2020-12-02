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

static void v_Delay(uint16_t u_NumberOfCounts);
static i_ConvertToFixedPoint(int ADCBuffer, e_FormatTypes FormatType);

int main(void)
{ 
  // Buffer for holding measured values.
  int ADCBuffer = 0;
  
  //Set current referent value.
  f_SetReferentCurrent(9);                        //Enter value between -18[A]  to 18[A].

  //Set referent speed value.
  f_SetReferentSpeed(2500);                       //Enter value from -3370[rpm] to 3370[rpm] .
  
  //Initialize all peripherals for dsPIC30F4011.
  dsPIC30F4011_v_Init();
  
  while(1)
  {
/********************************WRITING ON TERMINAL***************************/
    //Catch measurement value.
    ADCBuffer = ADC_v_Read(1u);
    
#ifdef CURRENT_REGULATOR_TEST
    
    Measured.Current = i_ConvertToFixedPoint(ADCBuffer, FORMAT_1_15);
    
    //Update measured current value.
    PIReg.s_CurrentReg.MeasuredCurrent = Measured.Current;
    
    //Print referent current.
    UART_v_Print( PIReg.s_CurrentReg.ReferentCurrent);		
    //Insert new line.
    UART_v_NewLine();		
    
    //Print measured current.
    UART_v_Print( PIReg.s_CurrentReg.MeasuredCurrent);    
    //Insert new line.
    UART_v_NewLine();	
        
    v_CalculatePIRegOutput(CURRENT_REGULATOR);
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
    v_Delay(NUMBER_OF_COUNTS);
     
/******************************************************************************/  
  }//while loop
}//main loop

static i_ConvertToFixedPoint(int ADCBuffer, e_FormatTypes FormatType)
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