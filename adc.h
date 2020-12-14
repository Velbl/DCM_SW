/* 
 * File:   adc.h
 * Author: Stefan Velbl
 *
 * Created on 22 October 2020, 20:09
 */

#ifndef ADC_H
#define	ADC_H

/// A/D convertor interface for reading of the A/D buffer values.
/// This function reads from one of the 16 ADC Buffers (ADCBUF0 - ADCBUFF).
uint16_t ADC_v_Read(uint8_t u_BufferIndex);

/// A/D convertor interface which returns ADC status register value.
/// ADC status register value tells us if ADC convertor is busy or not.
uint8_t BusyADC1(void);

/// Interface for A/D convertor configuraton.
void ADC_v_Config(void);


#endif	/* ADC_H */

