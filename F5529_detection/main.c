#include <msp430F5529.h>
#include "UCS_INIT.h"
#include "TIMER_INIT.h"
#include "KEY_SIG.h"
#include "LCD12864.h"
#include "ADC12_INIT.h"
#include <stdio.h>
#include <math.h>
#include <Volvalue.h>
#include"UART.h"
#include "SSD1306/SSD1306_api.h"
#define CPU_F ((double)16000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define Delay_Nms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))
void fft();
#define SMCLK 4000000
#define FreqHz 200                                 //周期(Hz)
#define Duty   60                                  //占空比(%)
#define PI  3.14
#define Period SMCLK/FreqHz                        //一般模式下周期       转换成计数值
#define Bright Period*Duty/100                     //一般模式下占空比   转换成计数值

#define DeadPeriod SMCLK/FreqHz/2                  //死区模式下周期       转换成计数值
#define DeadBright DeadPeriod*Duty/100             //死区模式下占空比   转换成计数值

unsigned int one_Percent = Period/100;             //占空比1%步进        转换成计数值
unsigned int ten_Percent = Period/10;              //占空比10%步进     转换成计数值
unsigned int max_Percent = Period;                 //占空比最大值           转换成计数值
unsigned int min_Percent = 0;                      //占空比最小值           转换成计数值

/*---------------------------------------------------------------------*/
float ADC_Value=0;                    //ADC采样值
/*-----------------------------------------------------频率，占空比，相位计算相关*/
//extern  unsigned int index;
extern unsigned int cap_count,cap1value,cap2value,f;
extern unsigned int cap_flag,cap_finish,uptime,downtime,addtime,i;
float freq=0,duty=0,f3=0;
float Vmax=0,Vmin=9999,gg=0,sum=0,Vpp=0,VV=0,vv=0;
extern float volvalue[512],curvalue[256];       //存储ADC0采集到的电压值
float data_i[256],data_r[256],curvalue2[256];
unsigned int kk=1;
long x=0, b=0,d=0;
int c,mm=0,ff=0;
char tab1[48],tab2[48],tab3[48],tab4[48];
/*--------------------------------------------------------------------*/

int main(void)


{

    WDTCTL = WDTPW | WDTHOLD;           //Stop watchdog timer
    UCS_init();                         //时钟初始化                         ok
   // KEY_SIG_init();                     //独立按键,LED初始化(P2.1,P1.1,P1.0,P4.7) ok
    //Ini_Lcd12864();                     //LCD12864并行初始化(P6.6,P6.5,P6.4,P3)  ok
    Uart1_Init();
    SSD1306_init();
  //Volvalue();
  ADC12_init();                       //片内ADC初始化                          ok
    TA0_Init();                         //定时器A0初始化                          ok                       // For debugger

    //GPIO初始化，对应LaunchPad板载的两颗LED灯
       P1DIR |= BIT0;
       P1OUT |= BIT0;
       P4DIR |= BIT7;
       P4OUT &= ~BIT7;

    _enable_interrupts();       //使能总中断
    while(1)
    {

        cap_count=0;
            while(cap_count<2);                         //等待第二次捕获
            freq  = SMCLK/4.0/cap1value;                //计算频率//
            Volvalue();
//            for(b=0;b<512;b++)
//            {
            for(b=0;b<512;b++)
            {
//                if(volvalue[b]>Vmax) Vmax=volvalue[b];
//                if(volvalue[b]<Vmin) Vmin=volvalue[b];
                gg +=volvalue[b]*3.3/4095;
           }
//            b=0;
//            mm++;
//            vv +=gg/512;
//            gg=0;
//            if(mm==8)
//            {
//                Vpp=vv/8*1000;
//                mm=0;
//                b=512;
//                vv=0;
//            }
//            }
            if(f==256)
            {
                for(ff=0;ff<256;ff++)
                {
               if(curvalue[ff]>Vmax) Vmax=curvalue[ff];
                if(curvalue[ff]<Vmin) Vmin=curvalue[ff];
                    curvalue2[ff]=curvalue[ff];
                }
            }
            fft();
            Vmax=Vmax*3.3/4095;
            Vmin=Vmin*3.3/4095;
          Vpp = gg/512*1000;
            if(freq>=45&&freq<=90)
            {
                Vpp=0.7174*Vpp + 4.4218;
                if(Vpp<32)
                Vpp= 1.0567*Vpp - 2.8599;
                if(Vpp<11.6) Vpp=Vpp-1.2;
            }
            if(freq<=200&&freq>90)
           {
                 Vpp = 0.7138*Vpp + 1.1999;
                 if(Vpp<22&&freq<140)
                 Vpp =1.1198*Vpp - 1.7563;
                 if(Vpp<22&&freq>=140)
                 Vpp =1.0435*Vpp - 1.5022;
                 if(Vpp<11.5) Vpp=Vpp-1.195;
           }
            if(freq>200&&freq<400)
          {
                Vpp= 0.714*Vpp + 0.917;
                if(Vpp<22)
                Vpp =1.0561*Vpp - 1.5616;
                if(Vpp<11.5) Vpp=Vpp-1.185;
           }
             if(freq>=400&&freq<600)
          {
                 Vpp= 0.7144*Vpp + 1.1556;
                 if(Vpp<22)
                 Vpp= 1.0651*Vpp - 1.7735;
                 if(Vpp<11.5) Vpp=Vpp-1.139;
           }
             if(freq>=600&&freq<800)
          {
                 Vpp= 0.7157*Vpp + 0.9967;
                 if(Vpp<22)
                 Vpp= 1.0587*Vpp - 1.3811;
                 if(Vpp<11.5) Vpp=Vpp-1.195;
           }
             if(freq>=800)
          {
                 Vpp= 0.7177*Vpp + 0.4967;
                 if(Vpp<11.65) Vpp=Vpp-1.2;
           }
//          VV=Vpp/10;
            //KEY_SIG_init();
            gg=0;
//          cap_count=0;
//          while(cap_count<2);                         //等待第二次捕获
//          freq  = SMCLK/2.0/cap1value;                //计算频率//
//          cap_finish=0;
//          while(!cap_finish);                         //
//          duty  = uptime*100.0;
//          duty  = duty/(uptime+downtime);             //
//          freq  = SMCLK/2.0/(uptime+downtime);        //
            sprintf(tab1,"F:%4.2f  Hz  ",freq);
            printf("%s",tab1);
            SSD1306_ShowString(0, 0, tab1, 16);
             sprintf(tab2,"I:%4.2f   mA   ",Vpp);
             printf("%s",tab2);
             SSD1306_ShowString(0, 2, tab2, 16);
             sprintf(tab3,"V:%4.2f   mv    ",Vmax);
             printf("%s",tab3);
             SSD1306_ShowString(0, 4, tab3, 16);
             sprintf(tab4,"V:%4.2f   mv    ",Vmin);
             printf("%s",tab4);
             SSD1306_ShowString(0, 6, tab4, 16);

            //KEY_SIG_init();
//            if(kk==1)
//               {
//                //clrddram();
//                sprintf(tab1,"F:%4.3f   Hz  ",freq);
//                printf(tab1);
//                sprintf(tab2,"I:%4.3f   mA   ",Vpp);
//                printf(tab2);
//                sprintf(tab3,"V:%4.3f   mv    ",Vmax);
//                printf(tab3);
//                sprintf(tab4,"V:%4.3f   mv    ",Vmin);
//                printf(tab4);
//               }
//            else if(kk==2)
//               {
//                //clrddram();
//                sprintf(tab1,"直流分量:%-4.3f",data_i[0]);   //将频率记入Str2数组中
//                sprintf(tab2,"基波:%-4.3f",data_i[1]);
//                sprintf(tab3,"二次谐波:%-4.3f",data_i[2]);
//                sprintf(tab4,"三次谐波:%-4.3f",data_i[3]);
//               }
//            else if(kk==3)
//               {
//                //clrddram();
//                sprintf(tab1,"四次谐波%-4.3f",data_i[4]);   //将频率记入Str2数组中
//                sprintf(tab2,"五次谐波%-4.3f",data_i[5]);
//                sprintf(tab3,"六次谐波%-4.3f",data_i[6]);
//                sprintf(tab4,"七次谐波%-4.3f",data_i[7]);
//                }
//               lcd12864_disp_str(1, 1, tab1);
//               lcd12864_disp_str(1, 2, tab2);
//               lcd12864_disp_str(1, 3, tab3);
//               lcd12864_disp_str(1, 4, tab4);
            Vmax=0;
            Vmin=9999;
            Delay_Nms(400);

            //GPIO翻转，LED灯发生变化
             P1OUT ^= BIT0;
             P4OUT ^= BIT7;

    }
}
void P11_Onclick(void)
{
    clrddram();
    kk--;
    if(kk==0)
        kk=3;
}
void P21_Onclick(void)
{
    clrddram();
    kk++;
    if(kk==4)
    kk=1;
}
void fft(void)
{ int x0,x1,x2,x3,x4,x5,x6,x7,xx,b1,c1,L,k1,f_1;
  float TR,TI;
  unsigned ii,jj;
  for(f_1=0;f_1<256;f_1++)
  {
      data_r[f_1]=curvalue2[f_1]/4095*3.3;
  }
  if(f==256)f3=0;
  ///////////////倒序///////////////////////
  for(ii=0;ii<256;ii++)
  {
      x0=x1=x2=x3=x4=x5=x6=x7=0;
      x0=ii&0x01; x1=(ii/2)&0x01; x2=(ii/4)&0x01;
      x3=(ii/8)&0x01;x4=(ii/16)&0x01;x5=(ii/32)&0x01;
      x6=(ii/64)&0x01;x7=(ii/128)&0x01;
      xx=x0*128+x1*64+x2*32+x3*16+x4*8+x5*4+x6*2+x7;
      data_i[xx]=data_r[ii];
  }
  for(ii=0;ii<256;ii++)
  {
      data_r[ii]=data_i[ii];
      data_i[ii]=0;
  }
  ///////////////fft处理函数///////////////
  for(L=1;L<=8;L++)
  {
      b1=1;ii=L-1;
      while(ii>0)
      {b1=b1*2;ii--;}
      for(jj=0;jj<=b1-1;jj++)
      {
          c1=1;ii=8-L;
          while(ii>0)
          {c1=c1*2;ii--;}
          c1=c1*jj;
          for(k1=jj;k1<256;k1=k1+2*b1)
          {
TR=data_r[k1+b1]*cos(2*PI*c1/256)-data_i[k1+b1]*sin(2*PI*c1/256);
TI=data_r[k1+b1]*sin(2*PI*c1/256)+data_i[k1+b1]*cos(2*PI*c1/256);
data_r[k1+b1]=data_r[k1]-TR;
 data_i[k1+b1]=data_i[k1]-TI;
 data_r[k1]=data_r[k1]+TR;
 data_i[k1]=data_i[k1]+TI;
  } }}
  for(xx=0;xx<=256;xx++)
  {
  data_i[xx]=sqrt(data_r[xx]*data_r[xx]+data_i[xx]*data_i[xx])/256*1000;
  }
}
