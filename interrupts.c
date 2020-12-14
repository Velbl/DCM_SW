/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include <stdint.h> 
#include <stdbool.h>      
#include "user.h"
#include "adc.h"
#include "pwm.h"
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
  int ref_speed   = 0;
  int ref_current = 0;
extern unsigned long SystemTime;

extern unsigned long SystemTimeMs;

extern t_PIRegulatorData  PIReg;

tStates              StateFlag;

// Initialization of all measured values.
t_MeasuredValues     Measured = 
{
  0u,    // Initial measured current is zero.
  0u,    // Initial measured DC voltage is zero.
  0u,    // Initial measured speed.
};

void __attribute__((interrupt,no_auto_psv)) _T1Interrupt(void)
{
  // Catch current system status.
  StateFlag = GetState();
  
  // System is in initialization state, set proper status flag.
  if ( StateFlag == INIT_STATE )
  {
    // Signal that system is initialized.
    LATFbits.LATF0  = 1;               // Set RF0 pin to 1. 
  }
  
   IFS0bits.T1IF    = 0;               // Reset the timer 1 interrupt flag.
}


void __attribute__((interrupt,no_auto_psv)) _T2Interrupt(void)
{
  // Catch current system status.
  StateFlag = GetState();
  
  // System is in idle state, set proper status flag.
  if (   StateFlag == IDLE_STATE )
  {
    // Signal that system is in idle state.
    LATFbits.LATF1 ^= 1;               // Toggle RF1 pin. 
  }
    IFS0bits.T2IF   = 0;               // Reset the timer 2 interrupt flag.
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
#ifdef SPEED_REG
  
  // Read current measurement
  while (BusyADC1());
  
  // Save measured value in structure for storing of all measured values.
  Measured.Speed = ADC_v_Read(1u);  // Read ADC value from ADCBUF1.
  
  // Convert saved measured speed to fixed point, 3.13 format.
  // Update measured speed value in speed PI regulator structure.
  PIReg.s_SpeedReg.MeasuredSpeed = i_ConvertToFixedPoint(Measured.Speed, FORMAT_3_13);
  
  f_SetReferentSpeed(ref_speed);                       //Enter value from -3370[rpm] to 3370[rpm] .
      
  v_CalculatePIRegOutput(SPEED_REGULATOR);
  
#endif // #ifdef SPEED_REG
/**********************************************************************************************/
  
/************************************CURRENT LOOP**********************************************/
#ifdef CURRENT_REG
  
#ifdef REAL_SCENARIO
  
  // Output of the speed regulator is reference for the current regulator.
  // 0 - 8192    -> 0 - 32768
  // 0 - (-8192) -> 0 - (-32768)
  int SpeedOutput = (int)(((float)MAXIMAL_CURRENT / (float)PIW_REG_MAX_OUTPUT) * PIReg.s_SpeedReg.Output); 
  
  // Set reference current based on speed regulator output.
  f_SetReferentCurrent(SpeedOutput);
  
  // Read current measurement and save it in global structure.
  //Measured.Current = ADC_v_Read(2u);  // Read ADC value from ADCBUF2.
  
#endif // #ifdef REAL_SCENARIO

#ifndef REAL_SCENARIO
  
  // Set reference current manually.
  f_SetReferentCurrent(5);
  
#endif // #ifndef REAL_SCENARIO
  
  // Wait ADC conversion to be finished.
  while (BusyADC1());
  
  // Read current measurement and save it in global structure.
  Measured.Current = ADC_v_Read(1u); // Read ADC value from ADCBUF2. 

  
  // Convert saved measured current to fixed point, 1.15 format.
  // Update measured current value in current PI regulator structure.
  PIReg.s_CurrentReg.MeasuredCurrent = i_ConvertToFixedPoint(Measured.Current, FORMAT_1_15);
  

  
  v_CalculatePIRegOutput(CURRENT_REGULATOR);
      
  // Scale output value to PERIOD register value.
  Temp = (long)PWM_PERIOD*(long)PIReg.s_CurrentReg.Output;
 
  // Update duty cycle value.
  // PDC1 = ((PWM/2) * Temp)*2
  PDC1 = ( (int)(PWM_PERIOD >> 1) + (int)(Temp >> 16) ) << 1;
  
  // Set PDC2 register value to be equal as PDC1 register value.
  PDC2 = PDC1;
  
#endif // CURRENT_REG
/**********************************************************************************************/
  
  IFS2bits.PWMIF = 0;
}

//INFO: XC16 Compiler User's Guide - Interrupts Section 14.