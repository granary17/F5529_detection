/*
 * ADS1256.h
 *
 *  Created on: 2021年6月25日
 *      Author: Zuoge
 */

#ifndef SRC_ADS1256_H_
#define SRC_ADS1256_H_

#define SOFT_SPI

#ifdef HARD_SPI

#endif

#ifdef SOFT_SPI

//#define ADS1256_SPI_MISO_DIR_IN()   {P4DIR&=~BIT2;P4REN|=BIT2;P4OUT|=BIT2;}
#define ADS1256_SPI_MISO_DIR_IN()   P4DIR&=~BIT2
#define ADS1256_SPI_MISO_GET()      P4IN&BIT2
#define ADS1256_SPI_MOSI_DIR_OUT()  P4DIR|=BIT1
#define ADS1256_SPI_MOSI_HIGH()     P4OUT|=BIT1
#define ADS1256_SPI_MOSI_LOW()      P4OUT&=~BIT1
#define ADS1256_SPI_SCK_DIR_OUT()   P4DIR|=BIT3
#define ADS1256_SPI_SCK_HIGH()      P4OUT|=BIT3
#define ADS1256_SPI_SCK_LOW()       P4OUT&=~BIT3

unsigned char ads1256_spi_read_byte();
void ads1256_spi_write_byte(unsigned char dat);
void ADS1256_port_init();

#endif

#define ADS1256_RST_DIR_OUT()   P2DIR|=BIT0
#define ADS1256_RST_HIGH()      P2OUT|=BIT0
#define ADS1256_RST_LOW()       P2OUT&=~BIT0
#define ADS1256_CS_DIR_OUT()    P4DIR|=BIT0
#define ADS1256_CS_HIGH()       P4OUT|=BIT0
#define ADS1256_CS_LOW()        P4OUT&=~BIT0
#define ADS1256_DRDY_DIR_IN()   {P1DIR&=~BIT6;P1REN|=BIT6;P1OUT|=BIT6;}
#define ADS1256_DRDY_GET()      P1IN&BIT6

/********************寄存器列表********************/
#define STATUS  0x00
#define MUX     0x01
#define ADCON   0x02
#define DRATE   0x03
#define IO      0x04
#define OFC0    0x05
#define OFC1    0x06
#define OFC2    0x07
#define FSC0    0x08
#define FSC1    0x09
#define FSC2    0x0A

/********************命令列表********************/
#define WAKEUP      0x00
#define RDATA       0x01
#define RDATAC      0x03
#define SDATAC      0x0F
#define RREG        0x10
#define WREG        0x50
#define SELFCAL     0xF0
#define SELFOCAL    0xF1
#define SELFGCAL    0xF2
#define SYSOCAL     0xF3
#define SYSGCAL     0xF4
#define SYNC        0xFC
#define STANDBY     0xFD
#define RESET       0xFE
#define WAKEup      0xFF    //效果貌似等同于WAKEUP

/********************数据速率及数据平均数量列表********************/
#define AVE_1_DRATE_30K     0xF0
#define AVE_2_DRATE_15K     0xE0
#define AVE_4_DRATE_7_5K    0xD0
#define AVE_8_DRATE_3_7_5K  0xC0
#define AVE_15_DRATE_2K     0xB0
#define AVE_30_DRATE_1K     0xA1
#define AVE_60_DRATE_500    0x92
#define AVE_300_DRATE_100   0x82
#define AVE_500_DRATE_60    0x72
#define AVE_600_DRATE_50    0x63
#define AVE_1K_DRATE_30     0x53
#define AVE_1_2K_DRATE_25   0x43
#define AVE_2K_DRATE_15     0x33
#define AVE_3K_DRATE_10     0x23
#define AVE_6K_DRATE_5      0x13
#define AVE_12K_DRATE_2_5   0x03

/********************PGA调整列表********************/
#define PGA1_5V             0x00
#define PGA2_2_5V           0x01
#define PGA4_1_25V          0x02
#define PGA8_625MV          0x03
#define PGA16_312_5MV       0x04
#define PGA32_156_25MV      0x05
#define PGA64_78_125MV      0x06

/********************通道列表********************/
#define AIN0    0x00
#define AIN1    0x01
#define AIN2    0x02
#define AIN3    0x03
#define AIN4    0x04
#define AIN5    0x05
#define AIN6    0x06
#define AIN7    0x07
#define AINCOM  0x08

void ADS1256_init();
void ADS1256_Scale_Config(unsigned char scale_buf);
void ADS1256_Rate_Config(unsigned char rate_buf);
float ADS1256_Single_Read(unsigned char channel1 , unsigned char channel2);// , unsigned char scale);
void ADS1256_Start_Continuous_Read(unsigned char channel1 , unsigned char channel2);
float ADS1256_Continuous_Read();
void ADS1256_Stop_Continuous_Read();

#endif /* SRC_ADS1256_H_ */
