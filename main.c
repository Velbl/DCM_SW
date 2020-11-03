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
void v_1sDelay(void);
void v_500msDelay(void);
void UART_v_Busy(void);

int main(void)
{
  //Pointer to ADC buffer 0.
  uint16_t* p_ADCBuffer = (uint16_t*) &(ADCBUF0); 
  uint8_t u_BufferCnt;
  uint16_t u_ADCValue = 55u;

  /* Initialize IO ports and peripherals for dsPIC30F4011 */
  dsPIC30F4011_v_Init();
  v_1sDelay();
  
  while(1)
  {
    //v_500msDelay();
    U1TXREG = 220u;
    UART_v_Busy();

  }//while loop
}//main loop


void v_500msDelay(void)
{
  //Timer 1 is counting.
  while ( IFS0bits.T1IF != 1u )
  {
    //Do nothing.
  }
}

void v_1sDelay(void)
{
  //Timer 2 is counting. 
  while ( IFS0bits.T2IF != 1u )
  {
    //Do nothing.
  }
}

void UART_v_Busy(void)
{
    //Transmission is in progress.
    while ( U1STAbits.TRMT != 1u )
    {
      //Do nothing.
    }
}

/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */