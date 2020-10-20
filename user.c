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
/* User Variables                                                            */
/******************************************************************************/
static uint16_t u_DutyRatio = PWM_PERIOD;                    //Duty cycle = 50%
/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/
/*Definitions of all static functions*/
static void Ports_v_Init(void);
static void Timers_v_Init(void);
static void PWM_v_SetDutyCycle(uint16_t , uint16_t , uint8_t );
static void PWM_v_InterruptCfg(void);
static void PWM_v_PWMCfg(uint16_t);
static void PWM_v_Init(void);
static void ADC_v_Init(void);
/* <Initialize variables in user.h and insert code for user algorithms.> */
/**
 * This function updates the duty cycle register and updated disable bit.
 */

/*I/O PORTS configuration.*/
static void Ports_v_Init()
{
    /**
     * PORT B pins:
     * RB2 -> DC motor current  (ia_oa)
     * RB3 -> VDC               (vdc_oa)
     * RB4 -> Speed             (w_oa)
     * RB5 -> Potentiometer WP1
     * RB6 -> Potentiometer WP2
     */
    ADPCFG = 0x007C;          //Set RB2,RB3,RB4,RB5,RB6 as analog pins.
    TRISB  = 0xffff;          //Set all B pins as inputs. 
    /**
     * PORT E pins:
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
    TRISE  = 0xFFC0;            //Set RE0 ... RE5 pins as outputs.
    /**
     * Test timer interrupts and oscillator configuration via led outputs on PORT F.
     */
    TRISF  = 0xFFC0;            //Set RF0 ... RF5 pins as outputs.
}
/*TIMERS configuration*/
static void Timers_v_Init()
{
    /*Timer 1*/
    T1CONbits.TCS   = 0;      //internal clock FCY
    T1CONbits.TCKPS = 3;      //prescaler 1:256
    PR1   = 782;              //100ms
    _T1IP = 1;                //Timer 1 interrupt priority level = 1
    _T1IF = 0;                //Timer 1 interrupt flag status cleared.
    __write_to_IEC(IEC0bits.T1IE = 1);// proper way to enable/disable wanted interrupt
    T1CONbits.TON   = 1;      //timer 1 on
   
    /*Timer 2*/
    T2CONbits.TCS   = 0;      //internal clock FCY
    T2CONbits.TCKPS = 3;      //prescaler 1:256
    PR2 = 7812;               //1s
    _T2IP = 3;                //Timer 3 interrupt priority level = 3
    _T2IF = 0;                //Timer 3 interrupt flag status cleared.
    __write_to_IEC(IEC0bits.T2IE = 1);// proper way to enable/disable wanted interrupt
    T2CONbits.TON   = 1;      //timer 3 on   
}
/*PWM configuration*/
static void PWM_v_SetDutyCycle(uint16_t u_PDCx, uint16_t u_DutyCycle, uint8_t u_UpdateDisable)
{
  PWMCON2bits.UDIS = u_UpdateDisable & 0x01;
  /*UDIS bit is activated*/
  if ( PWMCON2bits.UDIS == 0x01u)
  {
    /* IUE bit must be disabled*/
    PWMCON2bits.IUE = 0x00u;     
  }
  /*Set wanted duty cycle value in wanted duty cycle register*/
  *(&PDC1+ u_PDCx - 1) = u_DutyCycle; 
}

static void PWM_v_InterruptCfg()
{   
    /*Clear the interrupt flags*/
    IFS2bits.PWMIF   = 0u;
    IFS2bits.FLTAIF  = 0u;
    /*Set priority for the PWM interrupt*/
    IPC9bits.PWMIP   = 1u;
    /*Set priority for the Fault A interrupt*/
    IPC10bits.FLTAIP = 1u;
    /*Enable of interrupt PWM*/
    IEC2bits.PWMIE   = 1u;
    /*Disable of interrupt Fault A*/
    IEC2bits.FLTAIE  = 0u;
}

static void PWM_v_PWMCfg(uint16_t u_Period)
{
    PTPER  = u_Period;
    /*PWM1H pin is enabled for PWM output*/
    PWMCON1bits.PEN1H = 0x01u;
    /*PWM1L pin is enabled for PWM output*/
    PWMCON1bits.PEN1L = 0x01u;
    /*PWM2H pin is enabled for PWM output*/
    PWMCON1bits.PEN2H = 0x01u;
    /*PWM2L pin is enabled for PWM output*/
    PWMCON1bits.PEN2L = 0x01u;
    /*PWM1H & PWM1L pins are in complementary mode*/
    PWMCON1bits.PMOD1 = 0x00u;
    /*PWM2H & PWM2L pins are in complementary mode*/
    PWMCON1bits.PMOD2 = 0x00u;
    /*PWM time base operates in a continuous up/down (triangular carrier) counting mode*/
    PTCONbits.PTMOD   = 3u;
    /*PWM time base input clock period is TCY (1:1 prescale)*/
    PTCONbits.PTCKPS0 = 0u;
    /*Interrupt generated every time when PTMR = 0 (in chosen PTMOD3) 1:1 Postscale*/
    PTCONbits.PTOPS   = 0u;
    /*PWM time base is ON*/
    PTCONbits.PTEN    = 0x01u;
}
static void PWM_v_Init()
{
  /**
   * Clear PWM interrupt flag.
   * Set PWM interrupt priority to 1.
   * Enable PWM interrupt.
   */
  PWM_v_InterruptCfg();
  /**
   * Set PDC1 channel.
   * Set initial duty cycle to 50%.
   * Disable UDIS bit.
   */
  PWM_v_SetDutyCycle(1,u_DutyRatio,0);
    /**
   * Set PDC2 channel.
   * Set initial duty cycle to 50%.
   * Disable UDIS bit.
   */
  PWM_v_SetDutyCycle(2,u_DutyRatio,0);
  /**
   * Set PTPER register value.
   * Enable PWM1H,PWM1L,PWM2H and PWM2L pins.
   * Set complementary mode for PWM pins.
   * Set continuous up/down counting mode for PWM timer.
   * Enable PWM timer.
   */
  PWM_v_PWMCfg(PWM_PERIOD);
}
/*ADC configuration*/
static void ADC_v_Init()
{
    
}

void dsPIC30F4011_v_Init()
{
    Ports_v_Init();
    Timers_v_Init();
    PWM_v_Init();
    ADC_v_Init();
}

//PR = FCY / (Fwanted * Prescaler) - 1