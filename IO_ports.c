/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "IO_ports.h"
#include "user.h"

static void v_GPIOPins(void);
static void v_ControlPins(void);
static void v_SensorPins(void);



static void v_GPIOPins()
{
  /**
   * Test timer interrupts and oscillator configuration via led outputs on PORT F.
   */
  /*Set RF0 ... RF5 pins as outputs.*/
  TRISFbits.TRISF0 = 0u; 
  TRISFbits.TRISF1 = 0u;
  TRISFbits.TRISF2 = 0u;
  TRISFbits.TRISF3 = 0u;
  TRISFbits.TRISF4 = 0u;
  TRISFbits.TRISF5 = 0u;
}
static void v_ControlPins()
{
 /**
   * PWM:
   * RE0 -> G1
   * RE1 -> G2
   * RE2 -> G3
   * RE3 -> G4
   *
   * MOSFET:
   * RE4 -> G5 
   * 
   * Thyristor:
   * RE5 -> GT1 
   */
  /*Set RE0 ... RE5 pins as outputs.*/
  TRISE |= ( MASK(0) | MASK(1) | MASK(2) | MASK(3) | MASK(4) | MASK(5));
  /*
  TRISEbits.TRISE0 = 0u; 
  TRISEbits.TRISE1 = 0u;
  TRISEbits.TRISE2 = 0u;
  TRISEbits.TRISE3 = 0u;
  TRISEbits.TRISE4 = 0u;
  TRISEbits.TRISE5 = 0u; 
  */
}
static void v_SensorPins()
{
  uint8_t u_BitsCnt;
  uint8_t u_NumberOfBits = 8u;
 /**
   * PORT B pins:
   * RB2 -> DC motor current  (ia_oa)
   * RB3 -> VDC               (vdc_oa)
   * RB4 -> Speed             (w_oa)
   * RB5 -> Potentiometer WP1
   * RB6 -> Potentiometer WP2
   */
  /*Set RB2,RB3,RB4,RB5,RB6 as analog pins.*/
  //ADPCFG |= ( MASK(2) | MASK(3) | MASK(4) | MASK(5) | MASK(6) );
  ADPCFG = 0x007C;
  //for ( u_BitsCnt = 0u; u_BitsCnt < u_NumberOfBits ; u_BitsCnt++ )
  //{
    //TRISB |= MASK(u_BitsCnt);//Set all B pins as inputs. 
  //}
  TRISB  = 0xffff;          //Set all B pins as inputs. 
}

void Ports_v_Init()
{
  v_GPIOPins(); 
  v_ControlPins();
  v_SensorPins();
}