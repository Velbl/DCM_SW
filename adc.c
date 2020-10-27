/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "user.h"

static void ADC_v_Config(void);



static void ADC_v_Config()
{
    //ADCON1 |= ( MASK ());
}
void ADC_v_Convert()
{
    /*Clear SAMP to start conversion*/
    ADCON1bits.SAMP = 0u; 
}

uint16_t ADC_v_Read(uint8_t u_BufferIndex)
{
    return(*(&ADCBUF0 + u_BufferIndex));
}

void ADC_v_Init()
{
    /*Turn off ADC module*/
    ADCON1bits.ADON   = 0u;
    
    /*Voltage Reference: AVDD & AVSS*/
    ADCON2bits.VCFG   = 0u;
    
    /*Clock derived from system clock*/
    ADCON3bits.ADRC   = 0u;
    
    /*A/D Conversion Clock = TCY/2 (0.25us)*/
    ADCON3bits.ADCS   = 0u;                                  //TODO is this okay?
   
    /*Converts CH0, CH1, CH2 and CH3*/
    ADCON2bits.CHPS   = 2u;                       //TODO Can I represent 1x as 2?
    
    /* Samples CH0, CH1, CH2, CH3 simultaneously (when CHPS = 1x)*/
    ADCON1bits.SIMSAM = 1u;
    
    /*ADPCFG register is configured in "IO_ports.c" file, under the v_SensorPins(), static function*/
    
    /*Always use MUX A input multiplexer settings*/
    ADCON2bits.ALTS   = 0u;
    
    /*SETTINGS FOR MUX A*/
    /*Channel 0 negative input is VREF*/
    ADCHSbits.CH0NA   = 0u;
    /*Scan inputs*/
    ADCON2bits.CSCNA  = 1u;
    /*A/D Input Pin Scan Selection*/
    ADCSSLbits.CSSL5  = 1u;
    ADCSSLbits.CSSL6  = 1u;
    /*CH1 positive input is AN0, CH2 positive input is AN1, CH3 positive input is AN2*/
    ADCHSbits.CH123SA = 0u;
    /*CH1, CH2, CH3 negative input is VREF*/
    ADCHSbits.CH123NA = 0u;
    
    /*Internal counter ends sampling and starts conversion (auto convert)*/
    ADCON1bits.SSRC   = 6u;  
    /*Auto start sampling*/
    ADCON1bits.ASAM   = 1u;
    
    /*Auto sample time = 12Tad*/
    ADCON3bits.SAMC   = 12u;                                 //TODO is this okay?
    
    /*Integer data output format bits*/
    ADCON1bits.FORM   = 0u;
    
    /*Interrupts at the completion of conversion for each eight sample/convert sequence*/
    ADCON2bits.SMPI   = 7u;
    
    /*Single 16-word result buffer*/
    ADCON2bits.BUFM   = 0u;
    
    /*Turn on ADC module*/
    ADCON1bits.ADON   = 1u;
    
    /*Do not allow ADC interrupt*/
    IEC0bits.ADIE     = 0u;

}
//Info found in "Section 17. 10-Bit A/D Converter - dsPIC30F FRM", page 35 
//Followed 1 Msps Configuration Procedure, page 51