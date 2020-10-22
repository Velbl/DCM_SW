/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

static void v_InitTimer1(void);
static void v_InitTimer2(void);


static void v_InitTimer1()
{
  T1CONbits.TCS   = 0;      //internal clock FCY
  T1CONbits.TCKPS = 3;      //prescaler 1:256
  PR1   = 782;              //100ms
  _T1IP = 1;                //Timer 1 interrupt priority level = 1
  _T1IF = 0;                //Timer 1 interrupt flag status cleared.
  __write_to_IEC(IEC0bits.T1IE = 1);// proper way to enable/disable wanted interrupt
  T1CONbits.TON   = 1;      //timer 1 on
  
}

static void v_InitTimer2()
{
  /*Timer 2*/
  T2CONbits.TCS   = 0;      //internal clock FCY
  T2CONbits.TCKPS = 3;      //prescaler 1:256
  PR2 = 7812;               //1s
  _T2IP = 3;                //Timer 3 interrupt priority level = 3
  _T2IF = 0;                //Timer 3 interrupt flag status cleared.
  __write_to_IEC(IEC0bits.T2IE = 1);// proper way to enable/disable wanted interrupt
  T2CONbits.TON   = 1;      //timer 3 on   
}

void Timers_v_Init()
{
    v_InitTimer1();
    v_InitTimer2();
}
