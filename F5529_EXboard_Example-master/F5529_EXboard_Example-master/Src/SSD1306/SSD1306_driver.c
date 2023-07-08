/*
 *  Ӳ��SPI����
 *  USCIB0
 *  MOSI----P1.7
 *  MISO----P1.6
 *  CLK-----P1.5
 *  CS------P2.5
 *  DC------P2.4
 *  RST-----P2.3
 *
 */

#include <msp430.h>
#include "SSD1306/SSD1306_driver.h"
#include "SSD1306/SSD1306_api.h"

/*
 * Ŀǰ��ʾ��������ʹ�õ������ģ��SPI�ķ�ʽ��������
 * ��Ϊ��չ�װ�Ŀǰ����ʾ�������߲���Ӳ��SPI��Ӧ��GPIO
 * SCK  ->  P2.2
 * MOSI ->  P2.3
 * CS   ->  P2.6
 * RES  ->  P2.4
 * DC   ->  P2.5
 * ע�⣺SSD1306��SSD1351����ͬʱʹ��
 */

void SSD1306_port_init()
{
    SSD1306_MOSI_DIR_OUT();
    SSD1306_CLK_DIR_OUT();
    SSD1306_CS_DIR_OUT();
    SSD1306_DC_DIR_OUT();
    SSD1306_RST_DIR_OUT();
}

void SSD1306_write_byte(unsigned char byte)
{
    int i;
    for(i=0;i<8;i++)
    {
        SSD1306_CLK_LOW();
        if(byte & 0x80)
            SSD1306_MOSI_HIGH();
        else
            SSD1306_MOSI_LOW();
        byte<<=1;
        SSD1306_CLK_HIGH();
    }
    SSD1306_CLK_LOW();
}

void SSD1306_write_cmd(unsigned char cmd)
{
    SSD1306_DC_LOW();
    SSD1306_CS_LOW();
    SSD1306_write_byte(cmd);
    SSD1306_CS_HIGH();
    SSD1306_DC_HIGH();
}

void SSD1306_write_data(unsigned char dat)
{
    SSD1306_DC_HIGH();
    SSD1306_CS_LOW();
    SSD1306_write_byte(dat);
    SSD1306_CS_HIGH();
    SSD1306_DC_HIGH();
}
/*
 * ���ã���Ļ��ɫ��ʾ�����׺��֣�
 * ������i -> Ĭ��0 ��ɫ1
*/
void SSD1306_color_turn(unsigned i)
{
    if(i==0)
        SSD1306_write_cmd(0xA6);    //������ʾ
    if(i==1)
        SSD1306_write_cmd(0xA7);    //��ɫ��ʾ
}

/*
 * ���ã������Գ���Ϊ��׼��ת�����ֺ�λ�ö���ת���ʺϷ������oled��ʱ��ʹ��
 * ������i -> Ĭ��0 ��ת1
 * */
void SSD1306_display_turn(unsigned i)
{
    if(i==0)
    {
        SSD1306_write_cmd(0xC8);
        SSD1306_write_cmd(0xA1);    //������ʾ
    }
    if(i==1)
    {
        SSD1306_write_cmd(0xC0);
        SSD1306_write_cmd(0xA0);    //��ת��ʾ
    }
}

void SSD1306_set_pos(unsigned char x, unsigned char y)
{
    SSD1306_write_cmd(0xb0+y);
    SSD1306_write_cmd(((x&0xf0)>>4)|0x10);
    SSD1306_write_cmd((x&0x0f));
}
/*
 * ���ã�����
 * ��������
 * */
void SSD1306_clear()
{
    unsigned char i,n;
    for(i=0;i<8;i++)
    {
        SSD1306_write_cmd(0xb0+i);    //����ҳ��ַ��0~7��
        SSD1306_write_cmd(0x00);      //������ʾλ�á��е͵�ַ
        SSD1306_write_cmd(0x10);      //������ʾλ�á��иߵ�ַ
        for(n=0;n<128;n++)SSD1306_write_data(0);
    } //������ʾ
}

/*
 * ���ã�ֻ��������
 * ������sety -->��ʾ��
 * */
void SSD1306_clearline()
{
    unsigned char i,n;
        for(i=0;i<2;i++)
        {
            SSD1306_write_cmd(0xb0+i);    //����ҳ��ַ��0~7��
            SSD1306_write_cmd(0x00);      //������ʾλ�á��е͵�ַ
            SSD1306_write_cmd(0x10);      //������ʾλ�á��иߵ�ַ
            for(n=0;n<128;n++)SSD1306_write_data(0);
        } //������ʾ
}
/*
 * ���ã�SSD1306��ʼ��
 * ��������
 */
void SSD1306_init()
{
    SSD1306_port_init();

    SSD1306_RST_LOW();
    _delay_cycles(1600000);
    SSD1306_RST_HIGH();

    SSD1306_write_cmd(0xAE);//--turn off oled panel
    SSD1306_write_cmd(0x00);//---set low column address
    SSD1306_write_cmd(0x10);//---set high column address
    SSD1306_write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    SSD1306_write_cmd(0x81);//--set contrast control register
    SSD1306_write_cmd(0xCF); // Set SEG Output Current Brightness
    SSD1306_write_cmd(0xA1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
    SSD1306_write_cmd(0xC8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
    SSD1306_write_cmd(0xA6);//--set normal display
    SSD1306_write_cmd(0xA8);//--set multiplex ratio(1 to 64)
    SSD1306_write_cmd(0x3f);//--1/64 duty
    SSD1306_write_cmd(0xD3);//-set display offset   Shift Mapping RAM Counter (0x00~0x3F)
    SSD1306_write_cmd(0x00);//-not offset
    SSD1306_write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
    SSD1306_write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    SSD1306_write_cmd(0xD9);//--set pre-charge period
    SSD1306_write_cmd(0xF1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    SSD1306_write_cmd(0xDA);//--set com pins hardware configuration
    SSD1306_write_cmd(0x12);
    SSD1306_write_cmd(0xDB);//--set vcomh
    SSD1306_write_cmd(0x40);//Set VCOM Deselect Level
    SSD1306_write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    SSD1306_write_cmd(0x02);//
    SSD1306_write_cmd(0x8D);//--set Charge Pump enable/disable
    SSD1306_write_cmd(0x14);//--set(0x10) disable
    SSD1306_write_cmd(0xA4);// Disable Entire Display On (0xa4/0xa5)
    SSD1306_write_cmd(0xA6);// Disable Inverse Display On (0xa6/a7)
    SSD1306_clear();
    SSD1306_write_cmd(0xAF); /*display ON*/
}
