/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "timers.h"

static void v_ConfigTimer1()
{
  T1CONbits.TCS   = 0;      // Internal clock (FOSC/4)
  T1CONbits.TCKPS = 3;      // Prescaler 1:256
  PR1   = 15384;            // 500ms
  _T1IP = 2;                // Timer 1 interrupt priority level = 1
  _T1IF = 0;                // Timer 1 interrupt flag status cleared.
  __write_to_IEC(IEC0bits.T1IE = 1); // Enable Timer 1 interrupt.
  T1CONbits.TON   = 1;      // Turn on Timer 1. 
}

static void v_ConfigTimer2()
{
  T2CONbits.TCS   = 0;      // Internal clock (FOSC/4)
  T2CONbits.TCKPS = 3;      // Prescaler 1:256
  // PR = (Wanted Time) / (FOSC / TCS); TCS = FOSC/4 
  PR2 = 3076;               // 100ms
  _T2IP = 2;                // Timer 2 interrupt priority level = 3
  _T2IF = 0;                // Timer 2 interrupt flag status cleared.
  __write_to_IEC(IEC0bits.T2IE = 1); // Enable Timer 2 interrupt.
  T2CONbits.TON   = 1;      // Turno on Timer 2.   
}

void Timers_v_Config()
{
  v_ConfigTimer1();
  v_ConfigTimer2();
}
