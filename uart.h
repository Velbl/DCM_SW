/* 
 * File:   uart.h
 * Author: Stefan Velbl
 *
 * Created on 22 October 2020, 20:42
 */

#ifndef UART_H
#define	UART_H

#include "system.h"

 typedef struct
 {
   float u_One;
   float u_Tenth;
   float u_Hundredth;
   float u_Thousandth;
   float u_TenThousandth;
 }t_Number;
 
void UART_v_Write(int data);
 
uint8_t UART_v_IsBusy(void);

void CloseUART1(void);

void UART_v_Print(int Value);

void UART_v_NewLine();

//UART initialization interface.
void UART_v_Init(void);

#endif	/* UART_H */

