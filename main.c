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


/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int main(void)
{
  //Pointer to ADC buffer 0.
  uint16_t* p_ADCBuffer = (uint16_t*) &(ADCBUF0); 
  uint8_t u_BufferCnt;
  uint16_t u_ADCValue = 0u;
  
    
  /* Configure the oscillator for the device */
  ConfigureOscillator();
  
  /* Initialize IO ports and peripherals for dsPIC30F4011 */
  dsPIC30F4011_v_Init();

  while(1)
  {
    //SensorValues.u_DutyCycle1 = *(p_ADCBuffer);
    //SensorValues.u_DutyCycle2 = SensorValues.u_DutyCycle1;
    //Converting.
  /*  while (!IFS0bits.ADIF)
    {
      for (u_BufferCnt = 0; u_BufferCnt < 4u; u_BufferCnt++)
      {
          //Save converted value from ADC buffer.
          u_ADCValue = *p_ADCBuffer;
          
          //Move to next ADC buffer.
          p_ADCBuffer++;
          
          
          //Save converted values into sensor values structure.
          switch (u_BufferCnt)
          {
            case(0):
              //Update duty cycles.
              SensorValues.u_DutyCycle1   = u_ADCValue;
              SensorValues.u_DutyCycle2   = u_ADCValue;
              break;
            case(1):
              //Save current measurement.
              SensorValues.u_Current      = u_ADCValue;
              break;       
            case(2):
              //Save voltage measurement.
              SensorValues.u_VoltageDC    = u_ADCValue;
              break;
            case(3):
              //Save speed measurement.
              SensorValues.u_Speed        = u_ADCValue;
              break;
          }
      }
    }
    */
  }//while loop
}//main loop
//TODO: Configure UART peripheral.
//TODO: In each case, send data via UART to terminal.

/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */