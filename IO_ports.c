/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "IO_ports.h"

static void v_GPIOPins()
{
/****************************TEST PINS*****************************************/
  TRISFbits.TRISF0 = 0u;  // Set RF0 pin as output.
  TRISFbits.TRISF1 = 0u;  // Set RF1 pin as output.
  TRISFbits.TRISF2 = 0u;  // Set RF2 pin as output.
  TRISFbits.TRISF3 = 0u;  // Set RF3 pin as output.
  TRISFbits.TRISF4 = 0u;  // Set RF4 pin as output.
  TRISFbits.TRISF5 = 0u;  // Set RF5 pin as output.
/******************************************************************************/
}

static void v_ControlPins()
{
/*****************************PWM PINS*****************************************/
  TRISEbits.TRISE0 = 0u;  // Set RE0 pin as output.(PWM1L)
  TRISEbits.TRISE1 = 0u;  // Set RE1 pin as output.(PWM1H)
  TRISEbits.TRISE2 = 0u;  // Set RE2 pin as output.(PWM2L)
  TRISEbits.TRISE3 = 0u;  // Set RE3 pin as output.(PWM2H)
/******************************************************************************/
  
  TRISEbits.TRISE4 = 0u;  // Set RE4 pin as output.Pin for MOSFET control.
  TRISEbits.TRISE5 = 0u;  // Set RE5 pin as output.Pin for Thyristor control.
}
static void v_SensorPins()
{
/*****************************SENSOR PINS**************************************/
  TRISBbits.TRISB0 = 1u;  // Set RB0 pin as input.(current)
  TRISBbits.TRISB1 = 1u;  // Set RB1 pin as input.(voltage)
  TRISBbits.TRISB2 = 1u;  // Set RB2 pin as input.(speed)
  TRISBbits.TRISB3 = 1u;  // Set RB3 pin as input.(ref current/speed)
/******************************************************************************/
}

void Ports_v_Config()
{
  v_GPIOPins(); 
  v_ControlPins();
  v_SensorPins();
}