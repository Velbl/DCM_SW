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

int main(void)
{ 
  
  //Set current referent value in relative units.
  f_SetReferentCurrent(9);     //Enter from -18A  to 18A.

  //Set referent speed value in relative units.
  f_SetReferentSpeed(2500);     //Enter from -3370rpm to 3370rpm .
  
  //Initialize all peripherals for dsPIC30F4011.
  dsPIC30F4011_v_Init();
  
  while(1)
  {
/********************************WRITING ON TERMINAL***************************/

#ifdef CURRENT_REGULATOR_TEST
    
    //Catch measurement value. (linear scaling to 1.15 format)
    Measured.Current = (32768/1023)*ADC_v_Read(1u);//(NOMINAL_CURRENT/1023)*ADC_v_Read(1u);
    //Update measured current value.
    PIReg.s_CurrentReg.MeasuredCurrent = Measured.Current;
    
    //Print referent current.
    UART_v_Print(PIReg.s_CurrentReg.ReferentCurrent);		
    //Insert new line.
    UART_v_NewLine();		
    
    //Print measured current.
    UART_v_Print(PIReg.s_CurrentReg.MeasuredCurrent);    
    //Insert new line.
    UART_v_NewLine();	
        
    v_CalculatePIRegOutput(CURRENT_REGULATOR);
#endif
    
#ifdef SPEED_REGULATOR_TEST

    //Catch measurement value. (linear scaling)
    Measured.Speed =(8192/1023)*ADC_v_Read(1u);//(NOMINAL_CURRENT/1023)*ADC_v_Read(1u);
    
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