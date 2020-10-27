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
  /* Configure the oscillator for the device */
  ConfigureOscillator();
  
  /* Initialize IO ports and peripherals for dsPIC30F4011 */
  dsPIC30F4011_v_Init();

  while(1)
  {

    
  }//while loop
}//main loop


/**INFO: Another solution for sharing memory problems.
 *__builtin_disi(0x3FFF);//disable all interrupts, except level 7 interrupts
 *protected code
 *__builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
 */