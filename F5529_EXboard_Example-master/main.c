/*
 *      �����̻���MSP430F5529 LaunchPad Pad��չ�װ�
 *      ���������˾������������Դ�ĳ���Ϳ����õ��ĳ���
 *      ����δ������������̽������
 *      ���ߣ�ĳ����֪���Ŀ�����
 *      ʱ�䣺2022.5.15
 *      Ĭ��ȫ���ı����룺GBK/GB2312
 */

#include <msp430.h> 
#include <stdio.h>
//#include "SYS/UART.h"
//#include "SYS/UCS_INIT.h"
//#include "SYS/ADC12_INIT.h"
#include "SSD1306/SSD1306_api.h"
//#include "SSD1351/SSD1351_api.h"
//#include "LCD12864/LCD12864.h"
//#include "INA/INA.h"
//#include "ADS1256/ADS1256.h"
//#include "DAC8571/DAC8571.h"
#include "KEYBOARD/KEY_BOARD.h"
#include"HX711/HX711.h"
//#include"string.h"
/**
 * main.c
 */
int main(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
//  float Vlotage;
//  float Current;
//  float Power;
    char display_str[32];
    float weight;
//    unsigned int i =0;

//    UCS_init();
//    Uart1_Init();
    KEY_BOARD_init();
    SSD1306_init();

    init_hx711port();  //��ʼ��HX711


    //SSD1351_init();     //SSD1351��SSD1306ֻ��ʹ ����һ���������ʾ������ʱ�����ǲ���SSD1351����SSD1306�Ĵ����˵�֮�������
    //Lcd12864_Init();

//    INA_init();
//  ADS1256_init();     //<---�����������ADS1256���ٽ��г�ʼ���������ֱ�ӿ��������е���ѭ���ȴ���
//  DAC8571_Init();     //<---�����������DAC8571���ٽ��г�ʼ���������ֱ�ӿ��������е���ѭ���ȴ���
//ADC12_init();       //F5529Ƭ��ADC12��ʼ����ע���������������̸��ã�ע��ε���Ӧ�����������ñ�ڽ��в�������Ӱ�����ʹ��
//  TA0_PWM_init();     //TA0.1��P1.2��SPWM(��������Ϊռ�ձȳ����ұ仯��PWM)������Գ�ʼ������ʼ��Ϊ����ʼ�����ע��ε���Ӧ�����������ñ�ڽ��в�������Ӱ�����ʹ��

    _EINT();    //�����ж�


    //GPIO��ʼ������ӦLaunchPad���ص�����LED��
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    //SSD1351ˢ����ʾ������SSD1306��Ļʱ�����ע�͵���δ���
//  SSD1351_Clear(RED);
//  SSD1351_Clear(GREEN);
//  SSD1351_Clear(BLUE);
//  SSD1351_Clear(BLACK);

    //��ʾ�����ʾ���������������SSD1351��ʾ�������LCD12864���
  printf("Hello MSP430F5529!\r\n");
    //SSD1306_ShowString(0,0,"weight", 16);

    //SSD1351_Show_String(0, 0, "Hello MSP430F5529!", WHITE, BLUE, 12, 0);
    //lcd12864_disp_str(1, 1, "���");
    //lcd12864_disp_str(1, 2, "MSP430F5529");

    //KeyBoard���������ʾ
    //��һ���ֻ��ڿ��Ź���ʱ����������һ������ʱ���Ӵ��ڻ������Ӧ�����ı��
    //�����ĸ�������������ʲô����밴��Դ���ļ�������

//  //DAC8571�����ʾ<---�����������DAC8571���ٽ��г�ʼ���������ֱ�ӿ��������е���ѭ���ȴ���
//  DAC8571_OUT(2.5);

    while(1)
    {

        //INA260�ɼ�������ʾ
//        Vlotage = INA_read_vlot();
//        Current = INA_read_current();
//        Power = INA_read_power();


        weight =  get_weight();
        sprintf(display_str, "%.2f g",weight );
        SSD1306_ShowString(0, 0, display_str, 16);

        instr_num();

        //SSD1306_clear();
        //ADS1256�ɼ���ѹ��ʾ<---�����������ADS1256���ٽ��г�ʼ���������ֱ�ӿ��������е���ѭ���ȴ���
  //      Vlotage = ADS1256_Single_Read(AIN0, AINCOM);
//        sprintf(display_str, "ADS1256: %.2f V  ", Vlotage);
//        SSD1351_Show_String(0, 48, display_str, WHITE, GREEN, 12, 0);

        //ADC12�ɼ���ѹ��ʾ�����ݽ��ڴ��������ע��ε�����ñ���������֮���໥Ӱ��
        //printf("A0: %d\r\nA1: %d\r\nA2: %d\r\nA3: %d\r\n", ADC12MEM0, ADC12MEM1, ADC12MEM2, ADC12MEM3);

        //GPIO��ת��LED�Ʒ����仯
        P1OUT ^= BIT0;
        P4OUT ^= BIT7;
    }
}
