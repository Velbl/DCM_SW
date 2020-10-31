/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "uart.h"

#define BAUDRATE     9600
#define BRGVAL       ((FOSC*PLL1)/(4*16*BAUDRATE))-1

void UART_v_Init()
{
  //Disable UART.
  U1MODEbits.UARTEN = 0u; 
  //Set baud rate register value for wanted baud rate (9600).
  U1BRG = (uint16_t) BRGVAL;
  //8-bit data, no parity.
  U1MODEbits.PDSEL = 0x00u;
  //1 Stop bit.
  U1MODEbits.STSEL = 0u;
  //Baud rate measurement is disabled or complete.
  U1MODEbits.ABAUD = 0u;
  //Disable UART TX interrupt.
  IEC0bits.U1TXIE = 0u; 
  //Disable UART RX interrupt.
  IEC0bits.U1RXIE = 0u;
  //Enable UART.
  U1MODEbits.UARTEN = 1; 
  // Enable UART TX.
  U1STAbits.UTXEN = 1; 
}