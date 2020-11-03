/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "uart.h"

void UART_v_Init()
{
  U1MODEbits.UARTEN = 0u;      //Disable UART.
  U1MODEbits.STSEL = 0;        // 1-Stop bit
  U1MODEbits.PDSEL = 0;        // No Parity, 8-Data bits
  U1MODEbits.ABAUD = 0;        // Auto-Baud disabled

  //BRGVAL = ((FOSC*PLL1)/(4*16*BAUDRATE))-1
  //For PLL1 -> BRGVAL = 13
  //For PLL4 -> BRGVAL = 52
  U1BRG = 52;                  // Baud Rate set to 9600    
  U1STAbits.UTXISEL = 0u;      // Interrupt after one TX character is transmitted
  IEC0bits.U1TXIE = 0;         // Enable UART TX interrupt
  U1MODEbits.UARTEN = 1;       // Enable UART
  IPC2bits.U1TXIP = 1u;
  U1STAbits.UTXEN = 1;         // Enable UART TX
}

//Maximum U1TXREG size is 255 in decimal.