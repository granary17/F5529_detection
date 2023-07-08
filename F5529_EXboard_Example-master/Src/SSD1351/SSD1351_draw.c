/*
 * SSD1351_draw.c
 *
 *  Created on: 2021.7.22
 *      Author: Zuoge
 */

#include "SSD1351_driver.h"
#include "SSD1351_api.h"
#include "SSD1351_draw.h"
#include "SSD1351_font.h"

#define CNHZNUM 27       //字库汉字数量

void SSD1351_Clear(unsigned int Color)
{
    unsigned int i,j;
    SSD1351_Address_Set(0,0,SSD1351_W-1,SSD1351_H-1);
    for(i=0;i<SSD1351_H;i++)
    {
        for (j=0;j<SSD1351_W;j++)
        {
            SSD1351_write_data_16(Color);
        }
    }
}

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
      返回值：  无
******************************************************************************/
void SSD1351_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{
    unsigned int i,j;
    SSD1351_Address_Set(xsta,ysta,xend,yend);      //设置光标位置
    for(i=ysta;i<=yend;i++)
    {
        for(j=xsta;j<=xend;j++)
        {
            SSD1351_write_data_16(color);//设置光标位置
        }
    }
}

/******************************************************************************
      函数说明：LCD画点
      入口数据：x,y   起始坐标
      返回值：  无
******************************************************************************/
void SSD1351_DrawPoint(unsigned int x,unsigned int y,unsigned int color)
{
    SSD1351_Address_Set(x,y,x,y);//设置光标位置
    SSD1351_write_data_16(color);
}

/******************************************************************************
      函数说明：LCD画一个大的点
      入口数据：x,y   起始坐标
      返回值：  无
******************************************************************************/
void SSD1351_DrawPoint_big(unsigned int x,unsigned int y,unsigned int color)
{
    SSD1351_Fill(x-1,y-1,x+1,y+1,color);
}

/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
      返回值：  无
******************************************************************************/
void SSD1351_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
{
    unsigned int t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;//画线起点坐标
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if (delta_x==0)incx=0;//垂直线
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//水平线
    else {incy=-1;delta_y=-delta_x;}
    if(delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0;t<distance+1;t++)
    {
        SSD1351_DrawPoint(uRow,uCol,color);//画点
        xerr+=delta_x;
        yerr+=delta_y;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}


/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
      返回值：  无
******************************************************************************/
void SSD1351_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color)
{
    SSD1351_DrawLine(x1,y1,x2,y1,color);
    SSD1351_DrawLine(x1,y1,x1,y2,color);
    SSD1351_DrawLine(x1,y2,x2,y2,color);
    SSD1351_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
      返回值：  无
******************************************************************************/
void SSD1351_Draw_Circle(unsigned int x0,unsigned int y0,unsigned char r,unsigned int color)
{
    int a,b;
    //int di;
    a=0;b=r;
    while(a<=b)
    {
        SSD1351_DrawPoint(x0-b,y0-a,color);             //3
        SSD1351_DrawPoint(x0+b,y0-a,color);             //0
        SSD1351_DrawPoint(x0-a,y0+b,color);             //1
        SSD1351_DrawPoint(x0-a,y0-b,color);             //2
        SSD1351_DrawPoint(x0+b,y0+a,color);             //4
        SSD1351_DrawPoint(x0+a,y0-b,color);             //5
        SSD1351_DrawPoint(x0+a,y0+b,color);             //6
        SSD1351_DrawPoint(x0-b,y0+a,color);             //7
        a++;
        if((a*a+b*b)>(r*r))//判断要画的点是否过远
        {
            b--;
        }
    }
}


/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void SSD1351_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char temp,sizex,t,m=0;
    unsigned int i,TypefaceNum;//一个字符所占字节大小
    unsigned int x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';    //得到偏移后的值
    SSD1351_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置
    for(i=0;i<TypefaceNum;i++)
    {
        if(sizey==12)temp=ascii_1206[num][i];              //调用6x12字体
        else if(sizey==16)temp=ascii_1608[num][i];       //调用8x16字体
        else if(sizey==24)temp=ascii_2412[num][i];       //调用12x24字体
        else if(sizey==32)temp=ascii_3216[num][i];       //调用16x32字体
        else return;
        for(t=0;t<8;t++)
        {
            if(!mode)//非叠加模式
            {
                if(temp&(0x01<<t))SSD1351_write_data_16(fc);
                else SSD1351_write_data_16(bc);
                m++;
                if(m%sizex==0)
                {
                    m=0;
                    break;
                }
            }
            else//叠加模式
            {
                if(temp&(0x01<<t))SSD1351_DrawPoint(x,y,fc);//画一个点
                x++;
                if((x-x0)==sizex)
                {
                    x=x0;
                    y++;
                    break;
                }
            }
        }
    }
}


/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void SSD1351_ShowString(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    while(*p!='\0')
    {
        SSD1351_ShowChar(x,y,*p,fc,bc,sizey,mode);
        x+=sizey/2;
        p++;
    }
}


/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
unsigned long SSD1351_mypow(unsigned char m,unsigned char n)
{
    unsigned long result=1;
    while(n--)result*=m;
    return result;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void SSD1351_ShowIntNum(unsigned int x,unsigned int y,unsigned int num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey)
{
    unsigned char t,temp;
    unsigned char enshow=0;
    unsigned char sizex=sizey/2;
    for(t=0;t<len;t++)
    {
        temp=(num/SSD1351_mypow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                SSD1351_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
                continue;
            }else enshow=1;

        }
        SSD1351_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
}


/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void SSD1351_ShowFloatNum1(unsigned int x,unsigned int y,float num,unsigned char len,unsigned int fc,unsigned int bc,unsigned char sizey)
{
    unsigned char t,temp,sizex;
    unsigned int num1;
    sizex=sizey/2;
    num1=num*100;
    for(t=0;t<len;t++)
    {
        temp=(num1/SSD1351_mypow(10,len-t-1))%10;
        if(t==(len-2))
        {
            SSD1351_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
            t++;
            len+=1;
        }
        SSD1351_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
    }
}


/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组
      返回值：  无
******************************************************************************/
void SSD1351_ShowPicture(unsigned int x,unsigned int y,unsigned int length,unsigned int width,const unsigned char pic[])
{
    unsigned int i,j;
    unsigned long k=0;
    SSD1351_Address_Set(x,y,x+length-1,y+width-1);
    for(i=0;i<length;i++)
    {
        for(j=0;j<width;j++)
        {
            SSD1351_write_data_8(pic[k*2]);
            SSD1351_write_data_8(pic[k*2+1]);
            k++;
        }
    }
}

/******************************************************************************
      函数说明：显示单个12x12汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void SSD1351_ShowChinese12x12(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char i,j,m=0;
    unsigned int k;
    unsigned int HZnum;//汉字数目
    unsigned int TypefaceNum;//一个字符所占字节大小
    unsigned int x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;

    HZnum = CNHZNUM;//sizeof(tfont12)/sizeof(typFNT_GB12);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
        {
            SSD1351_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j))SSD1351_write_data_16(fc);
                        else SSD1351_write_data_16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j)) SSD1351_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个16x16汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void SSD1351_ShowChinese16x16(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char i,j,m=0;
    unsigned int k;
    unsigned int HZnum;//汉字数目
    unsigned int TypefaceNum;//一个字符所占字节大小
    unsigned int x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum = CNHZNUM;//sizeof(tfont16)/sizeof(typFNT_GB16);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
        {
            SSD1351_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j))SSD1351_write_data_16(fc);
                        else SSD1351_write_data_16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j)) SSD1351_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个24x24汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void SSD1351_ShowChinese24x24(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char i,j,m=0;
    unsigned int k;
    unsigned int HZnum;//汉字数目
    unsigned int TypefaceNum;//一个字符所占字节大小
    unsigned int x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum = CNHZNUM;//sizeof(tfont24)/sizeof(typFNT_GB24);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
        {
            SSD1351_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j))SSD1351_write_data_16(fc);
                        else SSD1351_write_data_16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j)) SSD1351_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示单个32x32汉字
      入口数据：x,y显示坐标
                *s 要显示的汉字
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void SSD1351_ShowChinese32x32(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char i,j,m=0;
    unsigned int k;
    unsigned int HZnum;//汉字数目
    unsigned int TypefaceNum;//一个字符所占字节大小
    unsigned int x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum = CNHZNUM;//sizeof(tfont32)/sizeof(typFNT_GB32);  //统计汉字数目
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
        {
            SSD1351_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//非叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j))SSD1351_write_data_16(fc);
                        else SSD1351_write_data_16(bc);
                        m++;
                        if(m%sizey==0)
                        {
                            m=0;
                            break;
                        }
                    }
                    else//叠加方式
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j)) SSD1351_DrawPoint(x,y,fc);//画一个点
                        x++;
                        if((x-x0)==sizey)
                        {
                            x=x0;
                            y++;
                            break;
                        }
                    }
                }
            }
        }
        continue;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
    }
}

/******************************************************************************
      函数说明：显示汉字字符串
      入口数据：x,y显示坐标
                *s 要显示的汉字字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void SSD1351_ShowChineseString(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{

    while(*s != '\0')
    {
        switch(sizey)
        {
            case 12:
            {
                SSD1351_ShowChinese12x12(x , y , s , fc , bc , sizey , mode);
                x+=sizey;
                s = s + 2;
                break;
            }
            case 16:
            {
                SSD1351_ShowChinese16x16(x , y , s , fc , bc , sizey , mode);
                x+=sizey;
                s = s + 2;
                break;
            }
            case 24:
            {
                SSD1351_ShowChinese24x24(x , y , s , fc , bc , sizey , mode);
                x+=sizey;
                s = s + 2;
                break;
            }
            case 32:
            {
                SSD1351_ShowChinese32x32(x , y , s , fc , bc , sizey , mode);
                x+=sizey;
                s = s + 2;
                break;
            }
        }
    }
}

void SSD1351_Show_String(unsigned int x,unsigned int y, char *p,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    while(*p!='\0')
    {
        if(*p <= 0x7F)
        {
            SSD1351_ShowChar(x,y,*p,fc,bc,sizey,mode);
            x+=sizey/2;
            p++;
        }
        else if(*p > 0x7F)
        {
            switch(sizey)
            {
                case 12:
                {
                    SSD1351_ShowChinese12x12(x , y , p , fc , bc , sizey , mode);
                    x+=sizey;
                    p = p + 2;
                    break;
                }
                case 16:
                {
                    SSD1351_ShowChinese16x16(x , y , p , fc , bc , sizey , mode);
                    x+=sizey;
                    p = p + 2;
                    break;
                }
                case 24:
                {
                    SSD1351_ShowChinese24x24(x , y , p , fc , bc , sizey , mode);
                    x+=sizey;
                    p = p + 2;
                    break;
                }
                case 32:
                {
                    SSD1351_ShowChinese32x32(x , y , p , fc , bc , sizey , mode);
                    x+=sizey;
                    p = p + 2;
                    break;
                }
            }
        }
    }
}

