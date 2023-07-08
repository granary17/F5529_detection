/*
 * SSD1351_api.h
 *
 *  Created on: 2021.7.22
 *      Author: Zuoge
 */

/*
 * 2022.5.15:
 * 在需要使用SSD1351显示屏的程序只需要include此头文件即可
 * #include "SSD1351/SSD1351_api.h"
 * 注意：SSD1306与SSD1351不可同时使用
 */

#ifndef SRC_SSD1351_API_H_
#define SRC_SSD1351_API_H_

#define SSD1351_W 128
#define SSD1351_H 128

#define HORIZONTAL
//#define VERTICAL

#include "SSD1351/SSD1351_font.h"

#define WHITE            0xFFFF
#define BLACK            0x0000
#define BLUE             0x001F
#define BRED             0XF81F
#define GRED                   0XFFE0
#define GBLUE                  0X07FF
#define RED              0xF800
#define MAGENTA          0xF81F
#define GREEN            0x07E0
#define CYAN             0x7FFF
#define YELLOW           0xFFE0
#define BROWN                0XBC40 //棕色
#define BRRED                0XFC07 //棕红色
#define GRAY                 0X8430 //灰色
#define DARKBLUE         0X01CF //深蓝色
#define LIGHTBLUE        0X7D7C //浅蓝色
#define GRAYBLUE         0X5458 //灰蓝色
#define LIGHTGREEN       0X841F //浅绿色
#define LGRAY                0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

void SSD1351_spi_write(unsigned char dat);
void SSD1351_spi_port_init();
void SSD1351_write_cmd(unsigned char cmd);
void SSD1351_write_data_8(unsigned char data);
void SSD1351_write_data_16(unsigned int data);
void SSD1351_Address_Set(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2);
void SSD1351_init();

void SSD1351_Clear(unsigned int Color);
void SSD1351_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color);
void SSD1351_DrawPoint(unsigned int x,unsigned int y,unsigned int color);
void SSD1351_DrawPoint_big(unsigned int x,unsigned int y,unsigned int color);
void SSD1351_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color);
void SSD1351_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color);
void SSD1351_Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r,unsigned int color);
void SSD1351_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
void SSD1351_ShowString(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
unsigned long SSD1351_mypow(unsigned char m,unsigned char n);
void SSD1351_ShowIntNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);
void SSD1351_ShowFloatNum1(unsigned int x,unsigned int y,float num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey);
void SSD1351_ShowPicture(unsigned int x,unsigned int y,unsigned int length,unsigned int width,const unsigned char pic[]);
void SSD1351_ShowChinese12x12(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
void SSD1351_ShowChinese16x16(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
void SSD1351_ShowChinese24x24(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
void SSD1351_ShowChinese32x32(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
void SSD1351_ShowChineseString(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);
void SSD1351_Show_String(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode);



#endif /* SRC_SSD1351_SSD1351_API_H_ */
