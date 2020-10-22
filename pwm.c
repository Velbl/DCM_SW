/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif
#include "pwm.h"
#include "user.h"
#include "adc.h" 

static void PWM_v_SetDutyCycle(uint16_t , uint16_t , uint8_t );
static void PWM_v_InterruptConfig(void);
static void PWM_v_PWMConfig(uint16_t);

/*Set duty cycle via potentiometer.*/
uint16_t u_DutyCycle = PWM_PERIOD;


static void PWM_v_SetDutyCycle(uint16_t u_PDCx, uint16_t u_DutyRatio, uint8_t u_UpdateDisable)
{
  PWMCON2bits.UDIS = u_UpdateDisable & 0x01;
  /*UDIS bit is activated*/
  if ( PWMCON2bits.UDIS == 0x01u)
  {
    /* IUE bit must be disabled*/
    PWMCON2bits.IUE = 0x00u;     
  }
  /*Set wanted duty cycle value in wanted duty cycle register*/
  *(&PDC1+ u_PDCx - 1) = u_DutyRatio; 
}

static void PWM_v_InterruptConfig()
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

static void PWM_v_PWMConfig(uint16_t u_Period)
{
  /*Set to 20KHz*/
  PTPER  = u_Period;
  /*PWM1H pin is enabled for PWM output*/
  PWMCON1bits.PEN1H = 0x01u;
  /*PWM1L pin is enabled for PWM output*/
  PWMCON1bits.PEN1L = 0x01u;
  /*PWM2H pin is enabled for PWM output*/
  PWMCON1bits.PEN2H = 0x01u;
  /*PWM2L pin is enabled for PWM output*/
  PWMCON1bits.PEN2L = 0x01u;
  /*PWM3H pin is disabled for PWM output*/
  PWMCON1bits.PEN3H = 0x00u;
  /*PWM3L pin is disabled for PWM output*/
  PWMCON1bits.PEN3L = 0x00u;
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

void PWM_v_Init()
{
  /**
   * Clear PWM interrupt flag.
   * Set PWM interrupt priority to 1.
   * Enable PWM interrupt.
   */
  PWM_v_InterruptConfig();
  /**
   * Set PDC1 channel.
   * Set initial duty cycle to 50%.
   * Disable UDIS bit.
   */
  PWM_v_SetDutyCycle(1,u_DutyCycle,0);
    /**
   * Set PDC2 channel.
   * Set initial duty cycle to 50%.
   * Disable UDIS bit.
   */
  PWM_v_SetDutyCycle(2,u_DutyCycle,0);
  /**
   * Set PTPER register value.
   * Enable PWM1H,PWM1L,PWM2H and PWM2L pins.
   * Set complementary mode for PWM pins.
   * Set continuous up/down counting mode for PWM timer.
   * Enable PWM timer.
   */
  PWM_v_PWMConfig(PWM_PERIOD);
}


