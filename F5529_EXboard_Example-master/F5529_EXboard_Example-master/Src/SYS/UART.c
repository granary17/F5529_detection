/*
 * UART.c
 *
 *  Created on: 2021.6.23
 *      Author: Zuoge
 */


#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "SYS/UART.h"
/*
 *  函数名：Uart1_Init
 *  描  述：初始化Uart1，波特率为9600，无奇偶校验，停止位1，8位数据
 */
void Uart1_Init()
{
    P4SEL |= BIT4 + BIT5;
    UCA1CTL1 |= UCSWRST;
    UCA1CTL1 |= UCSSEL__ACLK;
    UCA1BR0 = 3;
    UCA1BR1 = 0;
    UCA1MCTL = UCBRS_3;
    UCA1CTL1 &= ~UCSWRST;
}
/***在初始化完Uart1后通过对fputc和fputs的复写后
 *  既可以使用printf进行串口输出               ***/
int fputc(int _c, register FILE *_fp)
{
    while(!(UCA1IFG&UCTXIFG));
    UCA1TXBUF = (unsigned char) _c;
    return((unsigned char)_c);
}

int fputs(const char *_ptr, register FILE *_fp)
{
    unsigned int i, len;
    len = strlen(_ptr);
    for(i=0 ; i<len ; i++)
    {
        while(!(UCA1IFG&UCTXIFG));
        UCA1TXBUF = (unsigned char) _ptr[i];
    }
    return len;
}

/*
 *  函数名：Uart0_Init_9600
 *  描  述：初始化Uart0，波特率为9600，无奇偶校验，停止位1，8位数据
 */
void Uart0_Init_9600()
{
    P3SEL |= BIT3 + BIT4;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL__ACLK;
    UCA0BR0 = 3;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_3;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
}

/*
 *  函数名：Uart0_Init_38400
 *  描  述：初始化Uart0，波特率为38400，无奇偶校验，停止位1，8位数据
 */
void Uart0_Init_38400()
{
    P3SEL |= BIT3 + BIT4;
    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL__SMCLK;
    UCA0BR0 = 6;
    UCA0BR1 = 0;
    UCA0MCTL = UCBRS_0 | UCBRF_13 | UCOS16;
    UCA0CTL1 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
}

/*
 *  函数名：Uart0_TX
 *  描  述：通过Uart0进行发送字符串
 *  输  入： *_ptr:字符串指针，需要发送的字符串
 */
void Uart0_TX(char *_ptr)
{
    unsigned int i, len;
    len = strlen(_ptr);
    for(i = 0 ; i < len ; i++)
    {
        while(!(UCA0IFG&UCTXIFG));
        UCA0TXBUF = (unsigned char) _ptr[i];
    }
    //return len;
}




