/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "user_cfg.h"
#include "pwm.h"

void PWM_v_SetDutyCycle(uint16_t u_PDCx, uint16_t u_DutyRatio, uint8_t u_UpdateDisable)
{
  PWMCON2bits.UDIS = u_UpdateDisable & 0x01; // Updates from duty cycle and period buffer registers are disabled.
  
  *(&PDC1+ u_PDCx - 1) = u_DutyRatio;        // Set wanted duty cycle value in wanted duty cycle register.
  
  PWMCON2bits.UDIS = 0u;                     // Updates from duty cycle and period buffer registers are enabled.
}

static void v_Config(uint16_t u_Period)
{
  PTCONbits.PTEN     = 0u;    // PWM time base is OFF.
  PTPER = u_Period;           // Set PWM frequency to 20KHz.

/*******************************PTCON register*********************************/
  PTCONbits.PTSIDL   = 1u;    // PWM time base halts in CPU Idle mode.
  PTCONbits.PTOPS    = 0x00u; // 1:1 Postscale.
  PTCONbits.PTCKPS   = 0x00u; // PWM time bas input clock period is Tcy (1:1 prescale).
  PTCONbits.PTMOD    = 0x02u; // PWM time base operates in a continuous up/down.
/******************************************************************************/
  
/******************************PWMCON1 register********************************/
  PWMCON1bits.PMOD1  = 0u;    // PWM I/O pin pair is in the complementary output mode.
  PWMCON1bits.PMOD2  = 0u;    // PWM I/O pin pair is in the complementary output mode
  PWMCON1bits.PEN1H  = 1u;    // PWM1H pin is enabled for PWM output.
  PWMCON1bits.PEN1L  = 1u;    // PWM1L pin is enabled for PWM output.
  PWMCON1bits.PEN2H  = 1u;    // PWM2H pin is enabled for PWM output.
  PWMCON1bits.PEN2L  = 1u;    // PWM2L pin is enabled for PWM output.
/******************************************************************************/
  
/*******************************PWMCON2 register*******************************/
  PWMCON2bits.SEVOPS = 0x01u; // PWM Special Event Trigger Output Postscale bit is set to 1.
  PWMCON2bits.OSYNC  = 1u;    // Output overrides via the OVDCON register are synchronized to the PWM time base.
  PWMCON2bits.UDIS   = 0u;    // Updates from duty cycle and period buffer registers are enabled.
/******************************************************************************/
  
  DTCON1bits.DTA    = 0x00u;  // Clock period for Dead Time Unit A is TCY.

/******************************OVDCON register*********************************/
  OVDCONbits.POVD1H = 1u;     // Output on PWM1H I/O pin is controlled by the PWM generator.
  OVDCONbits.POVD1L = 1u;     // Output on PWM1L I/O pin is controlled by the PWM generator.
  OVDCONbits.POVD2H = 1u;     // Output on PWM2H I/O pin is controlled by the PWM generator.
  OVDCONbits.POVD2L = 1u;     // Output on PWM2l I/O pin is controlled by the PWM generator.
/******************************************************************************/
  
/*******************************PWM INTERRUPT CONFIG***************************/
  IFS2bits.PWMIF      = 0u;   // Clear the interrupt flags.
  SEVTCMPbits.SEVTDIR = 1u;   // A special event trigger will occur when the PWM time base is counting downwards.
  IPC9bits.PWMIP      = 0x05u;// We MUST trigger ADC conversion in PWM interrupt.
  IEC2bits.PWMIE      = 1u;   // Enable PWM interrupt.
/******************************************************************************/ 
  
  PTCONbits.PTEN    = 1u;     // PWM time base is ON.
}

void PWM_v_Config()
{ 
  PWM_v_SetDutyCycle(1,PWM_PERIOD,0); // Set initial duty cycle in PDC1 register to 50%.
  
  PWM_v_SetDutyCycle(2,PWM_PERIOD,0); // Set initial duty cycle in PDC2 register to 50%.

  v_Config(PWM_PERIOD);
}


