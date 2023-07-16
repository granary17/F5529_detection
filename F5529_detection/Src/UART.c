/*
 * UART.c
 *
 *  Created on: 2021.6.23
 *      Author: Zuoge
 */


#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "UART.h"
/*
 *  ��������Uart1_Init
 *  ��  ������ʼ��Uart1��������Ϊ9600������żУ�飬ֹͣλ1��8λ����
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
/***�ڳ�ʼ����Uart1��ͨ����fputc��fputs�ĸ�д��
 *  �ȿ���ʹ��printf���д������               ***/
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
 *  ��������Uart0_Init_9600
 *  ��  ������ʼ��Uart0��������Ϊ9600������żУ�飬ֹͣλ1��8λ����
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
 *  ��������Uart0_Init_38400
 *  ��  ������ʼ��Uart0��������Ϊ38400������żУ�飬ֹͣλ1��8λ����
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
 *  ��������Uart0_TX
 *  ��  ����ͨ��Uart0���з����ַ���
 *  ��  �룺 *_ptr:�ַ���ָ�룬��Ҫ���͵��ַ���
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




