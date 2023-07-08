/*
 * PWM.c
 *
 *  Created on: 2021.8.8
 *      Author: Zuoge
 */

/*
 * 2022.5.15:
 * 此PWM主要实现类似SPWM的效果，在TA0的通道1即P1.2输出PWM
 * V2.0/V2.1/V2.2扩展底板注意拔掉矩阵键盘的跳线帽以免影响矩阵键盘
 */
#include <msp430.h>
#include "PWM.h"

unsigned int spwmbit[100] = {200,211,222,233,244,254,264,275,284,294,303,312,320,328,335,342,
                             348,353,358,363,366,370,372,374,375,375,375,374,372,370,366,363,
                             358,353,348,342,335,328,320,312,303,294,284,275,264,254,244,233,
                             222,211,200,189,178,167,156,146,136,125,116,106,97,88,80,72,
                             65,58,52,47,42,37,34,30,28,26,25,25,25,26,28,30,
                             34,37,42,47,52,58,65,72,80,88,97,106,116,125,136,146,
                             156,167,178,189};
unsigned int spwm_i = 0;

void TA0_PWM_init()
{
    P1SEL |= BIT2;
    P1DIR |= BIT2;

    //设置TA0通道1即P1.2的PWM，参考时钟为SMCLK
    TA0CTL |= TASSEL_2 + ID_0 + MC_1 + TACLR;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = 400 - 1;      //PWM周期
    TA0CCR1 = 0;            //PWM在周期内高电平时间

    //设置TA1为普通定时器功能，参考时钟为SMCLK，每40000个tick进入一次中断
    TA1CCR0 = 40000 - 1;
    TA1CTL |= TASSEL_2 + ID_0 + MC_1 + TACLR;
    TA1CTL |= TAIE;
}

//TA1定时器中断，在中断中改变TA0.1的高电平时间以实现SPWM的效果
#pragma vector=TIMER1_A1_VECTOR
__interrupt void TimerA1(void)
{
    TA0CCR1 = spwmbit[spwm_i++];
    if(spwm_i==100)spwm_i=0;
    TA1IV=0;
}
