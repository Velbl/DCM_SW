/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "user.h"

static void ADC_v_InterruptConfig(void);
static void ADC_v_Config(void);


static void ADC_v_InterruptConfig()
{
    IFS0bits.ADIF = 0u;
    IPC2bits.ADIP = 3u;
    IEC0bits.ADIE = 1u;
}
static void ADC_v_Config()
{
    //Turn off ADC module
    ADCON1bits.ADON   = 0u;
    //Voltage Reference: VREF+ & VREF-
    ADCON2bits.VCFG   = 3u;
    //Clock derived from system clock
    ADCON3bits.ADRC   = 0u;
    //A/D Conversion Clock = TCY/2 (250ns)
    ADCON3bits.ADCS   = 0u;                                  //TODO is this okay?
    /*ADPCFG register is configured in "IO_ports.c" file, under the v_SensorPins(), static function*/
    //Sampling begins when SAMP bit set.
    ADCON1bits.ASAM   = 1u;
    //Channel 0 positive input is AN0.
    ADCHSbits.CH0SA0 = 0u;
    //Channel 0 negative input is VREF-.
    ADCHSbits.CH0NA  = 0u;
    //Clearing SAMP bit ends sampling and starts conversion.
    ADCON1bits.SSRC   = 0u;  
    //Samples multiple channels individually in sequence.
    ADCON1bits.SIMSAM = 0u;
    //Integer data output format bits.
    ADCON1bits.FORM   = 0u;
    //Single 16-word result buffer.
    ADCON2bits.BUFM   = 0u;
    //Disable ADC interrupt.
    IEC0bits.ADIE     = 0u;
    //Turn on ADC module.
    ADCON1bits.ADON   = 1u;
}

void ADC_v_Convert()
{
  /*Set SAMP to start conversion*/
  ADCON1bits.SAMP = 1u; 
}

uint16_t ADC_v_Read(uint8_t u_BufferIndex)
{
  return(*(&ADCBUF0 + u_BufferIndex));
}

void ADC_v_Init()
{
  ADC_v_InterruptConfig();
  ADC_v_Config();
}
//Info found in "Section 17. 10-Bit A/D Converter - dsPIC30F FRM", page 35 
//Followed 1 Msps Configuration Procedure, page 51