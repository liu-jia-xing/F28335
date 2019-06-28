/*
 * GUI.c
 *
 *  Created on: 2019年5月25日
 *      Author: sunxiaobing
 */
#include "DSP28x_Project.h"
#include "LCD_24TFT.h"
#include "GUI.h"
#include "Delay.h"
#include "font.h"
#include "math.h"
/************************************************************************/
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
/************************************************************************/
#define PI 3.1415
u16 LCD_BGR2RGB(u16 c)
{
  u16  r,g,b,rgb;
  b=(c>>0)&0x1f;
  g=(c>>5)&0x3f;
  r=(c>>11)&0x1f;
  rgb=(b<<11)+(g<<5)+(r<<0);
  return(rgb);

}
/**************************************************************/
//画线函数，使用Bresenham 画线算法
/**************************************************************/
void Gui_DrawLine(u16 x0, u16 y0,u16 x1, u16 y1,u16 Color)
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2,
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping


    Lcd_SetXY(x0,y0);
    dx = x1-x0;//计算x距离
    dy = y1-y0;//计算y距离

    if (dx>=0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx    = -dx;
    }

    if (dy>=0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy    = -dy;
    }

    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
    {//且线的点数等于x距离，以x轴递增画点
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index=0; index <= dx; index++)//要画的点数不会超过x距离
        {
            //画点
            Gui_DrawPoint(x0,y0,Color);

            // test if error has overflowed
            if (error >= 0) //是否需要增加y坐标值
            {
                error-=dx2;

                // move to next line
                y0+=y_inc;//增加y坐标值
            } // end if error overflowed

            // adjust the error term
            error+=dy2;

            // move to the next pixel
            x0+=x_inc;//x坐标值每次画点后都递增1
        } // end for
    } // end if |slope| <= 1
    else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
    {//以y轴为递增画点
        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index=0; index <= dy; index++)
        {
            // set the pixel
            Gui_DrawPoint(x0,y0,Color);

            // test if error overflowed
            if (error >= 0)
            {
                error-=dy2;

                // move to next line
                x0+=x_inc;
            } // end if error overflowed

            // adjust the error term
            error+=dx2;

            // move to the next pixel
            y0+=y_inc;
        } // end for
    } // end else |slope| > 1
}
/**********************************************************************************/
//画虚线
/**********************************************************************************/
void Gui_Draw_DotLine(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned int line_color)   //画虚线函数
{
    int temp;
    int dx,dy;                  //定义起点到终点的横、纵坐标增加值
    int s1,s2,status,i;
    int Dx,Dy,sub;

      dx = x1 - x0;
    if(dx >= 0)                 //X的方向是增加的
        s1 = 1;
    else                          //X的方向是降低的
        s1 = -1;
    dy = y1 - y0;               //判断Y的方向是增加还是降到的
    if(dy >= 0)
        s2 = 1;
    else
        s2 =- 1;

    Dx = x1-x0;             //计算横、纵标志增加值的绝对值
    Dy = y1-y0;

    if(Dy > Dx)
    {                                   //以45度角为分界线，靠进Y轴是status=1,靠近X轴是status=0
        temp = Dx;
        Dx = Dy;
        Dy = temp;
    status = 1;
    }
    else
        status = 0;

/*********Bresenham算法画任意两点间的直线********/
    sub = Dy + Dy - Dx;                              //第1次判断下个点的位置
    for(i = 0;i < Dx;i ++)
    {
        if((i%10)<=4)
            Gui_DrawPoint(x0,y0,line_color);       //画点，虚线间距大小可以在这里调节，设置不同的判断条件
        if(sub >= 0)
        {
            if(status == 1)                         //在靠近Y轴区，x值加1
                x0 += s1;
            else                                //在靠近X轴区，y值加1
                y0 += s2;
            sub -= (Dx + Dx);                       //判断下下个点的位置
        }
        if(status == 1)
            y0 += s2;
        else
            x0 += s1;
        sub += Dy + Dy;
    }
}
/**************************************************************/
// 画框
/**************************************************************/
void Gui_box(u16 x, u16 y, u16 w, u16 h,u16 bc)
{
    Gui_DrawLine(x,y,x+w,y,0xEF7D);
    Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
    Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
    Gui_DrawLine(x,y,x,y+h,0xEF7D);
    Gui_DrawLine(x+1,y+1,x+1+w-2,y+1+h-2,bc);
}
/**************************************************************/
//  画框2
/**************************************************************/
void Gui_box2(u16 x,u16 y,u16 w,u16 h, u8 mode)
{
    if (mode==0)    {
        Gui_DrawLine(x,y,x+w,y,0xEF7D);
        Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0x2965);
        Gui_DrawLine(x,y+h,x+w,y+h,0x2965);
        Gui_DrawLine(x,y,x,y+h,0xEF7D);
        }
    if (mode==1)    {
        Gui_DrawLine(x,y,x+w,y,0x2965);
        Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xEF7D);
        Gui_DrawLine(x,y+h,x+w,y+h,0xEF7D);
        Gui_DrawLine(x,y,x,y+h,0x2965);
    }
    if (mode==2)    {
        Gui_DrawLine(x,y,x+w,y,0xffff);
        Gui_DrawLine(x+w-1,y+1,x+w-1,y+1+h,0xffff);
        Gui_DrawLine(x,y+h,x+w,y+h,0xffff);
        Gui_DrawLine(x,y,x,y+h,0xffff);
    }
}

/****************************************************************************
* 名称：GUI_Circle(int xc,int yc,int r,unsigned char circle_colour)
* 功能：画圆
* 入口参数：xc               圆心的行坐标
*           yc          圆心的列坐标
*           r               半径
*          circle_color     显示颜色
* 出口参数：无
****************************************************************************/
void GUIcircle(unsigned int xc,unsigned int yc,unsigned int r,unsigned int yc_colour)
{
    int x,y,d;
    y = r;
    d = 3 - (r + r);
    x = 0;
    while(x <= y)
    {
        Gui_DrawPoint(xc+x,yc+y,yc_colour);
        Gui_DrawPoint(xc+x,yc-y,yc_colour);
        Gui_DrawPoint(xc-x,yc+y,yc_colour);
        Gui_DrawPoint(xc-x,yc-y,yc_colour);
        Gui_DrawPoint(xc+y,yc+x,yc_colour);
        Gui_DrawPoint(xc+y,yc-x,yc_colour);
        Gui_DrawPoint(xc-y,yc+x,yc_colour);
        Gui_DrawPoint(xc-y,yc-x,yc_colour);

        if(d < 0)
            d += (x + x + x + x) + 6;
        else
        {
            d+=((x - y) + (x - y) + (x - y) + (x - y)) + 10;
            y = y - 1;
        }
        x = x + 1;
    }
}
/**************************************************************************************
功能描述: 在屏幕显示一凸起的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonDown(u16 x1,u16 y1,u16 x2,u16 y2)
{
    Gui_DrawLine(x1,  y1,  x2,y1, GRAY2);  //H
    Gui_DrawLine(x1+1,y1+1,x2,y1+1, GRAY1);  //H
    Gui_DrawLine(x1,  y1,  x1,y2, GRAY2);  //V
    Gui_DrawLine(x1+1,y1+1,x1+1,y2, GRAY1);  //V
    Gui_DrawLine(x1,  y2,  x2,y2, WHITE);  //H
    Gui_DrawLine(x2,  y1,  x2,y2, WHITE);  //V
}

/**************************************************************************************
功能描述: 在屏幕显示一凹下的按钮框
输    入: u16 x1,y1,x2,y2 按钮框左上角和右下角坐标
输    出: 无
**************************************************************************************/
void DisplayButtonUp(u16 x1,u16 y1,u16 x2,u16 y2)
{
    Gui_DrawLine(x1,  y1,  x2,y1, WHITE); //H
    Gui_DrawLine(x1,  y1,  x1,y2, WHITE); //V

    Gui_DrawLine(x1+1,y2-1,x2,y2-1, GRAY1);  //H
    Gui_DrawLine(x1,  y2,  x2,y2, GRAY2);  //H
    Gui_DrawLine(x2-1,y1+1,x2-1,y2, GRAY1);  //V
    Gui_DrawLine(x2  ,y1  ,x2,y2, GRAY2); //V
}
void ClearButtonUp(u16 x1,u16 y1,u16 x2,u16 y2,u16 bcolour)
{
    Gui_DrawLine(x1,  y1,  x2,y1, bcolour); //H
    Gui_DrawLine(x1,  y1,  x1,y2, bcolour); //V

    Gui_DrawLine(x1+1,y2-1,x2,y2-1, bcolour);  //H
    Gui_DrawLine(x1,  y2,  x2,y2, bcolour);  //H
    Gui_DrawLine(x2-1,y1+1,x2-1,y2, bcolour);  //V
    Gui_DrawLine(x2  ,y1  ,x2,y2, bcolour); //V
}

void Gui_DrawFont_GBK16(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
    unsigned char i,j;
    unsigned short k,x0;
    x0=x;

    while(*s)
    {
        if((*s) < 128)
        {
            k=*s;
            if (k==13)
            {
                x=x0;
                y+=16;
            }
            else
            {
                if (k>32) k-=32; else k=0;

                for(i=0;i<16;i++)
                for(j=0;j<8;j++)
                    {
                        if(asc16[k*16+i]&(0x80>>j)) Gui_DrawPoint(x+j,y+i,fc);
                        else
                        {
                            if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
                        }
                    }
                x+=8;
            }
            s++;
        }

        else
        {


            for (k=0;k<hz16_num;k++)
            {
              if ((hz16[k].Index[0]==*(s))&&(hz16[k].Index[1]==*(s+1)))
              {
                    for(i=0;i<16;i++)
                    {
                        for(j=0;j<8;j++)
                            {
                                if(hz16[k].Msk[i*2]&(0x80>>j))  Gui_DrawPoint(x+j,y+i,fc);
                                else {
                                    if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
                                }
                            }
                        for(j=0;j<8;j++)
                            {
                                if(hz16[k].Msk[i*2+1]&(0x80>>j))    Gui_DrawPoint(x+j+8,y+i,fc);
                                else
                                {
                                    if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
                                }
                            }
                    }break;
                }
              }
            s+=2;x+=16;
        }

    }
}
/*****************************************************************************/
//函数说明：显示24*24分辨率的汉字或字符
/******************************************************************************/
void Gui_DrawFont_GBK24(u16 x, u16 y, u16 fc, u16 bc, u8 *s)
{
    unsigned char i,j;
    unsigned int k;

    while(*s)
    {
        if( *s < 0x80 )
        {
            k=*s;
            if (k>32) k-=32; else k=0;

            for(i=0;i<16;i++)
            for(j=0;j<8;j++)
                {
                    if(asc16[k*16+i]&(0x80>>j))
                    Gui_DrawPoint(x+j,y+i,fc);
                    else
                    {
                        if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
                    }
                }
            s++;x+=8;
        }
        else
        {

            for (k=0;k<hz24_num;k++)
            {
              if ((hz24[k].Index[0]==*(s))&&(hz24[k].Index[1]==*(s+1)))
              {
                    for(i=0;i<24;i++)
                    {
                        for(j=0;j<8;j++)
                            {
                                if(hz24[k].Msk[i*3]&(0x80>>j))
                                Gui_DrawPoint(x+j,y+i,fc);
                                else
                                {
                                    if (fc!=bc) Gui_DrawPoint(x+j,y+i,bc);
                                }
                            }
                        for(j=0;j<8;j++)
                            {
                                if(hz24[k].Msk[i*3+1]&(0x80>>j))    Gui_DrawPoint(x+j+8,y+i,fc);
                                else {
                                    if (fc!=bc) Gui_DrawPoint(x+j+8,y+i,bc);
                                }
                            }
                        for(j=0;j<8;j++)
                            {
                                if(hz24[k].Msk[i*3+2]&(0x80>>j))
                                Gui_DrawPoint(x+j+16,y+i,fc);
                                else
                                {
                                    if (fc!=bc) Gui_DrawPoint(x+j+16,y+i,bc);
                                }
                            }
                    }
              }
            }
            s+=2;x+=24;
        }
    }
}
/*******************************************************************************/
//函数说明：显示数码管字体格式的数字
/*******************************************************************************/
void Gui_DrawFont_Num32(u16 x, u16 y, u16 fc, u16 bc, u16 num)
{
    unsigned char i,j,k,c;
    //lcd_text_any(x+94+i*42,y+34,32,32,0x7E8,0x0,sz32,knum[i]);
//  w=w/8;

    for(i=0;i<32;i++)
    {
        for(j=0;j<4;j++)
        {
            c=*(sz32+num*32*4+i*4+j);
            for (k=0;k<8;k++)
            {

                if(c&(0x80>>k)) Gui_DrawPoint(x+j*8+k,y+i,fc);
                else {
                    if (fc!=bc) Gui_DrawPoint(x+j*8+k,y+i,bc);
                }
            }
        }
    }
}

/****************************************************************************
* 名称：GUITable(u16 x0,u16 y0,u16 x1,u16 y1,uchar TH,uchar TL,u16 colour)
* 功能：绘制任意行列的表格
* 入口参数：x0       起始点横坐标
*          y0       起始点纵坐标
*          x1       终止点横坐标
*          y1       终止点纵坐标
*          TH           表格行数
*          TL           表格列数
*          color        表格边框颜色
*          temp      0 画虚线     1 画实线
* 出口参数：无
****************************************************************************/
void GUITable(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1,unsigned char TH,unsigned char TL,unsigned int color,unsigned char temp)
{
  unsigned char i;

  for(i=0;i<=TH;i++)
  {
      if(temp==1)
          Gui_DrawLine(x0,y0+i*(y1-y0)/TH,x1,y0+i*(y1-y0)/TH,color);     //画实线
      else
          Gui_Draw_DotLine(x0,y0+i*(y1-y0)/TH,x1,y0+i*(y1-y0)/TH,color); //画虚线
  }

  for(i=0;i<=TL;i++)
  {
      if(temp==1)
          Gui_DrawLine(x0+i*(x1-x0)/TL,y0,x0+i*(x1-x0)/TL,y1,color);   //画实线
      else
          Gui_Draw_DotLine(x0+i*(x1-x0)/TL,y0,x0+i*(x1-x0)/TL,y1,color);   //画虚线
  }

}
/*******************************************************************************/
//显示固定分辨率的图片
/*******************************************************************************/

void Show_Image(unsigned int x0,unsigned int y0,unsigned int x1,unsigned int y1, const unsigned char *p)
{
    unsigned int i,j;
    unsigned char pich,picl;
//    Lcd_Clear(GRAY0);
    Lcd_SetRegion(x0,y0,x1-1,y1-1);       //坐标设置
    for(i=y0-y0;i<y1-y0;i++)
        for(j=x0-x0;j<x1-x0;j++)
        {
            picl=p[i*(x1-x0)*2+j*2];  //数据低位在前
            pich=p[i*(x1-x0)*2+j*2+1];
            Lcd_WriteData_16Bit(pich<<8|picl);
        }
}

/****************************************************************************
* 名称：GUICoordinate(u16 x0,u16 y0,u16 xm,u16 ym,uchar pix,uint ccolor,u16 bcolor)
* 功能：绘制一个坐标轴，以用于ADC显示电压为参考，可自行更改
* 入口参数：x0       原点横坐标
*           y0      原点纵坐标
*           xm      横坐标长度
*           ym          纵坐标长度
*           pix         坐标轴像素值大小，如1,2,3……
*           bcolor      背景颜色
*           ccolor      字符颜色
* 出口参数：无
****************************************************************************/
void GUICoordinate(u16 x0,u16 y0,u16 xm,u16 ym,u8 pix,u16 ccolor,u16 bcolor)
{
  u8 i,xj,yj;

  for(i=0;i<pix;i++)
  {
      Gui_DrawLine(x0-i,y0,x0-i,y0-ym,ccolor);                     //画Y轴
  }

  for(i=0;i<pix;i++)
  {
      Gui_DrawLine(x0-pix+1,y0+i,x0+xm,y0+i,ccolor);               //画X轴
  }

  Gui_DrawLine(x0,y0-ym,x0-5*cos(1*PI/3),y0-ym+5*sin(1*PI/3),ccolor);  //绘制各轴箭头，10个像素长，大小可更改里面的10这个值
  Gui_DrawLine(x0,y0-ym,x0+5*cos(1*PI/3),y0-ym+5*sin(1*PI/3),ccolor);
//  LCD_PutString(x0+10*cos(1*PI/3),y0-ym+10*sin(1*PI/3),"Voltage(V)",ccolor,bcolor);      //显示Y轴参数定义，如电压V

  Gui_DrawLine(x0+xm,y0,x0+xm-5*sin(1*PI/3),y0+5*cos(1*PI/3),ccolor);
  Gui_DrawLine(x0+xm,y0,x0+xm-5*sin(1*PI/3),y0-5*cos(1*PI/3),ccolor);
//  LCD_PutString(x0+xm-10*sin(1*PI/3)-32,y0-10*cos(1*PI/3)-16,"T(t)",ccolor,bcolor);      //显示X轴参数定义，如时间T


  xj=10;  //横坐标10个刻度
  yj=6;   //纵坐标5个刻度线，共6段

  for(i=1;i<yj;i++)
  {
      Gui_DrawLine(x0,y0-i*ym/yj,x0-5,y0-i*ym/yj,ccolor);    //绘制刻度，Y轴，等分绘制

//    LCD_PutString(x0-5-24,y0-i*ym/yj-8,Taby[i],ccolor,bcolor);  //显示刻度参数值，定义在Tab表里
  }

  for(i=1;i<xj;i++)
  {
      Gui_DrawLine(x0+i*xm/xj,y0,x0+i*xm/xj,y0+5,ccolor);   //绘制刻度，X轴，等分绘制
//    LCD_PutString(x0+i*xm/xj-4,y0+5,Tabx[i],ccolor,bcolor); //显示刻度参数值，定义在Tab表里
  }

//  LCD_PutString(x0-12,y0+4,"O",ccolor,bcolor);      //显示原点0
//  Gui_DrawFont_GBK16(x0-12,y0+4,ccolor,bcolor,"O");
}
/*************************************************************************************/
//显示一个32位整数
/************************************************************************************/
void Show_Num(Uint16 x0,Uint16 y0,Uint32 Num,Uint16 ccolor,Uint16 bcolor)
{
    u8 num[]={0,0,0,0,0,0,0,0,0};
    num[0] = Num/10000000;
    num[1] = (Num/1000000)%10;
    num[2] = (Num/100000)%10;
    num[3] = (Num/10000)%10;
    num[4] = (Num/1000)%10;
    num[5] = (Num/100)%10;
    num[6] = (Num/10)%10;
    num[7] = Num%10;

    Gui_DrawFont_GBK16(x0,y0,ccolor,bcolor,num);

}
/*************************************测试部分***************************************/

unsigned char Num[10]={0,1,2,3,4,5,6,7,8,9};

//绘制主菜单界面
/*******************************/
//    简易频率特性测试仪
//     1.单点测试
//     2.点频测试
//     3.DDS扫描
//     4.频率扫面
/*******************************/
void Draw_Mainmenu(void)
{
    Lcd_Clear(GRAY0);

    u8 cha1[]={"简易频率特性测试仪"};
    Gui_DrawFont_GBK24(50,8,RED,GRAY0,cha1);

//    DisplayButtonUp(79,46,177,74); //x1,y1,x2,y2
    u8 cha2[]={"单点测试"};
    Gui_DrawFont_GBK24(80,48,BLUE,GRAY0,cha2);
//  Gui_DrawFont_Num32(40,44,RED,GRAY0,Num[1]);
//  Show_Image(200,48,220,68, gImage_1);

//    DisplayButtonUp(79,86,177,112); //x1,y1,x2,y2
    u8 cha3[]={"点频测试"};
    Gui_DrawFont_GBK24(80,88,BLUE,GRAY0,cha3);
//  Gui_DrawFont_Num32(40,84,RED,GRAY0,Num[2]);

//    DisplayButtonUp(79,126,177,152); //x1,y1,x2,y2
    u8 cha4[]={"DDS扫频"};
    Gui_DrawFont_GBK24(80,128,BLUE,GRAY0,cha4);
//  Gui_DrawFont_Num32(40,124,RED,GRAY0,Num[3]);

//    DisplayButtonUp(79,166,177,192); //x1,y1,x2,y2
    u8 cha5[]={"频率扫描"};
    Gui_DrawFont_GBK24(80,168,BLUE,GRAY0,cha5);
//  Gui_DrawFont_Num32(40,164,RED,GRAY0,Num[4]);

    u8 cha6[]={"江金龙指导"};
    Gui_DrawFont_GBK16(20,208,RED,GRAY0,cha6);

    u8 cha7[]={"孙小兵 刘嘉兴 欧阳梁设计"};
    Gui_DrawFont_GBK16(120,208,RED,GRAY0,cha7);


/*
    u8 cha1[]={"全动电子技术"};
    u8 cha2[]={"液晶测试程序"};
    Gui_DrawFont_GBK16(16,2,BLUE,GRAY0,cha1);
    Gui_DrawFont_GBK16(16,20,RED,GRAY0,cha2);
    u8 cha3[]={"颜色填充测试"};
    DisplayButtonUp(15,38,113,58); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(16,40,GREEN,GRAY0,cha3);
    u8 cha4[]={"文字显示测试"};
    DisplayButtonUp(15,68,113,88); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(16,70,BLUE,GRAY0,cha4);
    u8 cha5[]={"图片显示测试"};
    DisplayButtonUp(15,98,113,118); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(16,100,RED,GRAY0,cha5);

    //Gui_DrawFont_GBK16(16,120,BLUE,GRAY0,"Welcome");
    Gui_DrawFont_GBK16(16,140,RED,GRAY0,"Welcome");

    Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[5]);
    Delay_ms(1000);
    Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[4]);
    Delay_ms(1000);
    Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[3]);
    Delay_ms(1000);
    Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[2]);
    Delay_ms(1000);
    Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[1]);
    Delay_ms(1000);
    Gui_DrawFont_Num32(100,125,RED,GRAY0,Num[0]);
*/
}

//绘制子菜单1界面
/*******************************/
//      单点测试
//  I路     频率：
//       相位：
//  Q路     频率：
//       相位：
/*******************************/
void Draw_Submenu1(void)
{
    Lcd_Clear(GRAY0);

    u8 chb1[]={"单点测试"};
    Gui_DrawFont_GBK24(100,8,RED,GRAY0,chb1);

    u8 chb2[]={"I路"};
    Gui_DrawFont_GBK24(10,60,RED,GRAY0,chb2);

    u8 chb3[]={"Q路"};
    Gui_DrawFont_GBK24(10,140,RED,GRAY0,chb3);

    u8 chb4[]={"频率"};
    Gui_DrawFont_GBK24(80,60,BLUE,GRAY0,chb4);

//    DisplayButtonUp(139,59,281,85); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,68,BLUE,GRAY0,"Hz");

    Gui_DrawFont_GBK24(80,140,BLUE,GRAY0,chb4);

//    DisplayButtonUp(139,139,281,165); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,148,BLUE,GRAY0,"Hz");

    u8 chb5[]={"相位"};
    u8 chb6[]={"度"};
    Gui_DrawFont_GBK24(80,100,BLUE,GRAY0,chb5);

//    DisplayButtonUp(139,99,281,125); //x1,y1,x2,y2
    Gui_DrawFont_GBK24(285,100,BLUE,GRAY0,chb6);

    Gui_DrawFont_GBK24(80,180,BLUE,GRAY0,chb5);

//    DisplayButtonUp(139,179,281,205); //x1,y1,x2,y2
    Gui_DrawFont_GBK24(285,180,BLUE,GRAY0,chb6);
}
//绘制子菜单2界面
/*******************************/
//         点频测试
//*******************************
// 输出频率
//
// 电压增益：             相移：
/*******************************/
void Draw_Submenu2()
{
    Lcd_Clear(GRAY0);

    u8 chc1[]={"点频测试"};
    Gui_DrawFont_GBK24(100,8,RED,GRAY0,chc1);

    u8 chc2[]={"输出频率:"};
    Gui_DrawFont_GBK24(10,80,BLUE,GRAY0,chc2);
    //DisplayButtonUp(139,79,281,105); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,88,BLUE,GRAY0,"Hz");

    u8 chc3[]={"电压增益:"};
    Gui_DrawFont_GBK24(10,160,BLUE,GRAY0,chc3);
    //DisplayButtonUp(139,159,281,185); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,168,BLUE,GRAY0,"dB");

    u8 chc4[]={"相移:"};
    Gui_DrawFont_GBK24(10,200,BLUE,GRAY0,chc4);
    //DisplayButtonUp(139,199,281,225); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,208,BLUE,GRAY0,"*");


}
//绘制子菜单3界面
/*******************************/
//         DDS扫频
//*******************************
// 最小频率:
// 最大频率:
// 扫频步进:
//
/*******************************/
void Draw_Submenu3()
{
    Lcd_Clear(GRAY0);

    u8 chd1[]={"DDS扫频"};
    Gui_DrawFont_GBK24(100,8,RED,GRAY0,chd1);

    u8 chd2[]={"最大频率:"};
    Gui_DrawFont_GBK24(10,40,BLUE,GRAY0,chd2);
    DisplayButtonUp(139,39,281,65); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,48,BLUE,GRAY0,"Hz");

    u8 chd3[]={"最小频率:"};
    Gui_DrawFont_GBK24(10,80,BLUE,GRAY0,chd3);
    DisplayButtonUp(139,79,281,105); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,88,BLUE,GRAY0,"Hz");

    u8 chd4[]={"扫频步进:"};
    Gui_DrawFont_GBK24(10,160,BLUE,GRAY0,chd4);
    DisplayButtonUp(139,159,281,185); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,168,BLUE,GRAY0,"Hz");

}
//绘制子菜单4界面
/*******************************/
//         频率扫描
//*******************************
// 最小频率:
// 最大频率:
// 扫频步进:
//
/*******************************/
void Draw_Submenu4(void)
{
    Lcd_Clear(GRAY0);

    u8 che1[]={"频率扫描"};
    Gui_DrawFont_GBK24(100,8,RED,GRAY0,che1);

    u8 che2[]={"最大频率:"};
    Gui_DrawFont_GBK24(10,40,BLUE,GRAY0,che2);
//    DisplayButtonUp(139,39,281,65); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,48,BLUE,GRAY0,"Hz");

    u8 che3[]={"最小频率:"};
    Gui_DrawFont_GBK24(10,80,BLUE,GRAY0,che3);
//    DisplayButtonUp(139,79,281,105); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,88,BLUE,GRAY0,"Hz");

    u8 che4[]={"扫频步进:"};
    Gui_DrawFont_GBK24(10,160,BLUE,GRAY0,che4);
//    DisplayButtonUp(139,159,281,185); //x1,y1,x2,y2
    Gui_DrawFont_GBK16(285,168,BLUE,GRAY0,"Hz");
}
//波形显示界面框架
void WavePicture(void)
{
    Lcd_Clear(GRAY0);
    Gui_DrawLine(220, 0,220,240,BLACK);
    Gui_DrawLine(222, 0,222,240,BLACK);
    Gui_Draw_DotLine(0,120,220,120,BLACK);

    u8 chf1[]={"中心频率:"};
    Gui_DrawFont_GBK24(224,0,BLUE,GRAY0,chf1);

    Gui_DrawFont_GBK16(225,40,BLUE,GRAY0,"00000000Hz");
//    DisplayButtonUp(224,39,319,57); //x1,y1,x2,y2
//    Gui_DrawFont_GBK16(254,168,BLUE,GRAY0,"Hz");
    u8 chf2[]={"带宽:"};
    Gui_DrawFont_GBK24(224,80,BLUE,GRAY0,chf2);

    Gui_DrawFont_GBK16(225,120,BLUE,GRAY0,"00000000Hz");
//    DisplayButtonUp(224,119,319,137); //x1,y1,x2,y2
//    Gui_DrawFont_GBK16(254,168,BLUE,GRAY0,"Hz");

    GUICoordinate(20,100,195,95,2,RED,GRAY0);
    GUICoordinate(20,220,195,95,2,RED,GRAY0);

}
//测试数码管字体
void Num_Test(void)
{
    u8 i=0;
    Lcd_Clear(GRAY0);
    Gui_DrawFont_GBK16(16,20,RED,GRAY0,"Num Test");
    Delay_ms(1000);
    Lcd_Clear(GRAY0);

    for(i=0;i<10;i++)
    {
    Gui_DrawFont_Num32((i%3)*40,32*(i/3)+30,RED,GRAY0,Num[i+1]);
    Delay_ms(100);
    }

}
//中英文显示测试
void Font_Test(void)
{
    Lcd_Clear(GRAY0);
    Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"文字显示测试");

    Delay_ms(1000);
    Lcd_Clear(GRAY0);
    Gui_DrawFont_GBK16(16,30,RED,GRAY0,"全动电子技术");
    Gui_DrawFont_GBK16(16,50,BLUE,GRAY0,"专注液晶批发");
    Gui_DrawFont_GBK16(16,70,RED,GRAY0, "全程技术支持");
    Gui_DrawFont_GBK16(0,100,BLUE,GRAY0,"Tel:15989313508");
    Gui_DrawFont_GBK16(0,130,RED,GRAY0, "www.qdtech.net");
    Delay_ms(1500);
}
//简单刷屏测试
void Color_Test(void)
{
    u8 i=1;
    Lcd_Clear(GRAY0);
    u8 chb1[]={"九江学院"};
    Gui_DrawFont_GBK24(20,10,BLUE,GRAY0,chb1);
    Delay_ms(1200);

    while(i--)
    {
        Lcd_Clear(WHITE); Delay_ms(500);
        Lcd_Clear(BLACK); Delay_ms(500);
        Lcd_Clear(RED);   Delay_ms(500);
        Lcd_Clear(GREEN); Delay_ms(500);
        Lcd_Clear(BLUE);  Delay_ms(500);
    }
}

//文字显示测试
//16位BMP 40X40 QQ图像取模数据
//Image2LCD取模选项设置
//水平扫描
//16位
//40X40
//不包含图像头数据
//自左至右
//自顶至底
//低位在前
void showimage(const unsigned char *p) //显示40*40 QQ图片
{
    int i,j,k;
    unsigned char picH,picL;
    Lcd_Clear(GRAY0);
    Gui_DrawFont_GBK16(16,10,BLUE,GRAY0,"图片显示测试");
    Delay_ms(1000);

    Lcd_Clear(GRAY0);
    for(k=0;k<Y_MAX_PIXEL/40;k++)
    {
        for(j=0;j<X_MAX_PIXEL/40;j++)
        {
            Lcd_SetRegion(40*j,40*k,40*j+39,40*k+39);       //坐标设置
            for(i=0;i<40*40;i++)
             {
                picL=*(p+i*2);  //数据低位在前
                picH=*(p+i*2+1);
                Lcd_WriteData_16Bit(picH<<8|picL);
             }
         }
    }
}
//综合测试函数
void QDTFT_Test_Demo(void)
{


//  Draw_Mainmenu();   //绘制主菜单界面
//  Delay_ms(1500);
//  Draw_Submenu1();
//    Draw_Submenu2();
    Draw_Submenu3();
//  Color_Test();//简单纯色填充测试
//  Num_Test();//数码管字体测试
//  Font_Test();//中英文显示测试
//  showimage(gImage_qq);//图片显示示例
//  Delay_ms(1500);

}


