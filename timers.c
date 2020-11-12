/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif
#include "timers.h"

static void v_InitTimer1()
{
  T1CONbits.TCS   = 0;      //internal clock (FOSC/4)
  T1CONbits.TCKPS = 3;      //prescaler 1:256
  PR1   = 15384;            //500ms
  _T1IP = 2;                //Timer 1 interrupt priority level = 1
  _T1IF = 0;                //Timer 1 interrupt flag status cleared.
  __write_to_IEC(IEC0bits.T1IE = 1);// proper way to enable/disable wanted interrupt
  T1CONbits.TON   = 1;      //timer 1 on 
}

static void v_InitTimer2()
{
  T2CONbits.TCS   = 0;      //internal clock (FOSC/4)
  T2CONbits.TCKPS = 3;      //prescaler 1:256
  // PR = (Wanted Time) / (FOSC / TCS); TCS = FOSC/4 
  PR2 = 3076;               //100ms
  _T2IP = 2;                //Timer 2 interrupt priority level = 3
  _T2IF = 0;                //Timer 2 interrupt flag status cleared.
  __write_to_IEC(IEC0bits.T2IE = 1);// proper way to enable/disable wanted interrupt
  T2CONbits.TON   = 1;      //timer 3 on   
}

void Timers_v_Init()
{
  v_InitTimer1();
  v_InitTimer2();
}
