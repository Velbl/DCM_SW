/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__dsPIC30F__)
        #include <p30Fxxxx.h>
    #endif
#endif

#include "uart.h"
#include <stdint.h> 
#include <stdbool.h>       /* Includes true/false definition                  */

void UART_v_Init()
{
  //U1BRG = ( ((FCY/Desired baud rate)/16) - 1 )= (((8000000/9600)/16) - 1)           
  U1BRG = 52;                  // Baud Rate set to 9600bps.   
  
  U1MODEbits.UARTEN = 0u;      //Disable UART.
  
/*******************************U1MODE register********************************/
  U1MODEbits.USIDL  = 1u;      //Discontinue operation when device enters Idle mode.
  U1MODEbits.ALTIO  = 0u;      //UART communicates using UxTX and UxRX I/O pins.
  U1MODEbits.ABAUD  = 0u;      //Auto-Baud disabled
  U1MODEbits.PDSEL  = 0x00u;   //No Parity, 8-Data bits.
  U1MODEbits.STSEL  = 0u;      //1-Stop bit.
/******************************************************************************/

/******************************UART INTERRUPT CONFIG***************************/  
  U1STAbits.UTXISEL = 0u;      //Interrupt after one TX character is transmitted from U1TXB to U1TSR.Transmit buffer has at least one empty word.
  IFS0bits.U1TXIF   = 0u;      //Clear interrupt flag.
  IEC0bits.U1TXIE   = 0u;      //Disable UART TX interrupt
  IPC2bits.U1TXIP   = 1u;      //UART interrupt priority.    
/******************************************************************************/
  
  U1MODEbits.UARTEN = 1;       //Enable UART
  
  U1STAbits.UTXEN   = 1u;      //Enable UART TX.
}

void UART_v_Write(int data)
{
    //9 bit data is configured.
    if( U1MODEbits.PDSEL == 0x03u )
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

uint8_t UART_v_IsBusy(void)
{  
    return( !U1STAbits.TRMT );
}
void CloseUART1(void)
{  
    U1MODEbits.UARTEN = 0;
	
    IEC0bits.U1RXIE = 0;
    IEC0bits.U1TXIE = 0;
	
    IFS0bits.U1RXIF = 0;
    IFS0bits.U1TXIF = 0;
}


void UART_v_Print(int Value)
{
  t_Number Number = {0u};
  
	while( UART_v_IsBusy() );
	if (Value < 0)						
	{
    //Write minus prefix.
		UART_v_Write('-');
    
    //Wait until writing is finished.
		while(UART_v_IsBusy());
    
    //Change input value sign.
		Value = -Value;
	}
  
  //Chop input value and print all parts separately.
 	Number.u_One=Value % 10;	
	Value/=10;
	Number.u_Tenth=Value % 10;
	Value/=10;
	Number.u_Hundredth=Value % 10;
	Value/=10;
	Number.u_Thousandth=Value % 10;	
	Value/=10;	
	Number.u_TenThousandth=Value % 10;	
  
	while(UART_v_IsBusy());
	UART_v_Write(Number.u_TenThousandth +'0');
	while(UART_v_IsBusy());
	UART_v_Write(Number.u_Thousandth +'0');
	while(UART_v_IsBusy());
	UART_v_Write(Number.u_Hundredth +'0');
	while(UART_v_IsBusy());
	UART_v_Write(Number.u_Tenth +'0');
	while(UART_v_IsBusy());
	UART_v_Write(Number.u_One +'0'); 
  while(UART_v_IsBusy());
}

void UART_v_NewLine()
{
	while(UART_v_IsBusy());					// CR
	UART_v_Write(13);
	while(UART_v_IsBusy());					// LF
	UART_v_Write(10);
	while(UART_v_IsBusy());
}