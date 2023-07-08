/*
 * UART.h
 *
 *  Created on: 2021.6.23
 *      Author: HASEE
 */

#ifndef SRC_UART_H_
#define SRC_UART_H_

void Uart1_Init();
void Uart0_Init_38400();
void Uart0_Init_9600();
void Uart0_TX(char *_ptr);
#endif /* SRC_UART_H_ */
