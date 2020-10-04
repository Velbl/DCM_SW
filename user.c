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

#include <stdint.h>          /* For uint32_t definition                       */
#include <stdbool.h>         /* For true/false definition                     */

#include "user.h"            /* variables/params used by user.c               */
#include "system.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

/* <Initialize variables in user.h and insert code for user algorithms.> */

/* TODO Initialize User Ports/Peripherals/Project here */

void InitApp(void)
{
    /*I/O ports configuration.*/
    TRISF = 0xfff0;           //Set F0,F1,F2,F3 as outputs.
    TRISE = 0xffff;
    
    /*Timer 1 configuration.*/
    T1CONbits.TCS   = 0;      //internal clock FCY
    T1CONbits.TCKPS = 3;      //prescaler 1:256
    PR1   = 782;               //100ms
    _T1IP = 1;                //Timer 1 interrupt priority level = 1
    _T1IF = 0;                //Timer 1 interrupt flag status cleared.
    __write_to_IEC(IEC0bits.T1IE = 1);// proper way to enable/disable wanted interrupt
    T1CONbits.TON   = 1;      //timer 1 on
    
    /*Timer 2 configuration.*/
    T2CONbits.TCS   = 0;      //internal clock FCY
    T2CONbits.TCKPS = 3;      //prescaler 1:256
    PR2 = 3906;               //0.5s
    _T2IP = 2;                //Timer 2 interrupt priority level = 2
    _T2IF = 0;                //Timer 1 interrupt flag status cleared.
    __write_to_IEC(IEC0bits.T2IE = 1);// proper way to enable/disable wanted interrupt
    T2CONbits.TON   = 1;      //timer 2 on
    
    /*Timer 3 configuration.*/
    T3CONbits.TCS   = 0;      //internal clock FCY
    T3CONbits.TCKPS = 3;      //prescaler 1:256
    PR3 = 7812;               //1s
    _T3IP = 3;                //Timer 3 interrupt priority level = 3
    _T3IF = 0;                //Timer 3 interrupt flag status cleared.
    __write_to_IEC(IEC0bits.T3IE = 1);// proper way to enable/disable wanted interrupt
    T3CONbits.TON   = 1;      //timer 3 on 
}

//PR = FCY / (Fwanted * Prescaler) - 1