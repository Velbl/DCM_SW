/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h>        
#include "user.h"
#include "pwm.h"
#include "adc.h"
/******************************************************************************/
/* Interrupt Vector Options                                                   */
/******************************************************************************/
/*                                                                            */
/* Refer to the C30 (MPLAB C Compiler for PIC24F MCUs and dsPIC33F DSCs)      */
/* MPLAB Assembler, Linker and Utilities for PIC24F MCUs and dsPIC DSCs       */
/* User's Guide for an up to date list of the available interrupt options.    */
/* Alternately these names can be pulled from the device linker scripts.      */
/*                                                                            */
/* Primary Interrupt Vector Names:                                            */
/*                                                                            */
/* _INT0Interrupt  _INT2Interrupt                                             */
/* _IC1Interrupt   _U2RXInterrupt                                             */
/* _OC1Interrupt   _U2TXInterrupt                                             */
/* _T1Interrupt    _SPI2Interrupt                                             */
/* _IC2Interrupt   _C1Interrupt                                               */
/* _OC2Interrupt   _IC3Interrupt                                              */
/* _T2Interrupt    _IC4Interrupt                                              */
/* _T3Interrupt    _IC5Interrupt                                              */
/* _SPI1Interrupt  _IC6Interrupt                                              */
/* _U1RXInterrupt  _OC5Interrupt                                              */
/* _U1TXInterrupt  _OC6Interrupt                                              */
/* _ADCInterrupt   _OC7Interrupt                                              */
/* _NVMInterrupt   _OC8Interrupt                                              */
/* _SI2CInterrupt  _INT3Interrupt                                             */
/* _MI2CInterrupt  _INT4Interrupt                                             */
/* _CNInterrupt    _C2Interrupt                                               */
/* _INT1Interrupt  _PWMInterrupt                                              */
/* _IC7Interrupt   _QEIInterrupt                                              */
/* _IC8Interrupt   _DCIInterrupt                                              */
/* _OC3Interrupt   _LVDInterrupt                                              */
/* _OC4Interrupt   _FLTAInterrupt                                             */
/* _T4Interrupt    _FLTBInterrupt                                             */
/* _T5Interrupt                                                               */
/*                                                                            */
/* For alternate interrupt vector naming, simply add 'Alt' between the prim.  */
/* interrupt vector name '_' and the first character of the primary interrupt */
/* vector name.                                                               */
/*                                                                            */
/* For example, the vector name _ADC2Interrupt becomes _AltADC2Interrupt in   */
/* the alternate vector table.                                                */
/*                                                                            */
/* Example Syntax:                                                            */
/*                                                                            */
/* void __attribute__((interrupt,auto_psv)) <Vector Name>(void)               */
/* {                                                                          */
/*     <Clear Interrupt Flag>                                                 */
/* }                                                                          */
/*                                                                            */
/* For more comprehensive interrupt examples refer to the C30 (MPLAB C        */
/* Compiler for PIC24 MCUs and dsPIC DSCs) User Guide in the                  */
/* <compiler installation directory>/doc directory for the latest compiler    */
/* release. For XC16, refer to the MPLAB XC16 ASSEMBLER, LINKER AND UTILITIES */
/* User's Guide within the <XC16 compiler instal directory>/doc folder.  The  */
/* chapter to refer is entitled "INTERRUPT VECTOR TABLES"                     */

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void)
{
  LATFbits.LATF0 ^= 1;               //Toggle RF0 pin 
  IFS0bits.T1IF   = 0;               //Reset the flag
}

void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void)
{
  LATFbits.LATF1 ^= 1;              //Toggle RF1 pin
  IFS0bits.T2IF   = 0;              //Reset the flag
}

void __attribute__((interrupt,no_auto_psv)) _PWMInterrupt(void)
{
  uint16_t u_ADCBuffer = 0u;
  
  //A/D is currently filling buffer 0x08-0x0F.
  if ( ADCON2bits.BUFS == 1u )
  {
   //Access data in buffer 0x03.
    u_ADCBuffer = ADC_v_Read(3u);
    
    PDC1 = ((2*PWM_PERIOD/400u) * u_ADCBuffer);                //Set duty cycle reference via potentiometer.
    PDC2 = PDC1;
  }
  
  /*Enable entering to PWM interrupt routine, next time*/
  IFS2bits.PWMIF = 0;
}

void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{

}
//INFO: XC16 Compiler User's Guide - Interrupts Section 14.