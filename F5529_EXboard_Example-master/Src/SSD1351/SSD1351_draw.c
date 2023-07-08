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

#define CNHZNUM 27       //�ֿ⺺������

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
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void SSD1351_Fill(unsigned int xsta,unsigned int ysta,unsigned int xend,unsigned int yend,unsigned int color)
{
    unsigned int i,j;
    SSD1351_Address_Set(xsta,ysta,xend,yend);      //���ù��λ��
    for(i=ysta;i<=yend;i++)
    {
        for(j=xsta;j<=xend;j++)
        {
            SSD1351_write_data_16(color);//���ù��λ��
        }
    }
}

/******************************************************************************
      ����˵����LCD����
      ������ݣ�x,y   ��ʼ����
      ����ֵ��  ��
******************************************************************************/
void SSD1351_DrawPoint(unsigned int x,unsigned int y,unsigned int color)
{
    SSD1351_Address_Set(x,y,x,y);//���ù��λ��
    SSD1351_write_data_16(color);
}

/******************************************************************************
      ����˵����LCD��һ����ĵ�
      ������ݣ�x,y   ��ʼ����
      ����ֵ��  ��
******************************************************************************/
void SSD1351_DrawPoint_big(unsigned int x,unsigned int y,unsigned int color)
{
    SSD1351_Fill(x-1,y-1,x+1,y+1,color);
}

/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void SSD1351_DrawLine(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int color)
{
    unsigned int t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    delta_x=x2-x1; //������������
    delta_y=y2-y1;
    uRow=x1;//�����������
    uCol=y1;
    if(delta_x>0)incx=1; //���õ�������
    else if (delta_x==0)incx=0;//��ֱ��
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if (delta_y==0)incy=0;//ˮƽ��
    else {incy=-1;delta_y=-delta_x;}
    if(delta_x>delta_y)distance=delta_x; //ѡȡ��������������
    else distance=delta_y;
    for(t=0;t<distance+1;t++)
    {
        SSD1351_DrawPoint(uRow,uCol,color);//����
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
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
      ����ֵ��  ��
******************************************************************************/
void SSD1351_DrawRectangle(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2,unsigned int color)
{
    SSD1351_DrawLine(x1,y1,x2,y1,color);
    SSD1351_DrawLine(x1,y1,x1,y2,color);
    SSD1351_DrawLine(x1,y2,x2,y2,color);
    SSD1351_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
      ����ֵ��  ��
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
        if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
        {
            b--;
        }
    }
}


/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void SSD1351_ShowChar(unsigned int x,unsigned int y,unsigned char num,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char temp,sizex,t,m=0;
    unsigned int i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    unsigned int x0=x;
    sizex=sizey/2;
    TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
    num=num-' ';    //�õ�ƫ�ƺ��ֵ
    SSD1351_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ��
    for(i=0;i<TypefaceNum;i++)
    {
        if(sizey==12)temp=ascii_1206[num][i];              //����6x12����
        else if(sizey==16)temp=ascii_1608[num][i];       //����8x16����
        else if(sizey==24)temp=ascii_2412[num][i];       //����12x24����
        else if(sizey==32)temp=ascii_3216[num][i];       //����16x32����
        else return;
        for(t=0;t<8;t++)
        {
            if(!mode)//�ǵ���ģʽ
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
            else//����ģʽ
            {
                if(temp&(0x01<<t))SSD1351_DrawPoint(x,y,fc);//��һ����
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
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
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
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
unsigned long SSD1351_mypow(unsigned char m,unsigned char n)
{
    unsigned long result=1;
    while(n--)result*=m;
    return result;
}


/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ��������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
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
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
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
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����
      ����ֵ��  ��
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
      ����˵������ʾ����12x12����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void SSD1351_ShowChinese12x12(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char i,j,m=0;
    unsigned int k;
    unsigned int HZnum;//������Ŀ
    unsigned int TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    unsigned int x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;

    HZnum = CNHZNUM;//sizeof(tfont12)/sizeof(typFNT_GB12);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if((tfont12[k].Index[0]==*(s))&&(tfont12[k].Index[1]==*(s+1)))
        {
            SSD1351_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
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
                    else//���ӷ�ʽ
                    {
                        if(tfont12[k].Msk[i]&(0x01<<j)) SSD1351_DrawPoint(x,y,fc);//��һ����
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
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/******************************************************************************
      ����˵������ʾ����16x16����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void SSD1351_ShowChinese16x16(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char i,j,m=0;
    unsigned int k;
    unsigned int HZnum;//������Ŀ
    unsigned int TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    unsigned int x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum = CNHZNUM;//sizeof(tfont16)/sizeof(typFNT_GB16);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
        {
            SSD1351_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
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
                    else//���ӷ�ʽ
                    {
                        if(tfont16[k].Msk[i]&(0x01<<j)) SSD1351_DrawPoint(x,y,fc);//��һ����
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
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/******************************************************************************
      ����˵������ʾ����24x24����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void SSD1351_ShowChinese24x24(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char i,j,m=0;
    unsigned int k;
    unsigned int HZnum;//������Ŀ
    unsigned int TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    unsigned int x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum = CNHZNUM;//sizeof(tfont24)/sizeof(typFNT_GB24);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
        {
            SSD1351_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
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
                    else//���ӷ�ʽ
                    {
                        if(tfont24[k].Msk[i]&(0x01<<j)) SSD1351_DrawPoint(x,y,fc);//��һ����
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
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/******************************************************************************
      ����˵������ʾ����32x32����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void SSD1351_ShowChinese32x32(unsigned int x,unsigned int y, char *s,unsigned int fc,unsigned int bc,unsigned char sizey,unsigned char mode)
{
    unsigned char i,j,m=0;
    unsigned int k;
    unsigned int HZnum;//������Ŀ
    unsigned int TypefaceNum;//һ���ַ���ռ�ֽڴ�С
    unsigned int x0=x;
    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum = CNHZNUM;//sizeof(tfont32)/sizeof(typFNT_GB32);  //ͳ�ƺ�����Ŀ
    for(k=0;k<HZnum;k++)
    {
        if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
        {
            SSD1351_Address_Set(x,y,x+sizey-1,y+sizey-1);
            for(i=0;i<TypefaceNum;i++)
            {
                for(j=0;j<8;j++)
                {
                    if(!mode)//�ǵ��ӷ�ʽ
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
                    else//���ӷ�ʽ
                    {
                        if(tfont32[k].Msk[i]&(0x01<<j)) SSD1351_DrawPoint(x,y,fc);//��һ����
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
        continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
    }
}

/******************************************************************************
      ����˵������ʾ�����ַ���
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ����ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
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

