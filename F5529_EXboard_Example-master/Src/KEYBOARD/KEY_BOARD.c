#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MSP430F5529.h"
#include"SSD1306/SSD1306_api.h"
#include"HX711/HX711.h"
void KEY_IODect(void);
void WDT_init(void);
void R0C0_Onclick();
void R0C1_Onclick();
void R0C2_Onclick();
void R0C3_Onclick();
void R1C0_Onclick();
void R1C1_Onclick();
void R1C2_Onclick();
void R1C3_Onclick();
void R2C0_Onclick();
void R2C1_Onclick();
void R2C2_Onclick();
void R2C3_Onclick();
void R3C0_Onclick();
void R3C1_Onclick();
void R3C2_Onclick();
void R3C3_Onclick();
void P11_Onclick(void);
void P21_Onclick(void);

char temp [32]={0};
unsigned char j =0;

/*-----------------------------------------------------------------*/
//                      ��λ����
//         P1.2    P1.3    P1.4    P1.5     PIN
//          C0      C1      C2      C3
// P6.0 R0  7       8       9
//
// P6.1 R1  4       5       6
//
// P6.2 R2  1       2       3
//
// P6.3 R3  CLR     0       .
//
// POUT
/*-----------------------------------------------------------------*/

void KEY_BOARD_init(void)
{
    P6DIR |=  BIT0 + BIT1 + BIT2 + BIT3 ;

    //-----��ϻ�е�����������ڲ���������-----

    P1DIR &=~ (BIT2 + BIT3 + BIT4 + BIT5);


    P1REN |=  BIT2 + BIT3 + BIT4 + BIT5;        //�����ڲ�����������


    P1OUT |=  BIT2 + BIT3 + BIT4 + BIT5;        //����������Ϊ����

    P1DIR &=~ BIT1;
    P1REN |=  BIT1;
    P1OUT |=  BIT1;

    P2DIR &=~ BIT1;
    P2REN |=  BIT1;
    P2OUT |=  BIT1;

	WDT_init();

	//Input_str = (char *)malloc(sizeof(char));

}

/******************************************************************************************************
 * ��       �ƣ�WDT_init()
 * ��       �ܣ��趨WDT��ʱ�ж�Ϊ16ms������WDT��ʱ�ж�ʹ��
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����WDT��ʱ�жϵ�ʱ��Դѡ��ACLK��
 * ��       ������
 ******************************************************************************************************/
void WDT_init(void)
{
	//-----�趨WDTΪ16ms�ж�-----
   WDTCTL  = WDT_ADLY_16;
	//-----WDT�ж�ʹ��-----
   SFRIE1 |= WDTIE;
}

/******************************************************************************************************
 * ��       �ƣ�WDT_ISR()
 * ��       �ܣ���ӦWDT��ʱ�жϷ���
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����WDT��ʱ�ж϶�ռ�ж����������������һ���ж��ж��¼���Ҳ�����˹������־λ��
 *                  ���ԣ���WDT��ʱ�жϷ����Ӻ����У�ֱ�ӵ���WDT�¼��������Ϳ����ˡ�
 * ��       ������
 ******************************************************************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR(void)
{
	//-----����Port1�¼���⺯��-----
	KEY_IODect();		//���ͨ�����������¼�������
}

/******************************************************************************************************
 * ��       �ƣ�P1_IODect()
 * ��       �ܣ��ж��Ƿ��м������£��ĸ��������£���������ӦIO���ж��¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       �����������������ɨ��Ľ������֪�������Ƿ񱻰���
 * ��       ������
 ******************************************************************************************************/
void KEY_IODect(void)
{
    static unsigned char KEY_Now[4][4] = {0};
    static unsigned char KEY11_Now=0, KEY21_Now=0;
    unsigned char KEY_Past[4][4] = {0};
    unsigned char KEY11_Past=0,KEY21_Past=0;
    unsigned char row = 0 ;

    for(row = 0 ; row < 4 ; row++)
    {
        switch(row)
        {
            case 0:
            {
                P6OUT |= BIT1 + BIT2 + BIT3 ;
                P6OUT &=~BIT0;
                break;
            }
            case 1:
            {

                P6OUT |= BIT0 + BIT2 + BIT3 ;
                P6OUT &=~BIT1;
                break;
            }
            case 2:
            {
                P6OUT |= BIT0 + BIT1 + BIT3 ;
                P6OUT &=~BIT2;
                break;
            }
            case 3:
            {

                P6OUT |= BIT0 + BIT1 + BIT2 ;
                P6OUT &=~BIT3;
                break;
            }
            default:break;
        }
        KEY_Past[row][0] = KEY_Now[row][0];
        KEY_Past[row][1] = KEY_Now[row][1];
        KEY_Past[row][2] = KEY_Now[row][2];
        KEY_Past[row][3] = KEY_Now[row][3];

        if((P1IN&BIT2)!=0)  KEY_Now[row][0] = 1;
            else            KEY_Now[row][0] = 0;
        if((P1IN&BIT3)!=0)  KEY_Now[row][1] = 1;
            else            KEY_Now[row][1] = 0;
        if((P1IN&BIT4)!=0)  KEY_Now[row] [2] = 1;
            else            KEY_Now[row][2] = 0;
        if((P1IN&BIT5)!=0)  KEY_Now[row][3] = 1;
            else            KEY_Now[row][3] = 0;

        switch(row)
        {
            case 0:
            {
                if((KEY_Past[row][0] == 1)&&(KEY_Now[row][0] == 0)) R0C0_Onclick();
                if((KEY_Past[row][1] == 1)&&(KEY_Now[row][1] == 0)) R0C1_Onclick();
                if((KEY_Past[row][2] == 1)&&(KEY_Now[row][2] == 0)) R0C2_Onclick();
                if((KEY_Past[row][3] == 1)&&(KEY_Now[row][3] == 0)) R0C3_Onclick();
                break;
            }
            case 1:
            {
                if((KEY_Past[row][0] == 1)&&(KEY_Now[row][0] == 0)) R1C0_Onclick();
                if((KEY_Past[row][1] == 1)&&(KEY_Now[row][1] == 0)) R1C1_Onclick();
                if((KEY_Past[row][2] == 1)&&(KEY_Now[row][2] == 0)) R1C2_Onclick();
                if((KEY_Past[row][3] == 1)&&(KEY_Now[row][3] == 0)) R1C3_Onclick();
                break;
            }
            case 2:
            {
                if((KEY_Past[row][0] == 1)&&(KEY_Now[row][0] == 0)) R2C0_Onclick();
                if((KEY_Past[row][1] == 1)&&(KEY_Now[row][1] == 0)) R2C1_Onclick();
                if((KEY_Past[row][2] == 1)&&(KEY_Now[row][2] == 0)) R2C2_Onclick();
                if((KEY_Past[row][3] == 1)&&(KEY_Now[row][3] == 0)) R2C3_Onclick();
                break;
            }
            case 3:
            {
                if((KEY_Past[row][0] == 1)&&(KEY_Now[row][0] == 0)) R3C0_Onclick();
                if((KEY_Past[row][1] == 1)&&(KEY_Now[row][1] == 0)) R3C1_Onclick();
                if((KEY_Past[row][2] == 1)&&(KEY_Now[row][2] == 0)) R3C2_Onclick();
                if((KEY_Past[row][3] == 1)&&(KEY_Now[row][3] == 0)) R3C3_Onclick();
                break;
            }
            default:break;
        }

    }

    KEY11_Past=KEY11_Now;
    KEY21_Past=KEY21_Now;

    if((P1IN&BIT1)!=0)  KEY11_Now=1;
        else            KEY11_Now=0;

    if((P2IN&BIT1)!=0)  KEY21_Now=1;
        else            KEY21_Now=0;

    if((KEY11_Past==1)&&(KEY11_Now==0)) P11_Onclick();
    if((KEY21_Past==1)&&(KEY21_Now==0)) P21_Onclick();

}


/******************************************************************************************************
 * ��       �ƣ�RxCx_Onclick()
 * ��       �ܣ�RxCx���ж��¼�����������λ��RxCx�ļ������º���һ����ʲô
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ʹ���¼�����������ʽ��������ǿ�������ֲ�ԺͿɶ���
 * ��       ������
 ******************************************************************************************************/
void R0C0_Onclick()
{
    printf("R0C0\r\n");
    temp[j]='1';
    SSD1306_ShowChar(j*8,3, temp[j++],16);
}

void R0C1_Onclick()
{
    printf("R0C1\r\n");
    temp[j]='2';
    SSD1306_ShowChar(j*8,3, temp[j++],16);
}

void R0C2_Onclick()
{
    printf("R0C2\r\n");
    temp[j]='3';
    SSD1306_ShowChar(j*8,3,temp[j++],16);
}

void R0C3_Onclick()
{
    printf("R0C3\r\n");
}

void R1C0_Onclick()
{

    printf("R1C0\r\n");
}

void R1C1_Onclick()
{

    printf("R1C1\r\n");
}

void R1C2_Onclick()
{

    printf("R1C2\r\n");
}

void R1C3_Onclick()
{
    printf("R1C3\r\n");
}
void R2C0_Onclick()
{

    printf("R2C0\r\n");
}

void R2C1_Onclick()
{
    printf("R2C1\r\n");
}

void R2C2_Onclick()
{
    printf("R2C2\r\n");
}

void R2C3_Onclick()
{
    printf("R2C3\r\n");
    temp[j]='*';
    SSD1306_ShowChar(j*8,3,temp[j++],16);
}
void R3C0_Onclick()
{
    printf("R3C0\r\n");
}

void R3C1_Onclick()
{
    printf("R3C1\r\n");
}

void R3C2_Onclick()
{
    printf("R3C2\r\n");
}

void R3C3_Onclick()
{
    printf("R3C3\r\n");
    temp[j]='=';
    SSD1306_ShowChar(j*8,3,temp[j++],16);
    int ans =0;char sign ;char display_str[64];
    float first = 0.0,second =0.0,result;
    for(ans =0;temp[ans]!='+'|temp[ans]!='-'|temp[ans]!='*'|temp[ans]!='/';ans++){
       if(temp[ans+1]!='+'|temp[ans+1]!='-'|temp[ans+1]!='*'|temp[ans+1]!='/') first =(first+temp[ans])*10;
       else first =first+temp[ans];
    }
    sign = ans;
    for(ans =ans+1;temp[ans]!='=';ans++){
           if(temp[ans+1]!='=') second =(second+temp[ans])*10;
           else second =second+temp[ans];
        }
    if(temp[sign]=='+') {
        result =first+second;
        sprintf(display_str, "%f",result );
       SSD1306_ShowString(j*8,3, display_str, 16);
    }
    j=0;
    __delay_cycles(500000);
    SSD1306_clear();
}

void P11_Onclick()
{
    get_maopi();
    printf("P11\r\n");

}

void P21_Onclick()
{

    printf("P21\r\n");

}


