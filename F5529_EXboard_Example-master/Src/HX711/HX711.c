/*
 * HX711.c
 *
 *  Created on: 2023��7��2��
 *      Author: 37756
 */
#include <msp430F5529.h>
#include <stdio.h>
#include "HX711/HX711.h"
#include "SSD1306/SSD1306_api.h"
#define SCK_L       P4OUT   &= ~BIT2
#define SCK_H       P4OUT   |= BIT2
#define SDA_IN      (P4IN & BIT1)
#define GapValue    116.18    /* ��ȡ����HX711Ϊһ���Ĵ�����ֵ��λ������g���������GapValueֵ���ǻ����g��λ */
#define Volt_to_weight   82.16   /*��ѹת����*/
unsigned long Weight_Maopi; /* ëƤ�أ��ϵ��Ͷ�ȡ��������� */

#define CPU_F ((double)16000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
/*
 * ��ʼ��HX711�˿�
 *  3.3V����
 *  4.0��SCK
 *  4.1��DT
 * */
void init_hx711port(void)
{
    P4DIR |= BIT2; /* ��Ƭ��P4.0������� */
    P4OUT &= ~BIT2; /* ��Ƭ��P4.0��������͵�ƽ */

    P4DIR &= ~BIT1; /* ��Ƭ��P4.1�������� */
    /* P4REN |= BIT1;   //��Ƭ��P4.1������������ʹ�� F149��Ƭ������û�ڲ����� */
    //P4OUT |= BIT1; /* ��Ƭ��P4.1���������������� */
    get_maopi();
}

/* ��ȡHX711�ڲ������� */
unsigned long read_hx711_24bit(void)
{
    unsigned long Count = 0;
    unsigned char i;
    SCK_L; /* SCL=0;ʹ��AD��PD_SCL �õͣ� */
    while ( SDA_IN)
        ;
    /* ADת��δ������ȴ�������ʼ��ȡ ѭ����ȡ24�� */
    for (i = 0; i < 24; i++)
    {
        SCK_H; /* SCL = 1; PD_SCL �øߣ��������壩 */
        Count = Count << 1; /*�½�����ʱ����Count����һλ���Ҳಹ�� */
        delay_us(2);
        SCK_L; /* SCL=0; PD_SCL �õ� */
        if ( SDA_IN)
            Count++; /* if(SDA) Count++; */
        delay_us(2);
    }
    SCK_H; /* SCL=1; */
    Count = Count ^ 0x800000; /* ��25�������½�����ʱ��ת������ */
    delay_us(1);
    SCK_L; /* SCL=0; */
    return (Count);
}

/*
 *
 * ��ȡëƤ����
 * �ϵ��ʼ��ʱ��ִ��
 *
 */
void get_maopi(void)
{
    delay_ms(100);
    Weight_Maopi = read_hx711_24bit();
    Weight_Maopi = read_hx711_24bit(); /* ���һ��ȷ���ȶ� */
}

/* ��ȡ���� ����g��λ������ */
float  get_weight(void)
{
    float HX711_Buffer;
    float maopia;
    float Weight_Shiwu;
    unsigned long ans = read_hx711_24bit();
    maopia =((float) Weight_Maopi / GapValue/Volt_to_weight); /* ëƤ����ת����g */
    HX711_Buffer = ((float) ans / GapValue/Volt_to_weight); /* ��ʱ����ת����g */
    if (HX711_Buffer > maopia)
    {
        Weight_Shiwu = HX711_Buffer - maopia; /* ����ʵ���ʵ������ */
        return (Weight_Shiwu); /* ������ȷ����g��λ��������ֵ */
    }
    if(HX711_Buffer <= maopia+0.1 && HX711_Buffer >= maopia-0.1)
        return (0); /* ���ز����Ϲ淶(HX711_Buffer > Weight_Maopi) ����2 */
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

