/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h> 
#include <stdbool.h>       /* Includes true/false definition                  */
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
extern int           a_MeasuredCurrents[NUMBER_OF_MEASUREMENTS];
extern unsigned long SystemTime;
extern unsigned long SystemTimeMs;
extern int           MeasuredCurrent;
extern long          MeasuredCurrentOffset;
extern int           MeasuredCurrent;
extern bool          OffsetIsSpecified;
extern uint16_t      Measurement; 

#define CURRENT_START_TIME (2000)
#define CURRENT_STOP_TIME  (3000)

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
  long Temp;
  SystemTime++;
  
  // One milisecond has passed.
  if ( (SystemTime % 20u) == 0u)
  {
    // Increase system time in miliseconds.
    SystemTimeMs++;
  }
/************************************SPEED   LOOP**********************************************/
  
  
/**********************************************************************************************/
/************************************CURRENT LOOP**********************************************/
  // Read current measurement
  while (BusyADC1());
  MeasuredCurrent = ADC_v_Read(1u);
  
  PIReg.s_CurrentReg.MeasuredCurrent = i_ConvertToFixedPoint(MeasuredCurrent, FORMAT_1_15);

  f_SetReferentCurrent(9);

  // Take offset into count.
  PIReg.s_CurrentReg.MeasuredCurrent = PIReg.s_CurrentReg.MeasuredCurrent;
      
  v_CalculatePIRegOutput(CURRENT_REGULATOR);
      
  // Scale output value to PERIOD register value.
  Temp = (long)PWM_PERIOD*(long)PIReg.s_CurrentReg.Output;
      
  // Update duty cycle value.
  PDC1 = ( (int)(PWM_PERIOD >> 1) + (int)(Temp >> 16) ) << 1;
  PDC2 = PDC1;
      
  if ( Measurement < NUMBER_OF_MEASUREMENTS )
  {
     a_MeasuredCurrents[Measurement] = PIReg.s_CurrentReg.MeasuredCurrent;
     Measurement++;
  }
/**********************************************************************************************/
  IFS2bits.PWMIF = 0;
}

void __attribute__((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{

}
//INFO: XC16 Compiler User's Guide - Interrupts Section 14.