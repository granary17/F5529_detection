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

    ack = I2C_TR((DEVICE_ADDRESS << 1)+1); //发送地址
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

    ack = I2C_TR(DEVICE_ADDRESS << 1); //发送地址
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

    ack = I2C_TR(DEVICE_ADDRESS << 1); //发送地址
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

//INA清除ALERT引脚的警报
//返回触发警报的芯片的地址
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
///////////////////使用的是INA260芯片
/*
 * 芯片初始化
 *
 */
void INA_init()
{
    INA_port_init();
    //INA_write_data(CONFIG_REG , 0x6127);        //配置寄存器，一般默认就行
}

/*
 * 读取电压值，最小精度1.25mv
 * 返回：float型电压值
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
 * 读取电流值，最小精度1.25ma，读取到的数据为补码
 * 返回：float型电流值
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
 * 读取功率值，最小精度10mw
 * 返回：float型功率值
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
///////////////////使用的是INA226芯片
/*
 * INA226初始化
 */
void INA_init()
{
    INA_port_init();
    //INA_write_data(CONFIG_REG , 0x4127);        //配置寄存器，一般默认就行
    INA_write_data(CALIBRATION_REG , 0x0200);       //校准寄存器设置，根据(0.00512/(负载电阻*最小测量电流))得出
                                                    //0x0200为(0.00512/(10mR*1mA))得出
}

/*
 * 读取电压值，最小精度1.25mv
 * 返回：float型电压值
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
 * 读取分压电压值，最小精度2.5uv
 * 返回：float型电压值，单位mv
 */
float INA_read_shunt_vlot()     //单位mv
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
 * 读取电流值，最小精度1ma
 * 返回：float型电流值
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
 * 读取功率值，最小精度25mw
 * 返回：float型功率值
 */
float INA_read_power()
{
    unsigned int power_bit;
    float power;
    INA_write_addr(POWER_REG);
    power_bit = INA_read_data();
    //if(power_bit > 0x7FFF) power_bit -= 0x10000;
    power = (float)(power_bit * 0.025);     //功率最小值固定为电流最小值的25倍
    return power;
}

#endif
