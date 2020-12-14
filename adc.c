/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "adc.h"

static void v_Config()
{
  ADCON1bits.ADON   = 0u;    // Turn of A/D convertor.
  
/*******************************ADCON1 register********************************/
  ADCON1bits.ADSIDL = 1u;    // Discontinue module operation when device enters Idle mode.
  ADCON1bits.FORM   = 0x00u; // Integer (DOUT = 0000 00dd dddd dddd)
  ADCON1bits.SSRC   = 0x03u; // Motor Control PWM interval ends sampling and starts conversion.
  ADCON1bits.SIMSAM = 1u;    // Samples CH0, CH1, CH2, CH3 simultaneously (when CHPS = 1x).
  ADCON1bits.ASAM   = 1u;    // Sampling begins immediately after last conversion completes. SAMP bit is auto set.  
/******************************************************************************/    
  
/*******************************ADCON2 register********************************/
  ADCON2bits.VCFG  = 0x00u;  // Set voltage references to AVdd and AVss.
  ADCON2bits.CHPS  = 0x03u;  // Converts CH0, CH1, CH2 and CH3.
  ADCON2bits.BUFM  = 1u;     // Buffer configured as two 8-word buffers ADCBUF(15...8), ADCBUF(7...0).
/******************************************************************************/    

/*******************************ADCON3 register********************************/
  ADCON3bits.SAMC  = 0x01u;  // Auto-Sample time = 1Tad.
  ADCON3bits.ADRC  = 0u;     // Clock derived from system clock (8MHz).
  //ADCS = (2*TADmin/Tcy) - 1    = (2*(154ns/125ns)) - 1  = 2
  //TAD = ( Tcy*(ADCS + 1) / 2 ) = ( 125ns*(2+1) / 2 )    = 187ns -> (TAD > TADmin) 
  ADCON3bits.ADCS  = 0x2u;   
/*******************************ADCON3 register********************************/
  
/*******************************ADCHS register*********************************/
  ADCHSbits.CH123NA = 0x00u; // CH1, CH2, CH3 negative input is VREF.
  ADCHSbits.CH123SA = 0u;    // CH1 positive input is AN0, CH2 positive input is AN1, CH3 positive input is AN2.
  ADCHSbits.CH0NA   = 0u;    // CH0 negative input is VREF.
  ADCHSbits.CH0SA   = 0x0Du; // CH0 positive input is AN3.
/******************************************************************************/  
  
/*******************************ADPCFD register********************************/
  ADPCFGbits.PCFG0 = 0u;     // Set AN0 (RB0) as analog input pin.
  ADPCFGbits.PCFG1 = 0u;     // Set AN1 (RB1) as analog input pin.
  ADPCFGbits.PCFG2 = 0u;     // Set AN2 (RB2) as analog input pin.
  ADPCFGbits.PCFG3 = 0u;     // Set AN3 (RB3) as analog input pin.
/******************************************************************************/ 
  
  IEC0bits.ADIE    = 0u;     // A/D interrupt disabled.
  
  ADCON1bits.ADON  = 1u;     // Turn on A/D converter.
}

uint16_t ADC_v_Read(uint8_t u_BufferIndex)
{
  return(*(&ADCBUF0 + u_BufferIndex));
}

uint8_t BusyADC1(void)
{
    return !(ADCON1bits.DONE);	// Returns the DONE bit status.
}

void ADC_v_Config()
{
  v_Config();
}
