/* 
 * File:   uart.h
 * Author: Stefan Velbl
 *
 * Created on 22 October 2020, 20:42
 */

#ifndef UART_H
#define	UART_H
 
/// Interface for starting of the writing process.
/// Writing characters on terminal.
void UART_v_Write(int data);
 
/// Interface which gives us information about UART writing process, is it busy or not.
uint8_t UART_v_IsBusy(void);

/// Interface for ending of the print process.
void CloseUART1(void);

/// Interface for editing printing on terminal.
/// This function manage with UART_v_Write() function in order to enable nicer writing on terminal.
void UART_v_Print(int Value);

/// Interface for inserting the new line in terminal.
void UART_v_NewLine();

/// Interface for UART configuration.
void UART_v_Config(void);

#endif	/* UART_H */

