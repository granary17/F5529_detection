/*
 * ADC12_INIT.h
 *
 *  Created on: 2015Äê7ÔÂ8ÈÕ
 *      Author: Administrator
 */

#ifndef ADC12_INIT_H_
#define ADC12_INIT_H_

#define   Num_of_Results   8

volatile unsigned int A0results[Num_of_Results];
volatile unsigned int A1results[Num_of_Results];
volatile unsigned int A2results[Num_of_Results];
volatile unsigned int A3results[Num_of_Results];

void ADC12_init(void);

#endif /* ADC12_INIT_H_ */
