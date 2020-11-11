/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "uart.h"

void UART_v_Busy(void)
{
  //Transmission is in progress.
  while ( U1STAbits.TRMT != 1u )
  {
    //Wait until transmission is done.
  }
  //Transmission is completed.
}

void UART_v_Init()
{
  //U1BRG = ( ((FCY/Desired baud rate)/16) - 1 )= (((8000000/9600)/16) - 1)           
  U1BRG = 52;                  // Baud Rate set to 9600bps.   
  
  U1MODEbits.UARTEN = 0u;      //Disable UART.
  
/*******************************U1MODE register********************************/
  U1MODEbits.USIDL  = 1u;      //Discontinue operation when device enters Idle mode.
  U1MODEbits.ALTIO  = 0u;      //UART communicates using UxTX and UxRX I/O pins.
  U1MODEbits.ABAUD  = 0u;      //Auto-Baud disabled
  U1MODEbits.PDSEL  = 0u;      //No Parity, 8-Data bits.
  U1MODEbits.STSEL  = 0u;      //1-Stop bit.
/******************************************************************************/

/******************************UART INTERRUPT CONFIG***************************/  
  U1STAbits.UTXISEL = 0u;      //Interrupt after one TX character is transmitted from U1TXB to U1TSR.Transmit buffer has at least one empty word.
  IFS0bits.U1TXIF   = 0u;      //Clear interrupt flag.
  IEC0bits.U1TXIE   = 1u;      //Enable UART TX interrupt
  IPC2bits.U1TXIP   = 1u;      //UART interrupt priority.    
/******************************************************************************/
  
  U1MODEbits.UARTEN = 1;       //Enable UART
  
  U1STAbits.UTXEN   = 1u;      //Enable UART TX.
}

//Maximum U1TXREG size is 255 in decimal.