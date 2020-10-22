/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif


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
    
}