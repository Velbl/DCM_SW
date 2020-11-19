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
uint16_t a_ADCBuffers[NUMBER_OF_USED_BUFFERS] = {0u};

static void v_Delay(uint16_t u_NumberOfCounts);
static void v_LoadADCBuffers(void);

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
  //A/D is currently filling buffer 0x08-0x0F.
  if ( ADCON2bits.BUFS == 1u )
  {
    v_LoadADCBuffers();

		UART_v_Print(a_ADCBuffers[3u]);		
    
    UART_v_NewLine();		
    
    v_Delay(NUMBER_OF_COUNTS);
   }
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

static void v_LoadADCBuffers()
{  
  uint8_t BufferCnt;
  for ( BufferCnt = 0u; BufferCnt < NUMBER_OF_USED_BUFFERS; BufferCnt++ )
  {
    a_ADCBuffers[BufferCnt] = ADC_v_Read(BufferCnt);
  }
}
/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */