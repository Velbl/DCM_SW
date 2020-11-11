/* 
 * File:   uart.h
 * Author: Stefan Velbl
 *
 * Created on 22 October 2020, 20:42
 */

#ifndef UART_H
#define	UART_H

#include "system.h"

//UART TX line is occupied, transmission is in progress.
void UART_v_Busy(void);

//UART initialization interface.
void UART_v_Init();

#endif	/* UART_H */

