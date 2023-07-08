/*
 * ADS1256.c
 *
 *  Created on: 2021年6月25日
 *      Author: Zuoge
 */

/*
 * 2022.5.15:
 * 针对于V2.1/V2.2底板，
 * ADS1256支持软件SPI或者硬件SPI驱动，
 * 但是目前存在一些bug，单通道使用目前没有问题，
 * 多通道使用也没问题只是数据输出有点反人类，建议多看看数据手册，
 * 注意：ADS1256与ADS1118不可同时使用！！！！！
 * SCK  ->  P4.3  ----|
 * MOSI ->  P4.1  ----|
 * MISO ->  P4.2  ----|
 * CS   ->  P4.0  ----|---->UCB1SPI
 * RST  ->  P2.0
 * DRDY ->  P1.6
 *
 */


#include <stdio.h>
#include "msp430.h"
#include "ADS1256.h"

unsigned char RXflag = 0;
unsigned char TXflag = 1;

unsigned char STATUS_BUF = 0x01;
unsigned char MUX_BUF = 0x01;
unsigned char ADCON_BUF = 0x20;
//unsigned char DRATE_BUF = 0xF0;
unsigned char IO_BUF = 0xE0;

unsigned char scale = PGA1_5V;
unsigned char rate = AVE_3K_DRATE_10;
//unsigned char rate = AVE_600_DRATE_50;
//unsigned char rate = AVE_12K_DRATE_2_5;


#ifdef HARD_SPI

void ads1256_spi_write_byte(unsigned char dat)
{
//    ADS1256_CS_LOW();
//    while((UCB1STAT & UCBBUSY));
//    UCB1TXBUF = dat;
//    while((UCB1STAT & UCBBUSY));
//    ADS1256_CS_HIGH();
    while(TXflag == 0);
    TXflag = 0;
    UCB1TXBUF = dat;
}

unsigned char ads1256_spi_read_byte()
{
    unsigned char dat = 0;
//    ADS1256_CS_LOW();
//    while((UCB1STAT & UCBBUSY));
//    dat = UCB1RXBUF;
//    while((UCB1STAT & UCBBUSY));
//    ADS1256_CS_HIGH();
//    return dat;
    while(RXflag == 0);
    RXflag = 0;
    dat = UCB1RXBUF;
    return dat;
}

void ADS1256_port_init()
{
    P4SEL |= BIT0 + BIT1 + BIT2 + BIT3;
    UCB1CTL1 = UCSWRST;
    UCB1CTL0 = UCCKPH + UCMST + UCMODE_2 + UCSYNC + UCMSB;
    UCB1CTL0 &= ~(UCCKPL);
    UCB1CTL1 = UCSWRST + UCSSEL_2;
    UCB1BR0 = 10;
    UCB1BR1 = 0;
    UCB1CTL1 &= ~UCSWRST;

    UCB1IE |= UCRXIE + UCTXIE;

    ADS1256_CS_DIR_OUT();
    ADS1256_RST_DIR_OUT();
    ADS1256_DRDY_DIR_IN();
    ADS1256_RST_LOW();
    ADS1256_RST_HIGH();
}

#endif

#ifdef SOFT_SPI

unsigned char ads1256_spi_read_byte()
{
    unsigned char i;
    unsigned char dat=0;
    for(i=0;i<8;i++)
    {
        ADS1256_SPI_SCK_HIGH();
        dat<<=1;
        if(ADS1256_SPI_MISO_GET())
            dat|=0x01;
        else
            dat&=0xFE;
        ADS1256_SPI_SCK_LOW();
    }
    ADS1256_SPI_SCK_LOW();
    return dat;
}

void ads1256_spi_write_byte(unsigned char dat)
{
    unsigned char i;
    ADS1256_SPI_SCK_LOW();
    for(i=0;i<8;i++)
    {
        ADS1256_SPI_SCK_HIGH();
        if(dat & 0x80)
            ADS1256_SPI_MOSI_HIGH();
        else
            ADS1256_SPI_MOSI_LOW();
        dat<<=1;
        ADS1256_SPI_SCK_LOW();
    }
}

void ADS1256_port_init()
{
    ADS1256_SPI_MISO_DIR_IN();
    ADS1256_SPI_MOSI_DIR_OUT();
    ADS1256_SPI_SCK_DIR_OUT();

    ADS1256_RST_DIR_OUT();
    ADS1256_CS_DIR_OUT();
    ADS1256_DRDY_DIR_IN();
    ADS1256_CS_HIGH();
    ADS1256_RST_LOW();
    ADS1256_RST_HIGH();
}

#endif

unsigned long ADS1256_Rec_data()
{
    unsigned long rec_data = 0;
    while(ADS1256_DRDY_GET());
    //ADS1256_CS_LOW();
    rec_data =rec_data + ads1256_spi_read_byte();
    rec_data <<= 8;
    rec_data =rec_data + ads1256_spi_read_byte();
    rec_data <<= 8;
    rec_data =rec_data + ads1256_spi_read_byte();
    //rec_data <<= 8;
    //ADS1256_CS_HIGH();
    return rec_data;
}



//采样速率速率
void ADS1256_Rate_Config(unsigned char rate_buf)
{
    rate = rate_buf;
    ADS1256_CS_LOW();
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(WREG | DRATE);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(0x00);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(rate);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(SELFCAL);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(SYNC);
    //_delay_cycles(16000);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(WAKEUP);
    ADS1256_CS_HIGH();
}

void ADS1256_init()
{
    ADS1256_port_init();
    ADS1256_CS_LOW();
    ads1256_spi_write_byte(WAKEUP);
//    ads1256_spi_write_byte(SYNC);
//    ads1256_spi_write_byte(WAKEUP);
    //while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(SELFCAL);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(WREG | STATUS);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(0x00);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(0x22);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(WREG | DRATE);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(0x00);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(rate);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(SELFCAL);
//    ads1256_spi_write_byte(SELFOCAL);
//    while(ADS1256_DRDY_GET());
//    ads1256_spi_write_byte(SELFGCAL);
    while(ADS1256_DRDY_GET());
    //ads1256_spi_write_byte(STANDBY);
    ADS1256_CS_HIGH();
}

//设置PGA增益
void ADS1256_Scale_Config(unsigned char scale_buf)
{
    scale = scale_buf;
    while(ADS1256_DRDY_GET());
    ADS1256_CS_LOW();
    ads1256_spi_write_byte(WREG | ADCON);
    ads1256_spi_write_byte(0x00);
    ADCON_BUF |= scale_buf;
    ads1256_spi_write_byte(ADCON_BUF);
    ads1256_spi_write_byte(SELFCAL);
    ADS1256_CS_HIGH();
    while(ADS1256_DRDY_GET());
}



//读取
float ADS1256_Single_Read(unsigned char channel1 , unsigned char channel2) //, unsigned char scale)
{
    float value;
    long value_data;
    ADS1256_CS_LOW();
    ads1256_spi_write_byte(WAKEUP);
    while(ADS1256_DRDY_GET());


    ads1256_spi_write_byte(WREG | MUX);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(0x00);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte( (channel1 << 4) | channel2 );

//    ADCON_BUF |= scale;
//    ads1256_spi_write_byte(ADCON_BUF);
    while(ADS1256_DRDY_GET());
//    ads1256_spi_write_byte(WREG | DRATE);
//    ads1256_spi_write_byte(0x00);
//    ads1256_spi_write_byte(rate);
//    ads1256_spi_write_byte(SELFCAL);
//    ads1256_spi_write_byte(SYNC);
//    _delay_cycles(16000);
//    ads1256_spi_write_byte(WAKEUP);
    while(ADS1256_DRDY_GET());
    //_delay_cycles(1600000);
    ads1256_spi_write_byte(RDATA);
    //ADS1256_CS_HIGH();
    _delay_cycles(16000);
    //ADS1256_CS_LOW();
    value_data = ADS1256_Rec_data();
    //ads1256_spi_write_byte(STANDBY);
    ADS1256_CS_HIGH();
    if(value_data > 0x7FFFFF) value_data -= 0x1000000;
    switch(scale)
    {
        case PGA1_5V:           value = (float)value_data * ((float)5/8388606);break;
        case PGA2_2_5V:         value = (float)value_data * ((float)2.5/8388607);break;
        case PGA4_1_25V:        value = (float)value_data * ((float)1.25/8388607);break;
        case PGA8_625MV:        value = (float)value_data * ((float)0.625/8388607);break;
        case PGA16_312_5MV:     value = (float)value_data * ((float)0.3125/8388607);break;
        case PGA32_156_25MV:    value = (float)value_data * ((float)0.15625/8388607);break;
        case PGA64_78_125MV:    value = (float)value_data * ((float)0.078125/8388607);break;
    }
    return value;
}

void ADS1256_Start_Continuous_Read(unsigned char channel1 , unsigned char channel2)
{
    ADS1256_CS_LOW();
    ads1256_spi_write_byte(WAKEUP);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(WREG | MUX);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(0x00);
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte( (channel1 << 4) | channel2 );
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(RDATAC);
    ADS1256_CS_HIGH();
}

float ADS1256_Continuous_Read()
{
    float value;
    long value_data;
    ADS1256_CS_LOW();
    value_data = ADS1256_Rec_data();
    ADS1256_CS_HIGH();
    if(value_data > 0x7FFFFF) value_data -= 0x1000000;
    switch(scale)
    {
        case PGA1_5V:           value = (float)value_data * ((float)5/8388606);break;
        case PGA2_2_5V:         value = (float)value_data * ((float)2.5/8388607);break;
        case PGA4_1_25V:        value = (float)value_data * ((float)1.25/8388607);break;
        case PGA8_625MV:        value = (float)value_data * ((float)0.625/8388607);break;
        case PGA16_312_5MV:     value = (float)value_data * ((float)0.3125/8388607);break;
        case PGA32_156_25MV:    value = (float)value_data * ((float)0.15625/8388607);break;
        case PGA64_78_125MV:    value = (float)value_data * ((float)0.078125/8388607);break;
    }
    return value;
}

void ADS1256_Stop_Continuous_Read()
{
    ADS1256_CS_LOW();
    while(ADS1256_DRDY_GET());
    ads1256_spi_write_byte(SDATAC);
    ADS1256_CS_HIGH();
}





void ADS1256_IO_Config(unsigned char IO_num , unsigned char IO_DIR)
{
    IO_BUF |= IO_DIR << (4+IO_num) ;
    ads1256_spi_write_byte(WREG | IO);
    ads1256_spi_write_byte(0x00);
    ads1256_spi_write_byte(IO_BUF);
}

void ADS1256_IO_Set(unsigned char IO_num , unsigned char IO_OUT)
{
    IO_BUF |= IO_OUT << (IO_num) ;
    ads1256_spi_write_byte(WREG | IO);
    ads1256_spi_write_byte(0x00);
    ads1256_spi_write_byte(IO_BUF);
}

unsigned char ADS1256_IO_Read(unsigned char IO_num , unsigned char IO_IN)
{
    unsigned char IO_READ_BUF;
    ads1256_spi_write_byte(RREG | IO);
    ads1256_spi_write_byte(0x00);
    IO_BUF = ads1256_spi_read_byte();
    IO_READ_BUF = (IO_BUF & (1 << IO_num) );
    return IO_READ_BUF;
}


#ifdef HARD_SPI

#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_ISR(void)
{
    switch(__even_in_range(UCB1IV,4))
    {
        case 0:break; // Vector 0 - no interrupt
        case 2: // Vector 2 - RXIFG
        {
            RXflag = 1;
            break;
        }
        case 4:// Vector 4 - TXIFG
        {
            TXflag = 1;
            break;
        }
        default: break;
    }
}

#endif
