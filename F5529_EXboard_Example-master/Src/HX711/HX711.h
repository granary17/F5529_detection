/*
 * HX711.h
 *
 *  Created on: 2023年7月2日
 *      Author: 37756
 */

#ifndef SRC_HX711_H_
#define SRC_HX711_H_

extern void init_hx711port(void);                                        /*初始化HX711端口  3.3V供电 4.0接SCK 4.1接DT */
extern unsigned long read_hx711_24bit(void);                 /* 读取HX711内部的数据 */
extern void  get_maopi(void);                                               /* 获取毛皮重量,上电初始化时候执行*/
extern float get_weight(void);                            /* 获取重量 返回g单位的重量 */
extern void  instr_num(void);   /*判定材料数量*/

#endif /* SRC_HX711_H_ */

