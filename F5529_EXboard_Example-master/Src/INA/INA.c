/*
 * INA260.c
 *
 *  Created on: 2021.7.31
 *      Author: HASEE
 */

#include <msp430.h>
#include "INA.h"

#define DEVICE_ADDRESS 0x40

void INA_port_init(void)
{
    P8DIR |=BIT1+BIT2;//P8_2 SDA   P8_1  SCL
}

void I2C_START(void)
{
    SCL_H;                   //SCL
    delay_us( 200);
    SDA_H;
    delay_us( 200);
    SDA_L;                    //SCL
    delay_us( 200);
 }

void I2C_OVER( void )
{
    SCL_L;
    SDA_H ;
    delay_us(400);
}

void I2C_STOP(void)
{
    SDA_DIROUT;
    SDA_L;
    SCL_L;
    delay_us( 100);
    SCL_H;
    delay_us( 100);
    SDA_H;
    delay_us(100);
}

unsigned char I2C_TR(unsigned char dat )
{
    unsigned char ack=0;
    int i;
    SDA_DIROUT;
    SCL_L;
    for (i=8; i>0; i--)
    {
        if (dat & 0x80)
        {
            SDA_H;
        }
        else
        {
            SDA_L;
        }
        dat <<= 1;

        SCL_H;

        SCL_L;
    }
    SCL_L;
    SDA_DIRIN;
    delay_us( 200);
    ack = SDA_IN;
    delay_us( 200);
    SCL_H;
    delay_us( 200);
    return ack;
}

unsigned char I2C_RX()
{
    unsigned char dat = 0;
    int i;
    SDA_DIRIN;
    for(i = 0 ; i < 8 ; i++)
    {
        SCL_H;
        dat <<= 1;
        if(SDA_IN)
        {
            dat |= BIT0;
        }
        SCL_L;
    }

    SDA_DIROUT;
    SDA_L

    SCL_H;

    SCL_L;


    SDA_DIRIN;

    return dat;
}

unsigned int INA_read_data()
{
    unsigned int dat = 0;
    unsigned char ack = 0;

    I2C_START();

    ack = I2C_TR((DEVICE_ADDRESS << 1)+1); //���͵�ַ
    if( ack != 0 )
    {
        return ack;
    }
    I2C_OVER();

    dat = I2C_RX();
    dat <<= 8;
    dat = dat + I2C_RX();

    I2C_STOP();

    return dat;
}

unsigned char INA_write_addr(unsigned char addr)
{
    unsigned char ack = 0;

    I2C_START();

    ack = I2C_TR(DEVICE_ADDRESS << 1); //���͵�ַ
    if( ack != 0 )
    {
        return ack;
    }
    I2C_OVER();

    ack = I2C_TR(addr);
    if( ack != 0 )
    {
        return ack;
    }
    I2C_STOP();

    return 0;
}

unsigned char INA_write_data(unsigned char addr , unsigned int dat)
{
    unsigned char ack = 0;

    I2C_START();

    ack = I2C_TR(DEVICE_ADDRESS << 1); //���͵�ַ
    if( ack != 0 )
    {
        return ack;
    }
    I2C_OVER();

    ack = I2C_TR(addr);
    if( ack != 0 )
    {
        return ack;
    }
    I2C_OVER();

    ack = I2C_TR(dat>>8);
    if( ack != 0 )
    {
        return ack;
    }
    I2C_OVER();

    ack = I2C_TR(dat & 0xFF);
    if( ack != 0 )
    {
        return ack;
    }

    I2C_STOP();

    return 0;
}

//INA���ALERT���ŵľ���
//���ش���������оƬ�ĵ�ַ
unsigned char INA_clear_alert()
{
    unsigned char device_addr;
    unsigned char ack = 0;

    I2C_START();

    ack = I2C_TR( (0x0C << 1) + 1);
    if( ack != 0 )
    {
        return 0;
    }
    I2C_OVER();

    device_addr = I2C_RX();

    I2C_STOP();

    return device_addr;
}


#ifdef INA260
///////////////////ʹ�õ���INA260оƬ
/*
 * оƬ��ʼ��
 *
 */
void INA_init()
{
    INA_port_init();
    //INA_write_data(CONFIG_REG , 0x6127);        //���üĴ�����һ��Ĭ�Ͼ���
}

/*
 * ��ȡ��ѹֵ����С����1.25mv
 * ���أ�float�͵�ѹֵ
 */
float INA_read_vlot()
{
    unsigned int vlot_bit;
    float vlot;
    INA_write_addr(BUS_VOLT_REG);
    vlot_bit = INA_read_data();
    if(vlot_bit > 0x7FFF) vlot_bit -= 0x10000;
    vlot = (float)(vlot_bit * 0.00125);
    return vlot;
}

/*
 * ��ȡ����ֵ����С����1.25ma����ȡ��������Ϊ����
 * ���أ�float�͵���ֵ
 */
float INA_read_current()
{
    unsigned int current_bit;
    float current = 0;
    INA_write_addr(CURRENT_REG);
    current_bit = INA_read_data();
    if(current_bit > 0x7FFF)
        current_bit = ~current_bit;
    current = (float)(current_bit * 0.00125);
    return current;
}

/*
 * ��ȡ����ֵ����С����10mw
 * ���أ�float�͹���ֵ
 */
float INA_read_power()
{
    unsigned int power_bit;
    float power;
    INA_write_addr(POWER_REG);
    power_bit = INA_read_data();
    //if(power_bit > 0x7FFF) power_bit -= 0x10000;
    power = (float)(power_bit * 0.01);
    return power;
}

#endif



#ifdef INA226
///////////////////ʹ�õ���INA226оƬ
/*
 * INA226��ʼ��
 */
void INA_init()
{
    INA_port_init();
    //INA_write_data(CONFIG_REG , 0x4127);        //���üĴ�����һ��Ĭ�Ͼ���
    INA_write_data(CALIBRATION_REG , 0x0200);       //У׼�Ĵ������ã�����(0.00512/(���ص���*��С��������))�ó�
                                                    //0x0200Ϊ(0.00512/(10mR*1mA))�ó�
}

/*
 * ��ȡ��ѹֵ����С����1.25mv
 * ���أ�float�͵�ѹֵ
 */
float INA_read_vlot()
{
    unsigned int vlot_bit;
    float vlot;
    INA_write_addr(BUS_VOLT_REG);
    vlot_bit = INA_read_data();
    if(vlot_bit > 0x7FFF) vlot_bit -= 0x10000;
    vlot = (float)(vlot_bit * 0.00125);
    return vlot;
}

/*
 * ��ȡ��ѹ��ѹֵ����С����2.5uv
 * ���أ�float�͵�ѹֵ����λmv
 */
float INA_read_shunt_vlot()     //��λmv
{
    unsigned int shunt_vlot_bit;
    float shunt_vlot;
    INA_write_addr(SHUNT_VLOT_REG);
    shunt_vlot_bit = INA_read_data();
    if(shunt_vlot_bit > 0x7FFF) shunt_vlot_bit -= 0x10000;
    shunt_vlot = (float)(shunt_vlot_bit * 0.0025);
    return shunt_vlot;
}

/*
 * ��ȡ����ֵ����С����1ma
 * ���أ�float�͵���ֵ
 */
float INA_read_current()
{
    unsigned int current_bit;
    float current = 0;
    INA_write_addr(CURRENT_REG);
    current_bit = INA_read_data();

    //current_bit = ~current_bit;

    if(current_bit > 0x7FFF) current_bit -= 0x10000;
    current = (float)(current_bit * 0.001);
    return current;
}

/*
 * ��ȡ����ֵ����С����25mw
 * ���أ�float�͹���ֵ
 */
float INA_read_power()
{
    unsigned int power_bit;
    float power;
    INA_write_addr(POWER_REG);
    power_bit = INA_read_data();
    //if(power_bit > 0x7FFF) power_bit -= 0x10000;
    power = (float)(power_bit * 0.025);     //������Сֵ�̶�Ϊ������Сֵ��25��
    return power;
}

#endif
