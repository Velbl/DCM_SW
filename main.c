/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdlib.h>
#include <stdint.h>      
#include <stdbool.h>      

#include "system.h" 
#include "user.h" 
#include "uart.h"

/// Global variable for storing global system time in microseconds.
/// In every PWM cycle (50us period) this variable is increased.
unsigned long SystemTime            = 0u; 

/// Global variable for storing global system time in milliseconds.
/// In every 20th PWM cycle (1ms period) this variable is increased.
unsigned long SystemTimeMs          = 0u;

/// Global variable for storing information about current system state.
volatile tStates State = NOT_INITIALIZED;

//PI regulators parameters.
extern t_PIRegulatorData  PIReg;

// Delay function.
// User define delay duration by inserting wanted delay in milliseconds.
static void v_Delay(long Duration);
  
int main(void)
{ 
  v_InitState();
  
  v_Delay(ONE_SECOND);
  
  while(1)
  {
    v_IdleState();
  }
}

void v_Delay(long Duration)
{
  unsigned long StartTime; 
  
  // Catch current system time when function is called.
  StartTime = SystemTimeMs;
  
  // While system time, measured from calling this function, is not greater than wanted duration, wait and do nothing.
  while ( (SystemTimeMs - StartTime) < Duration);
}

void v_InitState()
{
  // Update current active state.
  SetState(INIT_STATE);
  
  // Initialize all the peripherals.
  dsPIC30F4011_v_Config();
}

void v_IdleState()
{   
  // Update current active state.
  SetState(IDLE_STATE);
  
/********************CURRENT PI REGULATOR INFORMATIONS*************************/
#ifdef CURRENT_REG
  
  // Print the magic key which marks the beginning of printing current data.
  UART_v_Print( 0x01 );		
  // Insert new line.
  UART_v_NewLine();		
    
  // Print referent current.
  UART_v_Print( PIReg.s_CurrentReg.ReferentCurrent);		
  // Insert new line.
  UART_v_NewLine();		
    
  // Print measured current.
  UART_v_Print( PIReg.s_CurrentReg.MeasuredCurrent);    
  // Insert new line.
  UART_v_NewLine();	
    
  // Print output current from current PI regulator.
  UART_v_Print( PIReg.s_CurrentReg.Output);    
  //Insert new line.
  UART_v_NewLine();	
  
#endif // CURRENT_REG
/******************************************************************************/

/********************SPEED PI REGULATOR INFORMATIONS***************************/
#ifdef SPEED_REG
  
  // Print the magic key which marks the beginning of printing speed data.
  UART_v_Print( 0x02 );		
  //Insert new line.
  UART_v_NewLine();		
    
  // Print referent speed.
  UART_v_Print( PIReg.s_SpeedReg.ReferentSpeed);		
  // Insert new line.
  UART_v_NewLine();		
    
  // Print measured speed.
  UART_v_Print( PIReg.s_SpeedReg.MeasuredSpeed);    
  // Insert new line.
  UART_v_NewLine();	
   
  // Print output speed from speed PI regulator.
  UART_v_Print( PIReg.s_SpeedReg.Output);    
  //Insert new line.
  UART_v_NewLine();
  
#endif // SPEED_REG
/******************************************************************************/
  
  // Delay writing on terminal for one second.
  v_Delay(ONE_SECOND);
}

void SetState(tStates NewState)
{
  // Update current active state.
  State = NewState;
}

tStates GetState(void)
{
  // Return updated active state.
  return State;
}

/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */