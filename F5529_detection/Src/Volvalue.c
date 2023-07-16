#include <msp430.h>
#include "LCD12864.h"
#include <stdio.h>
#define   Num_of_Results  64
volatile unsigned int results[Num_of_Results];//存储ADC0采集到的电压值
                                            // Needs to be global in this
                                            // example. Otherwise, the
                                            // compiler removes it because it
float curvalue[256];
float curvalue1[256];
float volvalue[512];
char tab10[64];
extern float freq,f3;
float freq1;
float t;
unsigned int index = 0,f=0;
int i=0;
void Volvalue()
{
    WDTCTL = WDTPW+WDTHOLD;
    TA1CCTL0 = CCIE;                          // CCR0 interrupt enabled
     freq1=freq;
//  if(freq1>200)freq1=200;
//    TA1CCR0 = 125000/freq1;                     //64点一个周期
//    TA1CCR0 = 62500/freq1;                     //64点一个周期
//    TA1CCR0 = 31250/freq1;                     //128点一个周期
      TA1CCR0 = 15625/freq1;
//    TA1CCR0 = 7812.5/freq1;
//    TA1CCR0 = 312;
    TA1CTL = TASSEL_2 + MC_1 + TACLR;
    P6SEL = 0x0F;                             // Enable A/D channel inputs
    ADC12CTL0 = ADC12ON+ADC12MSC+ADC12SHT0_8; // Turn on ADC12, set sampling time
    ADC12CTL1 = ADC12SHP+ADC12CONSEQ_2;
    ADC12MCTL0 = ADC12INCH_0;                 // ref+=AVcc, channel = A0
//    ADC12MCTL1 = ADC12INCH_1;                 // ref+=AVcc, channel = A1
//    ADC12MCTL2 = ADC12INCH_2;                 // ref+=AVcc, channel = A2
//    ADC12MCTL3 = ADC12INCH_3+ADC12EOS;        // ref+=AVcc, channel = A3, end seq.
//    ADC12IE = 0x00;                           // Enable ADC12IFG.3
    ADC12CTL0 |= ADC12ENC;                    // Enable conversions
    ADC12CTL0 |= ADC12SC;                     // Start conversion
//    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM4, Enable interrupts
    __no_operation();                         // For debugger

}
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void)
{
     if(index<512)
     {
        volvalue[index]=ADC12MEM0;
     }
     else
     {
          index = 0;
          if(f3==0)f=0;
          f3=1;
     }
          index++;
     if(f<256)
     {
       curvalue[f]=ADC12MEM0;
     }
        f++;
        if(f>=256) f=256;
//       for(i=0;i<64;i++)
//       {
//           curvalue[i]=curvalue1[i];
//       }
//       f=0;
//   }
          LPM0_EXIT;
}
//#pragma vector=ADC12_VECTOR
//__interrupt void ADC12ISR (void)
//{
//  static unsigned int indexs = 0;                                 // Vector 12:  ADC12IFG3
//  volvalue[indexs] = ADC12MEM1*3.3/4095;           // Move A0 results, IFG is cleared
//    indexs++;                                // Increment results index, modulo; Set Breakpoint1 here
//    if (indexs == 64)
//    {
//      (indexs = 0);
//    }
//    __bic_SR_register_on_exit(LPM4_bits);
//
//}
