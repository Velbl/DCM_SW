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
unsigned long SystemTime            = 0u;  //50us  <- PWM period
unsigned long SystemTimeMs          = 0u;  //20 -> 1ms
int           MeasuredCurrent       = 0;
int           MeasuredSpeed         = 0;

tStates State = NOT_INITIALIZED;

typedef void(* StateFunction)(void);


void v_InitState(void);
void v_IdleState(void);

static void v_Delay(long Duration);
  
int main(void)
{ 
 
  v_InitState();
  
  v_Delay(ONE_SECOND);
  
  while(1)
  {
    v_IdleState();
  }//while loop
}//main loop

static void v_Delay(long Duration)
{
  unsigned long StartTime; 
  StartTime = SystemTimeMs;
  while ( (SystemTimeMs - StartTime) < Duration);
}

void v_InitState()
{
  State = INIT_STATE;
  
  //Initialize all peripherals for dsPIC30F4011.
  dsPIC30F4011_v_Init();
 
}

void v_IdleState()
{    
    State = IDLE_STATE;

    //Print current data.
    UART_v_Print( 0x01);		
    //Insert new line.
    UART_v_NewLine();		
    
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

    //Delay writing on terminal.
    v_Delay(ONE_SECOND);
    
    //Print speed data.
    UART_v_Print( 0x02);		
    //Insert new line.
    UART_v_NewLine();	
    
    //Print referent current.
    UART_v_Print( PIReg.s_SpeedReg.ReferentSpeed);		
    //Insert new line.
    UART_v_NewLine();		
    
    //Print measured current.
    UART_v_Print( PIReg.s_SpeedReg.MeasuredSpeed);    
    //Insert new line.
    UART_v_NewLine();	
    
    //Print measured current.
    UART_v_Print( PIReg.s_SpeedReg.Output);    
    //Insert new line.
    UART_v_NewLine();	
    
    //Print duty cycle value.
    UART_v_Print( PDC1 );    
    //Insert new line.
    UART_v_NewLine();
    
    //Delay writing on terminal.
    v_Delay(ONE_SECOND);
    
}
/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */