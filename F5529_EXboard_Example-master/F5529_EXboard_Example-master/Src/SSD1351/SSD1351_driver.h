/*
 * SSD1351.h
 *
 *  Created on: 2021.7.22
 *      Author: Zuoge
 */

#ifndef SRC_SSD1351_DRIVER_H_
#define SRC_SSD1351_DRIVER_H_

#define SSD1351_W 128
#define SSD1351_H 128



#define MOSI_DIR_OUT()  P2DIR|=BIT3
#define MOSI_HIGH()     P2OUT|=BIT3
#define MOSI_LOW()      P2OUT&=~BIT3
#define SCK_DIR_OUT()   P2DIR|=BIT2
#define SCK_HIGH()      P2OUT|=BIT2
#define SCK_LOW()       P2OUT&=~BIT2
#define CS_DIR_OUT()    P2DIR|=BIT6
#define CS_HIGH()       P2OUT|=BIT6
#define CS_LOW()        P2OUT&=~BIT6
#define RES_DIR_OUT()   P2DIR|=BIT4
#define RES_HIGH()      P2OUT|=BIT4
#define RES_LOW()       P2OUT&=~BIT4
#define DC_DIR_OUT()    P2DIR|=BIT5
#define DC_HIGH()       P2OUT|=BIT5
#define DC_LOW()        P2OUT&=~BIT5

void SSD1351_spi_write(unsigned char dat);
void SSD1351_spi_port_init();
void SSD1351_write_cmd(unsigned char cmd);
void SSD1351_write_data_8(unsigned char data);
void SSD1351_write_data_16(unsigned int data);
void SSD1351_Address_Set(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
void SSD1351_init();

//#define HARDSPI
//#define SOFTSPI

#endif /* SRC_SSD1351_SSD1351_DRIVER_H_ */
