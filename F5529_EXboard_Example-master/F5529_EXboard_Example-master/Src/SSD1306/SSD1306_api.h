/*
 * SSD1306_api.h
 *
 *  Created on: 2021年8月18日
 *      Author: Zuoge
 */

/*
 * 2022.5.15:
 * 在需要使用SSD1306显示屏的程序只需要include此头文件即可
 * #include "SSD1306/SSD1306_api.h"
 * 注意：SSD1306与SSD1351不可同时使用
 */

#ifndef SRC_SSD1306_SSD1306_API_H_
#define SRC_SSD1306_SSD1306_API_H_

#define SSD1306_W 128
#define SSD1306_H 64

#include "SSD1306/Font_code.h"

void SSD1306_write_cmd(unsigned char cmd);
void SSD1306_write_data(unsigned char dat);
void SSD1306_color_turn(unsigned i);
void SSD1306_display_turn(unsigned i);
void SSD1306_set_pos(unsigned char x, unsigned char y);
void SSD1306_clear();
void SSD1306_clearline();
void SSD1306_init();

void SSD1306_ShowChar(unsigned char x,unsigned char y, char chr,unsigned char sizey);
unsigned long SSD1306_pow(unsigned char m,unsigned char n);
void SSD1306_ShowNum(unsigned char x,unsigned char y,unsigned long num,unsigned char len,unsigned char sizey);
void SSD1306_ShowString(unsigned char x,unsigned char y, char *chr,unsigned char sizey);

#endif /* SRC_SSD1306_SSD1306_API_H_ */
