/*
 * SSD1351_driver.c
 *
 *  Created on: 2021.7.22
 *      Author: Zuoge
 */
#include "msp430.h"
#include "SSD1351_driver.h"
#include "SSD1351_api.h"

/*
 * 目前显示屏的驱动使用的是软件模拟SPI的方式进行驱动
 * 因为扩展底板目前给显示屏的连线不是硬件SPI对应的GPIO
 * SCK  ->  P2.2
 * MOSI ->  P2.3
 * CS   ->  P2.6
 * RES  ->  P2.4
 * DC   ->  P2.5
 * 注意：SSD1306与SSD1351不可同时使用
 */

void SSD1351_spi_write(unsigned char dat)
{
    int i;
    for(i=0;i<8;i++)
    {
        SCK_LOW();
        if(dat & 0x80)
            MOSI_HIGH();
        else
            MOSI_LOW();
        dat<<=1;
        SCK_HIGH();
    }
    SCK_LOW();
}

void SSD1351_spi_port_init()
{
    MOSI_DIR_OUT();
    SCK_DIR_OUT();
    CS_DIR_OUT();
    RES_DIR_OUT();
    DC_DIR_OUT();
}

void SSD1351_write_cmd(unsigned char cmd)
{
    DC_LOW();
    CS_LOW();
    SSD1351_spi_write(cmd);
    CS_HIGH();
    DC_HIGH();
}

void SSD1351_write_data_8(unsigned char data)
{
    DC_HIGH();
    CS_LOW();
    SSD1351_spi_write(data);
    CS_HIGH();
    DC_HIGH();
}

void SSD1351_write_data_16(unsigned int data)
{
    DC_HIGH();
    CS_LOW();
    SSD1351_spi_write(data>>8);
    SSD1351_spi_write(data);
    CS_HIGH();
    DC_HIGH();
}

void SSD1351_Address_Set(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2)
{
#ifdef VERTICAL
    SSD1351_write_cmd(0x15);//列地址设置
    SSD1351_write_data_8(x1);
    SSD1351_write_data_8(x2);
    SSD1351_write_cmd(0x75);//行地址设置
    SSD1351_write_data_8(y1);
    SSD1351_write_data_8(y2);
    SSD1351_write_cmd(0x5c);
#endif

#ifdef HORIZONTAL
    SSD1351_write_cmd(0x15);//列地址设置
    SSD1351_write_data_8(y1);
    SSD1351_write_data_8(y2);
    SSD1351_write_cmd(0x75);//行地址设置
    SSD1351_write_data_8(x1);
    SSD1351_write_data_8(x2);
    SSD1351_write_cmd(0x5c);
#endif
}

void SSD1351_init()
{
    SSD1351_spi_port_init();

    RES_LOW();
    //delay_ms(200);
    RES_HIGH();

    SSD1351_write_cmd(0xFD);
    SSD1351_write_data_8(0x12);
    SSD1351_write_cmd(0xFD);
    SSD1351_write_data_8(0xB1);
    SSD1351_write_cmd(0xAE);
    SSD1351_write_cmd(0xB3);
    SSD1351_write_data_8(0xF1);
    SSD1351_write_cmd(0xCA);
    SSD1351_write_data_8(0x7F);
    SSD1351_write_cmd(0xA2);
    SSD1351_write_data_8(0x00);
    SSD1351_write_cmd(0xA1);
    SSD1351_write_data_8(0x00);
    SSD1351_write_cmd(0xA0);
#ifdef VERTICAL
    SSD1351_write_data_8(0x74);
#endif
#ifdef HORIZONTAL
    SSD1351_write_data_8(0x64 | 0x01);
#endif
    SSD1351_write_cmd(0xB5);
    SSD1351_write_data_8(0x00);
    SSD1351_write_cmd(0xAB);
    SSD1351_write_data_8(0x01);
    SSD1351_write_cmd(0xB4);
    SSD1351_write_data_8(0xA0);
    SSD1351_write_data_8(0xB5);
    SSD1351_write_data_8(0x55);
    SSD1351_write_cmd(0xC1);
    SSD1351_write_data_8(0xC8);
    SSD1351_write_data_8(0x80);
    SSD1351_write_data_8(0xC8);
    SSD1351_write_cmd(0xC7);
    SSD1351_write_data_8(0x0F);
    SSD1351_write_cmd(0xB1);
    SSD1351_write_data_8(0x32);
    SSD1351_write_cmd(0xBB);
    SSD1351_write_data_8(0x17);
    SSD1351_write_cmd(0xB2);
    SSD1351_write_data_8(0xA4);
    SSD1351_write_data_8(0x00);
    SSD1351_write_data_8(0x00);
    SSD1351_write_cmd(0xB6);
    SSD1351_write_data_8(0x01);
    SSD1351_write_cmd(0xBE);
    SSD1351_write_data_8(0x05);
    SSD1351_write_cmd(0xA6);
    SSD1351_write_cmd(0xAF);
}
