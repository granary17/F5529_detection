/*
 * HX711.c
 *
 *  Created on: 2023年7月2日
 *      Author: 37756
 */
#include <msp430F5529.h>
#include <stdio.h>
#include "HX711/HX711.h"
#include "SSD1306/SSD1306_api.h"
#define SCK_L       P4OUT   &= ~BIT2
#define SCK_H       P4OUT   |= BIT2
#define SDA_IN      (P4IN & BIT1)
#define GapValue    116.18    /* 读取出的HX711为一个寄存器数值单位并不是g，除以这个GapValue值就是换算成g单位 */
#define Volt_to_weight   82.16   /*电压转重量*/
unsigned long Weight_Maopi; /* 毛皮重，上电后就读取出这个变量 */

#define CPU_F ((double)16000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
/*
 * 初始化HX711端口
 *  3.3V供电
 *  4.0接SCK
 *  4.1接DT
 * */
void init_hx711port(void)
{
    P4DIR |= BIT2; /* 单片机P4.0引脚输出 */
    P4OUT &= ~BIT2; /* 单片机P4.0引脚输出低电平 */

    P4DIR &= ~BIT1; /* 单片机P4.1引脚输入 */
    /* P4REN |= BIT1;   //单片机P4.1引脚输入上拉使能 F149单片机这里没内部上拉 */
    //P4OUT |= BIT1; /* 单片机P4.1引脚输入上拉电阻 */
    get_maopi();
}

/* 读取HX711内部的数据 */
unsigned long read_hx711_24bit(void)
{
    unsigned long Count = 0;
    unsigned char i;
    SCK_L; /* SCL=0;使能AD（PD_SCL 置低） */
    while ( SDA_IN)
        ;
    /* AD转换未结束则等待，否则开始读取 循环读取24次 */
    for (i = 0; i < 24; i++)
    {
        SCK_H; /* SCL = 1; PD_SCL 置高（发送脉冲） */
        Count = Count << 1; /*下降沿来时变量Count左移一位，右侧补零 */
        delay_us(2);
        SCK_L; /* SCL=0; PD_SCL 置低 */
        if ( SDA_IN)
            Count++; /* if(SDA) Count++; */
        delay_us(2);
    }
    SCK_H; /* SCL=1; */
    Count = Count ^ 0x800000; /* 第25个脉冲下降沿来时，转换数据 */
    delay_us(1);
    SCK_L; /* SCL=0; */
    return (Count);
}

/*
 *
 * 获取毛皮重量
 * 上电初始化时候执行
 *
 */
void get_maopi(void)
{
    delay_ms(100);
    Weight_Maopi = read_hx711_24bit();
    Weight_Maopi = read_hx711_24bit(); /* 多读一次确保稳定 */
}

/* 获取重量 返回g单位的重量 */
float  get_weight(void)
{
    float HX711_Buffer;
    float maopia;
    float Weight_Shiwu;
    unsigned long ans = read_hx711_24bit();
    maopia =((float) Weight_Maopi / GapValue/Volt_to_weight); /* 毛皮重量转换成g */
    HX711_Buffer = ((float) ans / GapValue/Volt_to_weight); /* 此时称重转换成g */
    if (HX711_Buffer > maopia)
    {
        Weight_Shiwu = HX711_Buffer - maopia; /* 计算实物的实际重量 */
        return (Weight_Shiwu); /* 测量正确返回g单位的重量数值 */
    }
    if(HX711_Buffer <= maopia+0.1 && HX711_Buffer >= maopia-0.1)
        return (0); /* 称重不符合规范(HX711_Buffer > Weight_Maopi) 返回2 */
    return HX711_Buffer - maopia;
}

void instr_num(void){

    static int num[3]={0};
    static float weights=0.0;
    char display_str[32];
    float campar ;
    static float ans = 0;
      // __delay_cycles(2000000);

    delay_ms(10);
    campar = get_weight();
    ans =campar;
    campar = get_weight();

    if(campar<=0.05 && campar>=-0.05) {
        num[0] = 0;num[1] = 0;num[2] = 0; weights=0;
    }
    if (campar >ans-0.1 && campar <ans+0.1 && (campar - weights <= 1 || campar - weights >= -1) )
    {
        if ((campar - weights >= 0.70 && campar - weights <= 0.90)) {   /*74LS00*/
            num[2]++;
            weights = campar;
        }

        else if ((campar - weights >= 0.28 && campar - weights <= 0.42)) {  /*OP07*/
            num[1]++;
            weights = campar;

        }
        else if (campar - weights >= 0.15 && campar - weights <= 0.25) {  /*9013*/
            num[0]++;
            weights = campar;
        }

        if(weights-campar >= 0.1 && weights-campar <= 0.25){        /*9013*/
            if(num[0]!=0)
                    num[0]--;
            weights = campar;
        }
        else if(weights-campar >= 0.31 && weights-campar <= 0.42){      /*OP07*/
            if(num[1]!=0)
                    num[1]--;
            weights = campar;
        }
        else if(weights-campar >= 0.70 && weights-campar <= 0.90){      /*74LS00*/
            if(num[2]!=0)
                    num[2]--;
            weights = campar;
        }
    }
        sprintf(display_str, "9013: %d  ", num[0]);
        SSD1306_ShowString(0, 2, display_str, 16);
        sprintf(display_str, "OP07: %d  ", num[1]);
        SSD1306_ShowString(0, 4, display_str, 16);
        sprintf(display_str, "74LS00: %d   ", num[2]);
        SSD1306_ShowString(0, 6, display_str, 16);
}

