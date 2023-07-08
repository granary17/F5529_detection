/*
 * INA260.h
 *
 *  Created on: 2021.7.31
 *      Author: HASEE
 */

#ifndef SRC_INA_H_
#define SRC_INA_H_



#define SCL_H P8OUT |= BIT1 //IIC
#define SDA_H P8OUT |=BIT2 ;  //IIC
#define SCL_L P8OUT &= ~BIT1 ; //IIC
#define SDA_L P8OUT &= ~BIT2 ;  //IIC
#define SDA_DIRIN  P8DIR &= ~BIT2
#define SDA_DIROUT P8DIR |= BIT2
#define SDA_IN  P8IN&BIT2


#define CPU_F ((double)16000000)
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define Delay_Nms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

void I2C_START(void);
void I2C_OVER( void );
void I2C_STOP(void);
unsigned char I2C_TR(unsigned char dat );
unsigned char I2C_RX();
void INA_port_init(void);
unsigned int INA_read_data();
unsigned char INA_write_addr(unsigned char addr);
unsigned char INA_write_data(unsigned char addr , unsigned int dat);
unsigned char INA_clear_alert();

//选择是INA260还是INA226
#define INA260
//##define INA226

#ifdef  INA260

///////////////////////?????///////////////////
#define CONFIG_REG              0x00
#define CURRENT_REG             0x01
#define BUS_VOLT_REG            0x02
#define POWER_REG               0x03
#define MASK_ENABLE_REG         0x06
#define ALERT_LIMIT_REG         0x07
#define MANUFACTURER_ID_REG     0xFE
#define DIE_ID_REG              0xFF

/////////////////CONFIG_REG////////////////////
#define RST_BIT                 BITF

#define AVG_1                   0x0000
#define AVG_4                   0x0200
#define AVG_16                  0x0400
#define AVG_64                  0x0600
#define AVG_128                 0x0800
#define AVG_256                 0x0A00
#define AVG_542                 0x0C00
#define AVG_1024                0x0E00

#define VLOT_CONV_TIM_140_US    0x0000
#define VLOT_CONV_TIM_204_US    0x0040
#define VLOT_CONV_TIM_332_US    0x0080
#define VLOT_CONV_TIM_588_US    0x00C0
#define VLOT_CONV_TIM_1_1_MS    0x0100
#define VLOT_CONV_TIM_2_116_MS  0x0140
#define VLOT_CONV_TIM_4_156_MS  0x0180
#define VLOT_CONV_TIM_8_224_MS  0x01C0

#define CURR_CONV_TIM_140_US    0x0000
#define CURR_CONV_TIM_204_US    0x0008
#define CURR_CONV_TIM_332_US    0x0010
#define CURR_CONV_TIM_588_US    0x0018
#define CURR_CONV_TIM_1_1_MS    0x0020
#define CURR_CONV_TIM_2_116_MS  0x0028
#define CURR_CONV_TIM_4_156_MS  0x0030
#define CURR_CONV_TIM_8_224_MS  0x0038

#define MODE_POWER_DOWN_1       0x0000
#define MODE_TRGGERED_CURR      0x0001
#define MODE_TRGGERED_VLOT      0x0002
#define MODE_TRGGERED_CURR_VLOT 0x0003
#define MODE_POWER_DOWN_2       0x0004
#define MODE_CONU_CURR          0x0005
#define MODE_CONU_VLOT          0x0006
#define MODE_CONU_CURR_VLOT     0x0007

/////////////////MASK_ENABLE_REG///////////////
#define OCL                     BITF
#define UCL                     BITE
#define BOL                     BITD
#define BUL                     BITC
#define POL                     BITB
#define CNVR                    BITA
#define AFF                     BIT4
#define CVRF                    BIT3
#define OVF                     BIT2
#define APOL                    BIT1
#define LEN                     BIT0


void INA_init();
float INA_read_vlot();
float INA_read_current();
float INA_read_power();


#endif


#ifdef INA226

///////////////////////?????///////////////////
#define CONFIG_REG              0x00
#define SHUNT_VLOT_REG          0x01
#define BUS_VOLT_REG            0x02
#define POWER_REG               0x03
#define CURRENT_REG             0x04
#define CALIBRATION_REG         0x05
#define MASK_ENABLE_REG         0x06
#define ALERT_LIMIT_REG         0x07
#define MANUFACTURER_ID_REG     0xFE
#define DIE_ID_REG              0xFF

/////////////////CONFIG_REG////////////////////
#define RST_BIT                 BITF

#define AVG_1                   0x0000
#define AVG_4                   0x0200
#define AVG_16                  0x0400
#define AVG_64                  0x0600
#define AVG_128                 0x0800
#define AVG_256                 0x0A00
#define AVG_542                 0x0C00
#define AVG_1024                0x0E00

#define BUS_VLOT_CONV_140_US    0x0000
#define BUS_VLOT_CONV_204_US    0x0040
#define BUS_VLOT_CONV_332_US    0x0080
#define BUS_VLOT_CONV_588_US    0x00C0
#define BUS_VLOT_CONV_1_1_MS    0x0100
#define BUS_VLOT_CONV_2_116_MS  0x0140
#define BUS_VLOT_CONV_4_156_MS  0x0180
#define BUS_VLOT_CONV_8_224_MS  0x01C0

#define SHUNT_VLOT_CONV_140_US      0x0000
#define SHUNT_VLOT_CONV_204_US      0x0008
#define SHUNT_VLOT_CONV_332_US      0x0010
#define SHUNT_VLOT_CONV_588_US      0x0018
#define SHUNT_VLOT_CONV_1_1_MS      0x0020
#define SHUNT_VLOT_CONV_2_116_MS    0x0028
#define SHUNT_VLOT_CONV_4_156_MS    0x0030
#define SHUNT_VLOT_CONV_8_224_MS    0x0038

#define MODE_POWER_DOWN_1       0x0000
#define MODE_TRGGERED_SHUNT     0x0001
#define MODE_TRGGERED_BUS       0x0002
#define MODE_TRGGERED_SHUNT_BUS 0x0003
#define MODE_POWER_DOWN_2       0x0004
#define MODE_CONU_SHUNT         0x0005
#define MODE_CONU_BUS           0x0006
#define MODE_CONU_SHUNT_BUS     0x0007

void INA_init();
float INA_read_vlot();
float INA_read_shunt_vlot();
float INA_read_current();
float INA_read_power();

/////////////////MASK_ENABLE_REG///////////////
#define SOL                     BITF
#define SUL                     BITE
#define BOL                     BITD
#define BUL                     BITC
#define POL                     BITB
#define CNVR                    BITA
#define AFF                     BIT4
#define CVRF                    BIT3
#define OVF                     BIT2
#define APOL                    BIT1
#define LEN                     BIT0


#endif

#endif /* SRC_INA260_H_ */
