/** Device header file 
 * The MPLAB X IDE generates a Makefile which controls the build process.
 * The C compiler will generate a pre-defined macro __XC16__ and also
  __dsPIC30F4011__. 
 */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "system.h"          /* variables/params used by system.c             */

void ConfigureOscillator(void)
{
#if 0
        /* Disable Watch Dog Timer */
        RCONbits.SWDTEN = 0;

        /* When clock switch occurs switch to Pri Osc controlled by FPR<4:0> */
        __builtin_write_OSCCONH(0x03);  /* Set OSCCONH for clock switch */
        __builtin_write_OSCCONL(0x01);  /* Start clock switching */
        while(OSCCONbits.COSC != 0b011);

        /* Wait for Clock switch to occur */
        /* Wait for PLL to lock, if PLL is used */
        /* while(OSCCONbits.LOCK != 1); */
#endif
}

