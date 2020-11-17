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
#define DELAY_105uS asm volatile ("REPEAT, #4201"); Nop(); // 105uS delay

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
static void v_100msDelay(void);
static void v_500msDelay(void);

void UART_v_Busy(void);

int main(void)
{ 
  //Initialize all peripherals for dsPIC30F4011.
  dsPIC30F4011_v_Init();
  
  //Initialize DC machine parameters.
  DCM_v_Init();
  
  //Initialize sensor values.
  Sensor_v_Init();
  
  //Set current & speed PI regulator parameters.
  PIReg_v_SetParameters();
  
  while(1)
  {
/********************************WRITING ON TERMINAL***************************/

    //Start signal.
    U1TXREG = 255u; 
    
    //The last transmission has completed.
    if ( U1STAbits.TRMT == 1)
    {
      //Write Ia measured.
      U1TXREG = 0;

      //Write Ua measured.
      U1TXREG = 1;

      //Write measured speed.
      U1TXREG = 2;

      //Write parameter reference from potentiometer.
      U1TXREG = (PDC1/2);
    }
    v_100msDelay();


/******************************************************************************/
  }//while loop
}//main loop

static void v_500msDelay(void)
{
  //Timer 1 is counting.
  while ( IFS0bits.T1IF != 1u )
  {
    //Do nothing.
  }
}



static void v_100msDelay(void)
{
  //Timer 2 is counting. 
  while ( IFS0bits.T2IF != 1u )
  {
    //Do nothing.
  }
}

/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */