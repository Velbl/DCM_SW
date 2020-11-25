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
   uint16_t u_One;
   uint16_t u_Tenth;
   uint16_t u_Hundredth;
   uint16_t u_Thousandth;
   uint16_t u_TenThousandth;
 }t_Number;
 
void UART_v_Write(uint16_t data);
 
uint8_t UART_v_IsBusy(void);

void UART_v_Print(int Value);

void UART_v_NewLine();

//UART initialization interface.
void UART_v_Init(void);

#endif	/* UART_H */

