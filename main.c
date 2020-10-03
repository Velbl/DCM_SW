/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>        /* Includes uint16_t definition                    */
#include <stdbool.h>       /* Includes true/false definition                  */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp              */

/******************************************************************************/
/* Global Variable Declaration                                                */
/******************************************************************************/

/* i.e. uint16_t <variable_name>; */

/******************************************************************************/
/* Main Program                                                               */
/******************************************************************************/
int main(void)
{
    /* Configure the oscillator for the device */
    ConfigureOscillator();

    /* Initialize IO ports and peripherals */
    InitApp();

    /* TODO <INSERT USER APPLICATION CODE HERE> */
    IFS0bits.T1IF   = 0;    //clear the flag
    //IEC0bits.T1IE   = 1;    //enable the interrupt (unknown needed number of cycles)
    __write_to_IEC(IEC0bits.T1IE = 1);// proper way of enabling/disabling the wanted interrupt
    T1CONbits.TON   = 1;    //timer 1 off
    
    __builtin_disi(0x3FFF);//disable all interrupts
    //protected code
    __builtin_disi(0x0000);//enable all interrupts or using DISICNT = 0x0000;
    while(1)
    {
      
    }//while loop
}//main loop
