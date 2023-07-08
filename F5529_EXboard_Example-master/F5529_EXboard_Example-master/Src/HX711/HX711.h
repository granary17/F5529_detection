/*
 * HX711.h
 *
 *  Created on: 2023��7��2��
 *      Author: 37756
 */

#ifndef SRC_HX711_H_
#define SRC_HX711_H_

extern void init_hx711port(void);                                        /*��ʼ��HX711�˿�  3.3V���� 4.0��SCK 4.1��DT */
extern unsigned long read_hx711_24bit(void);                 /* ��ȡHX711�ڲ������� */
extern void  get_maopi(void);                                               /* ��ȡëƤ����,�ϵ��ʼ��ʱ��ִ��*/
extern float get_weight(void);                            /* ��ȡ���� ����g��λ������ */
extern void  instr_num(void);   /*�ж���������*/

#endif /* SRC_HX711_H_ */

