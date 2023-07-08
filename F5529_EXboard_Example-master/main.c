/*
 *      本工程基于MSP430F5529 LaunchPad Pad扩展底板
 *      基本包含了绝大多数板载资源的程序和可能用到的程序
 *      其余未包含的请自行探索开发
 *      作者：某个不知名的科艺人
 *      时间：2022.5.15
 *      默认全局文本编码：GBK/GB2312
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

    init_hx711port();  //初始化HX711


    //SSD1351_init();     //SSD1351和SSD1306只可使 用其一，当你的显示有问题时请检查是不是SSD1351用了SSD1306的代码了等之类的问题
    //Lcd12864_Init();

//    INA_init();
//  ADS1256_init();     //<---请务必连接上ADS1256后再进行初始化，否则会直接卡死在其中的死循环等待中
//  DAC8571_Init();     //<---请务必连接上DAC8571后再进行初始化，否则会直接卡死在其中的死循环等待中
//ADC12_init();       //F5529片内ADC12初始化，注意其引脚与矩阵键盘复用，注意拔掉对应矩阵键盘跳线帽在进行测试以免影响键盘使用
//  TA0_PWM_init();     //TA0.1即P1.2的SPWM(你可以理解为占空比呈正弦变化的PWM)输出测试初始化，初始化为即开始输出，注意拔掉对应矩阵键盘跳线帽在进行测试以免影响键盘使用

    _EINT();    //打开总中断


    //GPIO初始化，对应LaunchPad板载的两颗LED灯
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    P4DIR |= BIT7;
    P4OUT &= ~BIT7;

    //SSD1351刷屏演示，测试SSD1306屏幕时请务必注释掉这段代码
//  SSD1351_Clear(RED);
//  SSD1351_Clear(GREEN);
//  SSD1351_Clear(BLUE);
//  SSD1351_Clear(BLACK);

    //显示输出演示，包括串口输出、SSD1351显示屏输出、LCD12864输出
  printf("Hello MSP430F5529!\r\n");
    //SSD1306_ShowString(0,0,"weight", 16);

    //SSD1351_Show_String(0, 0, "Hello MSP430F5529!", WHITE, BLUE, 12, 0);
    //lcd12864_disp_str(1, 1, "你好");
    //lcd12864_disp_str(1, 2, "MSP430F5529");

    //KeyBoard矩阵键盘演示
    //这一部分基于看门狗定时器，当按下一个按键时，从串口会输出相应按键的编号
    //具体哪个按键的作用是什么请进入按键源码文件内设置

//  //DAC8571输出演示<---请务必连接上DAC8571后再进行初始化，否则会直接卡死在其中的死循环等待中
//  DAC8571_OUT(2.5);

    while(1)
    {

        //INA260采集数据演示
//        Vlotage = INA_read_vlot();
//        Current = INA_read_current();
//        Power = INA_read_power();


        weight =  get_weight();
        sprintf(display_str, "%.2f g",weight );
        SSD1306_ShowString(0, 0, display_str, 16);

        instr_num();

        //SSD1306_clear();
        //ADS1256采集电压演示<---请务必连接上ADS1256后再进行初始化，否则会直接卡死在其中的死循环等待中
  //      Vlotage = ADS1256_Single_Read(AIN0, AINCOM);
//        sprintf(display_str, "ADS1256: %.2f V  ", Vlotage);
//        SSD1351_Show_String(0, 48, display_str, WHITE, GREEN, 12, 0);

        //ADC12采集电压演示，数据将在串口输出，注意拔掉跳线帽以免与键盘之间相互影响
        //printf("A0: %d\r\nA1: %d\r\nA2: %d\r\nA3: %d\r\n", ADC12MEM0, ADC12MEM1, ADC12MEM2, ADC12MEM3);

        //GPIO翻转，LED灯发生变化
        P1OUT ^= BIT0;
        P4OUT ^= BIT7;
    }
}
