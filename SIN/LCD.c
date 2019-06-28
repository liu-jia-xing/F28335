/*
 * LED.c
 *
 *  Created on: 2017年11月27日
 *      Author: lenovo
 */

#include "DSP28x_Project.h"
//#include "LCD.h"

#define uchar unsigned char

#define  BASIC_SET      0x30           //基本指令集  00110000
#define  EXTEND_SET     0x34           //扩展指令集  00110100
#define  DRAW_ON        0x36           //绘图显示开  00110110
#define  DRAW_OFF       0x34           //绘图显示关  00110100
#define  DISP_ON        0x0F           //显示ON,游标ON,游标位反白ON 00001111

#define  execute_72us   delay_loop(400)     //执行时间
#define  SCLK_quarter   delay_loop(40)       //四分之一SCLK时钟 SCLK是LCD串行通信时钟
#define  SCLK_half      delay_loop(80)       //二分之一SCLK时钟 SCLK是LCD串行通信时钟

#define CS_1        GpioDataRegs.GPADAT.bit.GPIO5=1             //GPIO5作为CS
#define CS_0        GpioDataRegs.GPADAT.bit.GPIO5=0             //GPIO5作为CS

#define SCLK_1      GpioDataRegs.GPADAT.bit.GPIO4=1             //GPIO4作为SCLK
#define SCLK_0      GpioDataRegs.GPADAT.bit.GPIO4=0             //GPIO4作为SCLK

#define SDATA_OUT   GpioCtrlRegs.GPADIR.bit.GPIO2=1             //GPIO2作为SDATA
#define SDATA_IN    GpioCtrlRegs.GPADIR.bit.GPIO2=0             //GPIO2作为SDATA
#define SDATA       GpioDataRegs.GPADAT.bit.GPIO2

//#define RST_1       GpioDataRegs.GPADAT.bit.GPIO3=1             //GPIO3做RST
//#define RST_0       GpioDataRegs.GPADAT.bit.GPIO3=0             //GPIO3做RST

//// 数字和英文字符,Uint16会生成16位编码，如：'0'=0x0030
//uchar str_ENG[]="A1511";
//uchar str_10[]="10";
//uchar str_date[]="2017.10.23";
// 特殊图案编码表
// 32个特殊图案的8位编码，字符 uchar编译后为16位，前面补0
uchar sign[]={
0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,
0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,
0x1a,0x1b,0x1c,0x1d,0x1e,0x1f,0x20,0x21,
};

//// 定义中文字符编码变量，ST7920驱动型LCD只能显示下面编号的宋体字，实际大小为16*16点阵
//uchar   jju[]={0xBEC5,0xBDAD,0xD1A7,0xD4BA,0xB5E7,0xD7D3,0xD1A7,0xD4BA};   //“九江学院”编码
//uchar   dian[]={0xB5E7};
//uchar   class[]={0xB0E0};
//uchar   number[]={0xBAC5};
//uchar   name[]={0xD0A4,0xC7BF};
//uchar   desin[]={0xC9E8,0xBCC6};
//Uint16    str6[]="九江学院";   //“九江学院”编码,这种方法，编译后不能再ST7920驱动的LCD上显示指定的汉字

// 下面的定义不能使用
uchar  str2[]="合肥零零电子科技";
uchar  str1[]="黄河远上白云间，一片孤城万仞山。羌笛何须怨杨柳，春风不度玉门关。";


// 位图1：一副动画，宽×高（像素）: 128×64，横向取模，字节正序，16位数据
//uchar  fig1[];

//位图2：九江学院.bmp: 宽×高（像素）: 128×64，横向取模，字节正序，8位数据，
//uchar jju_fig8[];

// 显示图画变量
uchar  fig[512]={0};

Uint16 flag = 1;
uchar i=0;
uchar j=0;
uchar temp;
Uint16 k= 0;

// 绘图有关变量
uchar point1[]={0,0};      // 画线的起点坐标(row,col)
uchar point2[]={63,127};   // 画线的终点坐标

uchar point3[]={63,0};      // 画线的起点坐标
uchar point4[]={0,127};     // 画线的终点坐标

uchar point5[]={0,0};      // 画线的起点坐标
uchar point6[]={0,63};   // 画线的终点坐标

uchar point7[]={63,0};      // 画线的起点坐标
uchar point8[]={63,63};   // 画线的终点坐标

uchar point9[]={63,0};      // 画线的起点坐标
uchar point10[]={0,63};     // 画线的终点坐标

// LCD12864 汉字显示坐标，上下屏结构
uchar AC_TABLE[]={
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,      //第一行汉字位置
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,      //第二行汉字位置
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,      //第三行汉字位置
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,      //第四行汉字位置
};



void InitLcdGpio(void)
{
    EALLOW;

    //GPIO2输出串行数据，接到LCD的5脚；
    GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;     // GPIO2 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;      // GPIO2 = output
    GpioDataRegs.GPASET.bit.GPIO2 = 1;      // Output latch (H)
    GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;      // Enable pullup

    //GPIO3输出复位信号，接到LCD的17脚；
    GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;     // GPIO3 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;      // GPIO3 = output
    GpioDataRegs.GPASET.bit.GPIO3 = 1;      // Output latch (H)
    GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;      // Enable pullup

    //GPIO4输出串行时钟，接到LCD的6脚；
    GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;     // GPIO4 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;      // GPIO4 = output
    GpioDataRegs.GPASET.bit.GPIO4 = 1;      // Output latch (H)
    GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;      // Enable pullup

    //GPIO5输出片选信号，接到LCD的4脚，L电平有效；
    GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;     // GPIO5 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;      // GPIO5 = output
    GpioDataRegs.GPASET.bit.GPIO5 = 1;      // Output latch (H)
    GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;      // Enable pullup

    EDIS;
}

void delay_loop(Uint32 x)
{
    Uint32      i;
    for (i = 0; i < x; i++) {}
}
/******************************************************************************************
* 函数名称    ：WriteByte：写一个8bit数据到LCD
******************************************************************************************/
void WriteByte(Uint16 x)
{
    Uint16 i = 0;
    SDATA_OUT;
    SCLK_quarter;
    for(i=0;i<=7;i++)
    {
        SCLK_0;
        SCLK_quarter;
        SDATA=(0x1&((x)>>(7-i)));  //先把高位移出然后发送
        SDATA=(0x1&((x)>>(7-i)));
        SCLK_quarter;
        SCLK_1;                    //把时间改成上升沿锁存的
        SCLK_half;
        SCLK_0;
        SCLK_quarter;
    }
    SDATA=0;
}



/******************************************************************************************
* 函数名称    ：WriteLcdCmd：写一个8bit命令到LCD
*
* 时序前缀：11111  RW  RS  0
* 11111 00 0（0xF8）发命令；
* 11111 10 0（0xFC）读忙标志和地址；
* 11111 01 0（0xFA）写RAM数据；
* 11111 11 0（0xFE）读RAM数据；
*
******************************************************************************************/
void WriteLcdCmd(Uint16 data)   //lcd 命令写
{
    Uint16 tmp1,tmp2;
    CS_1;
    delay_loop(40);
    tmp1= (data&0xF0);           //高四位
    tmp2= ((data&0x0F)<<4);      //低四位左移
    // 命令传输时序:1111 1000  D7-D4 0000    D3-D0  0000
    WriteByte(0xF8);      //11111 00 0（0xF8）发命令；
    WriteByte(tmp1);
    WriteByte(tmp2);
    delay_loop(40);
    CS_0;
    execute_72us;
}


/******************************************************************************************
* 函数名称    ：WriteLcdData；写8位数据到LCD，本函数不考虑发送地址
* 输入：Uint16 data：8位数据，
******************************************************************************************/
void WriteLcdData(Uint16 data)
{
    Uint16 tmp1,tmp2;
    CS_1;
    delay_loop(40);
    tmp1= (data&0xF0);           //高四位
    tmp2= ((data&0x0F)<<4);      //低四位左移

    // 数据传输时序:1111 1010  D7-D4 0000    D3-D0  0000
    WriteByte(0xFA);     //11111 01 0（0xFA）写RAM数据；
    WriteByte(tmp1);
    WriteByte(tmp2);
    delay_loop(40);
    CS_0;
    execute_72us;
}

/******************************************************************************************
* 函数名称    ：InitLcdFun：初始化LCD接口功能
******************************************************************************************/
void InitLcdFun( void )
{
     WriteLcdCmd(0x30);      //8BitMCU,基本指令集合
     WriteLcdCmd(0x03);      //AC归0,不改变DDRAM内容
     WriteLcdCmd(0x0C);      //显示ON,游标OFF,游标位反白OFF
     //WriteLcdCmd(0x0F);      //显示ON,游标ON,游标位反白ON
     WriteLcdCmd(0x01);      //清屏,AC归0
     WriteLcdCmd(0x06);      //写入时,游标右移动
}

/******************************************************************************************
* 函数名称    ：LcdClearTxt
* 功能描述    ：清除全部字符，光标回到（0,0）处，（用空白字符填充文本区）
******************************************************************************************/
void LcdClearTxt( void )
{
     uchar i;
     WriteLcdCmd(0x30);      //8BitMCU,基本指令集合
     WriteLcdCmd(0x80);      //AC归起始位

     for(i=0;i<64;i++)
         WriteLcdData(0x20);
}

/******************************************************************************************
* 函数名称    ：LcdClearBmp
* 功能描述    ：图形区清RAM函数
******************************************************************************************/
void LcdClearBmp( void )
{
     uchar i,j;

     WriteLcdCmd(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     WriteLcdCmd(0x36);      //绘图ON,基本指令集里面36H不能开绘图

     for(i=0;i<32;i++)            //12864实际为256x32
     {
           WriteLcdCmd(0x80|i);      //行位置
           WriteLcdCmd(0x80);        //列位置

           for(j=0;j<32;j++)            //256/8=32 byte
                WriteLcdData(0);
     }
}


void DispCHN(uchar row,uchar col,uchar *puts,Uint16 nums)
{
    Uint16 i=0;
    Uint16 temp=0;
    Uint16 temp_L=0;
    Uint16 temp_H=0;

    WriteLcdCmd(BASIC_SET);      //8BitMCU,基本指令集合
    WriteLcdCmd(AC_TABLE[8*row+col]);      //起始位置

    for (i=0;i<nums;i++)   //从第一个汉字开始
    {
        temp=*puts;
        temp_L=temp&0x00FF;
        temp_H=(temp&0xFF00)>>8;

        if(col==8)            //判断换行,若不判断,则自动从第一行到第三行
        {
             col=0;
             row++;
        }
        if(row==4) row=0;      //一屏显示完,回到屏左上角

        WriteLcdCmd(AC_TABLE[8*row+col]);

        WriteLcdData(temp_H);  //一个汉字要写两次,先发送高位
        WriteLcdData(temp_L);

        puts++;
        col++;
     }
}

/******************************************************************************************
* 函数名称    ：DispENG:显示英语字符或数字串
* 显示起始位置：
* uchar row：行号：0-3
* uchar col：列号：0-7
* 字符指针：uchar *puts
* 字符数：uchar nums，不超过64个，奇数个字符时，自动加一个空格
******************************************************************************************/
void DispENG(uchar row,uchar col,uchar *puts,Uint16 nums)
{
    Uint16 i=0;
    Uint16 odd=0;          // 奇数个字符标志

    if ((nums%2)==1)
    {
        odd=1;
        nums=(nums-1)/2;
    }
    else
    {
        nums=nums/2;
    }

    WriteLcdCmd(0x30);      //8BitMCU,基本指令集合
    WriteLcdCmd(AC_TABLE[8*row+col]);      //起始位置

    for (i=0;i<nums;i++)     //从第一个字符开始
    {
        if(col==8)            //判断换行,若不判断,则自动从第一行到第三行
        {
             col=0;
             row++;
        }
        if(row==4) row=0;      //一屏显示完,回到屏左上角

        WriteLcdCmd(AC_TABLE[8*row+col]);
        WriteLcdData(*puts);   //发送8位数据编码
        puts++;
        WriteLcdData(*puts);   //发送8位数据编码
        puts++;
        col++;
     }
    if (odd==1)
    {
        if(col==8)            //判断换行,若不判断,则自动从第一行到第三行
        {
             col=0;
             row++;
        }
        if(row==4) row=0;      //一屏显示完,回到屏左上角

        WriteLcdCmd(AC_TABLE[8*row+col]);
        WriteLcdData(*puts);   //发送8位数据编码
        WriteLcdData(0x20);    //0x20空格编码
    }
}

/******************************************************************************************
* 底层函数，在DispFig中调用
* 函数名称：DispFigData:在指定位置显示一个16位的像素，数据横排，位顺序左高右低，1表示亮
* 8位的像素按低8位使用，在高8位显示，需选左移8位
* uchar,row：指定起始行，0~63
* uchar,col：指定起始列，0~7
*******************************************************************************************/

void DispFigData(uchar row, uchar col, uchar *figdata)
{
    uchar row_lcd=0;
    uchar col_lcd=0;

    WriteLcdCmd(0x34);      //8Bit扩充指令集,即使是36H也要写两次
    WriteLcdCmd(0x36);      //绘图ON,基本指令集里面36H不能开绘图

    //判断在LCD的显示位置
    if (row>48)
    {
         row_lcd=row-32;
         col_lcd=AC_TABLE[24]+col;  //AC_TABLE[24]=0x98
    }
    else if (row>32)
    {
         row_lcd=row-32;
         col_lcd=AC_TABLE[16]+col;     // AC_TABLE[16]=0x88
    }
    else if (row>16)
    {
         row_lcd=row;
         col_lcd=AC_TABLE[8]+col;          // AC_TABLE[8]=0x90
    }
    else
    {
         row_lcd=row;
         col_lcd=AC_TABLE[0]+col;           // AC_TABLE[0]=0x80
    }
    WriteLcdCmd(AC_TABLE[0]|row_lcd);      //行位置
    WriteLcdCmd(col_lcd);           //列起始位置， 列位置自动增加

    WriteLcdData((*figdata&0xFF00)>>8);
    WriteLcdData(*figdata&0x00FF);
}

/******************************************************************************************
* 函数名称    ：DispFig:显示128x64图片，横向取模，字节正序，16位数据格式（数据横排，8个像素对应一个字节，位顺序左高右低）
*         亮暗方式：0x55的图形为：0101  0101，0表示暗，1表示亮。
******************************************************************************************/
void DispFig(uchar *fig)
{
    for (i=0;i<64;i++)
    {
      for (j=0;j<8;j++)
      {
          DispFigData(i,j,fig+i*8+j);
      }
    }
}

/******************************************************************************************
* 函数名称    ：DispFigTest:按顺序产生16位点阵数据，放在fig[]中，并显示。
* fig[]为64行、8列，每个数据16位，共512单元
* 显示128x64图片，横向取模，字节正序，16位数据格式
******************************************************************************************/
void DispFigTest(void)
{
    for (i=0;i<64;i++)
    {
      for (j=0;j<8;j++)
      {
         fig[i*8+j]=i*8+j;
      }

    }

    for (i=0;i<64;i++)
    {
      for (j=0;j<8;j++)
      {
          DispFigData(i,j,fig+i*8+j);
      }
    }
}

/******************************************************************************************
* 函数名称    ：DispFig:显示128x64图片，横向取模，字节正序，16位数据格式,上下屏数据结构
******************************************************************************************/
void DispFigUd(uchar *fig)
{

    for (i=0;i<32;i++)
    {
      for (j=0;j<16;j++)
      {
          DispFigData(i,j,fig+i*16+j);
      }
    }
}

/******************************************************************************************
* 函数名称    ：DispFig8:显示128x64图片，横向取模，字节正序，8位数据格式（数据横排，8个像素对应一个字节，位顺序左高右低）
*         亮暗方式：0x55的图形为：0101  0101，0表示暗，1表示亮。
******************************************************************************************/
void DispFig8(uchar *fig)
{
     uchar i,j;
     WriteLcdCmd(0x34);      //8Bit扩充指令集,即使是36H也要写两次
     WriteLcdCmd(0x36);      //绘图ON,基本指令集里面36H不能开绘图

     //上屏
     for(i=0;i<32;i++)            //第1行--->第32行
     {
           WriteLcdCmd(0x80|i);      //行位置
           WriteLcdCmd(0x80);        //列起始位置， 列位置自动增加
           for(j=0;j<8;j++)           //第1列--->第8列
           {
              WriteLcdData(*fig); fig++;
              WriteLcdData(*fig); fig++;
           }
      }

     //下屏
     for(i=0;i<32;i++)            //第1行--->第32行
     {
           WriteLcdCmd(0x80|i);      //行位置
           WriteLcdCmd(0x88);        //列起始位置， 列位置自动增加
           for(j=0;j<8;j++)           //第1列--->第8列
           {
              WriteLcdData(*fig); fig++;
              WriteLcdData(*fig); fig++;
           }
      }

}

/******************************************************************************************
* 函数名称    ：Delay，时间单位：ms
******************************************************************************************/
void Delay(Uint16 MS)
{
     uchar us,usn;
     while(MS!=0)            //for 12M
      {
         usn = 2;
         while(usn!=0)
          {
            us = 0xf5;
            while (us!=0)
             {
               us--;
             };
            usn--;
           }
         MS--;
      }
}

/******************************************************************************************
* 函数名称    ：LcdDelay：LCD延迟函数：时间待定
******************************************************************************************/
void LcdDelay(Uint16 Second , Uint16 MS100)
{                              //输入精确到0.1S,是用","
     Uint16 i;
     for(i=0;i<Second*100+MS100*10;i++)
     {
           if(flag==0)
           {
                 Delay(20);
                 while(flag==0)
                   {
                    Delay(20);
                   }
                 break;
           }
           else Delay(10);
     }
}

/******************************************************************************************
* 函数名称    ：delay_loop
******************************************************************************************/
//void delay_loop(Uint32 x)
//{
//    Uint32      i;
//    for (i = 0; i < x; i++) {}
//}

//// 按行点阵方向生成的的数据，上下屏结构
//uchar fig1[]={
//0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1000,0x0000,0x703F,0xFC04,0x0003,0x10F0,0x3FF9,0x01FE,0x642F,0x8818,
//0x0000,0x0000,0x1000,0x0000,0x0000,0x0200,0x3000,0x0000,0x707F,0xFC06,0x2003,0x90D0,0x3FFF,0x0100,0x000F,0xC0B8,
//0x0000,0x0000,0x3800,0x0000,0x0000,0x0601,0xF000,0x0000,0x607F,0xFE06,0x6003,0x70D0,0x3B3A,0x0000,0x000F,0xFFF0,
//0x0000,0x0C00,0x3800,0x0000,0x0000,0x0E0F,0xF000,0x0000,0x60FF,0xFF04,0x4001,0x7000,0x7B00,0x0000,0x000F,0xBFE0,
//0x0000,0x0ED8,0x3800,0x0000,0x0000,0x0F1C,0x7000,0x0000,0x60FF,0xFF04,0x6000,0xF050,0x3B80,0x3FF0,0x0007,0xBF80,
//0x0000,0x0FFC,0xFC00,0x0000,0x0000,0x1FFC,0xE000,0x0000,0x607F,0xFE07,0xE000,0x0030,0x3B81,0xF838,0x0007,0xFF08,
//0x0000,0x07FF,0xFC00,0x0000,0x0000,0x1FFC,0xF000,0x0000,0x607F,0xFE0D,0xC000,0x0030,0x3B03,0x801C,0x0007,0xEE18,
//0x0000,0x07FF,0xFC00,0x0000,0x0000,0x1EF9,0xFF80,0x0000,0x6013,0xFC0C,0x8000,0x0030,0x3F06,0x000C,0x0007,0xDC30,
//0x0000,0x03FF,0xFE00,0x0000,0x0000,0x3EFD,0xFFE0,0x0000,0x7007,0xFC07,0x8000,0x0010,0x1F06,0x000C,0x0007,0xB868,
//0x0000,0x03FF,0xFE00,0x0000,0x0000,0x3FFF,0xC7F8,0x0000,0x7807,0xF800,0x0000,0x0010,0x1F06,0x0004,0x0007,0x7858,
//0x0000,0x03FF,0xFFE0,0x0000,0x0000,0xFDFE,0x3C7E,0x0000,0x7E7F,0xF800,0x0000,0x0030,0x0706,0x0004,0x000F,0xF0D8,
//0x0000,0x0FFF,0xFFF8,0x0000,0x0003,0xFFFF,0xFFDF,0x0000,0x3FFF,0xF800,0x0000,0x0030,0x0302,0x0006,0x001B,0xE0B0,
//0x0000,0x7FFF,0xFFFE,0x0000,0x0007,0xCFFF,0xFFF7,0x8000,0x1FFF,0xF800,0x0000,0x0030,0x0303,0x0006,0x002F,0x81A0,
//0x0000,0xFFFF,0xFFFF,0x8000,0x000F,0x7FFF,0xFFFB,0xC000,0x07FF,0xFC00,0x0000,0x0060,0x0383,0x0006,0x00DF,0x0160,
//0x0003,0xFFFF,0xFFFF,0xC000,0x001E,0xFFFF,0xFFFE,0xE000,0x003D,0xFE00,0x0000,0x00E0,0x01C1,0x8006,0x1F7C,0x0340,
//0x0007,0xFFFF,0xFFFF,0xE000,0x003D,0xFFFF,0xFFFF,0x7000,0x000C,0xFE00,0x0000,0x01C0,0x00C0,0x8006,0x7FFC,0x0680,
//0x0003,0xFFFF,0xFFFF,0xF000,0x007B,0xFE00,0x7FFF,0xB800,0x0000,0x7E00,0x01E0,0x0380,0x00F0,0xC006,0xDBFE,0x0D80,
//0x0003,0xFFFF,0xFFFF,0xF800,0x00F7,0xFC00,0x3FFF,0xBC00,0x0000,0x7F00,0x01FF,0x8700,0x0078,0x6004,0x9DEE,0x1B00,
//0x0033,0xFFFF,0x01FF,0xFC00,0x00FF,0xF000,0x1FFF,0xDC00,0x0000,0x7F80,0x03FF,0xDE00,0x001F,0x7007,0xBE36,0x7600,
//0x0077,0xFFF0,0x001E,0x0C00,0x01C1,0xC000,0x0FFF,0xEE00,0x0000,0x1FE0,0x07FF,0xFC00,0x000F,0xF80F,0xBFF7,0xCC00,
//0x0077,0xFFE0,0x0000,0x0600,0x0180,0x0000,0x07FF,0x3E00,0x0000,0x03FF,0xFFFF,0xF000,0x0003,0xFC1F,0xC7FF,0xB800,
//0x00FF,0xFFC0,0x0000,0x0600,0x0380,0x0000,0x07FF,0xFFE0,0x0000,0x007F,0xFFFF,0xC000,0x0000,0xFFF8,0x7CCF,0x6000,
//0x01FF,0xFFC0,0x0000,0x0300,0x0300,0x1C00,0x1DFF,0xFFF0,0x0000,0x007F,0xFFFF,0xC000,0x0000,0xDFF0,0x3FFF,0xC000,
//0x01FF,0xFF80,0x0000,0x0380,0x0300,0x07FF,0xF3FE,0xF9F8,0x0000,0x007F,0xFFFF,0xC000,0x0000,0xDE60,0x07FF,0xC000,
//0x01FF,0xFF80,0x0000,0x0180,0x0300,0x000C,0x03FE,0xE078,0x0000,0x007F,0xFFE7,0xC000,0x0000,0x6D60,0x09E7,0x4000,
//0x03FF,0xFF80,0x0000,0x0180,0x0318,0x0000,0x01FF,0xC038,0x0000,0x007F,0xFF87,0xC000,0x0000,0x7FF0,0x0FF7,0x6000,
//0x03FF,0xFF80,0x0000,0x0180,0x0730,0x0000,0x01FF,0xBC18,0x0000,0x007F,0xFF07,0x8000,0x0000,0x3FF0,0x0FFF,0x6000,
//0x03FF,0xFF80,0x0000,0x26C0,0x0660,0x0000,0x09FF,0xFE08,0x0000,0x003F,0xFE06,0x0000,0x0000,0x0038,0x1FEE,0x6000,
//0x17FF,0xFF80,0x8000,0xEED0,0x06C0,0x01FF,0xF9FF,0xFE08,0x0000,0x003F,0xF804,0x0000,0x0000,0x001C,0x3F9F,0xC000,
//0x17FF,0xFF83,0x9801,0x82D0,0x0F90,0x01BF,0xE1FF,0xFE08,0x0000,0x001F,0xFC0C,0x0000,0x0000,0x000F,0xFF79,0x8000,
//0x3DFF,0xFF07,0x0C01,0x01F0,0x1F7C,0x01BD,0xFCFF,0xFE08,0x0000,0x0000,0x0000,0x0000,0x0000,0x0006,0x7EF0,0x0000,
//0x799F,0xFC0C,0x0003,0x00F0,0x1F7E,0x01FD,0xFC7F,0xDC08,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
//};


/*------------------------------------------------------------------------------
  源文件 / 文字 : E:\...\zimo3\九江学院.bmp
  宽×高（像素）: 128×64
------------------------------------------------------------------------------*/
// 单色横向取模，字节正序（数据横排，8个像素对应一个字节，位顺序左高右低）
//jju_fig8[]={
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x00,0x00,0x00,0x1C,0x00,
//0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x66,0x00,0x00,0x00,0x1C,0x00,
//0x00,0x03,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x08,0x6F,0x00,0x00,0x03,0x1C,0x00,
//0x00,0x03,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x0C,0x6F,0x00,0x00,0x07,0x0C,0x00,
//0x00,0x03,0x80,0x00,0x00,0x1E,0x00,0x00,0x00,0x0F,0xCF,0x00,0x00,0x07,0x0C,0x00,
//0x00,0x07,0x80,0x00,0x00,0x1E,0x00,0x00,0x00,0x0F,0xC7,0x80,0x00,0x0E,0x0F,0x80,
//0x00,0x07,0x80,0x00,0x00,0x1F,0x00,0x00,0x00,0x3F,0xC7,0x80,0x00,0x1E,0x0F,0xC0,
//0x00,0x07,0x80,0x00,0x00,0x1F,0x00,0x00,0x00,0x6D,0xEF,0x80,0x00,0x3E,0x0F,0xC0,
//0x00,0x07,0x80,0x00,0x00,0x1F,0x00,0x00,0x00,0x2C,0xFF,0x80,0x00,0x7C,0x1F,0xC0,
//0x00,0x07,0x8E,0x00,0x00,0x1F,0x00,0x00,0x00,0x2D,0x3F,0x00,0x00,0x7C,0x9F,0x80,
//0x00,0x07,0x9E,0x00,0x00,0x0F,0x00,0x00,0x00,0x2F,0x6F,0x00,0x00,0xFC,0x9B,0x00,
//0x00,0x0F,0xFF,0x00,0x00,0x07,0x00,0x00,0x00,0x2F,0xEE,0x00,0x01,0xFC,0xF3,0x00,
//0x00,0x0F,0xFF,0x00,0x00,0x07,0x00,0xE0,0x00,0x3F,0xCE,0x00,0x01,0x98,0xE2,0x00,
//0x00,0x0F,0xFF,0x00,0x00,0x03,0x01,0xE0,0x00,0x3E,0xFE,0x00,0x00,0x18,0xC2,0x00,
//0x00,0x1F,0xFF,0x00,0x00,0x20,0x07,0xE0,0x00,0x3D,0xFC,0x00,0x00,0x18,0xC4,0x00,
//0x00,0x1F,0xFE,0x00,0x00,0x20,0x1F,0xE0,0x00,0xBF,0x3E,0x80,0x00,0x39,0xCC,0x00,
//0x00,0x3F,0xFE,0x00,0x00,0x30,0x3F,0xC0,0x00,0x9F,0xFF,0xE0,0x00,0xB9,0xCC,0x00,
//0x00,0x7F,0xFE,0x00,0x00,0x70,0x7F,0xC0,0x01,0x9F,0xFF,0xF0,0x00,0x99,0x9C,0x00,
//0x00,0xFF,0xFE,0x00,0x00,0x70,0xFF,0xC0,0x01,0xBF,0xFF,0xF0,0x01,0x9D,0xF8,0x00,
//0x01,0xFF,0xFC,0x00,0x00,0x70,0xFF,0xC0,0x03,0xFF,0x40,0xF0,0x01,0x8D,0xB3,0x00,
//0x00,0xFF,0xFC,0x00,0x00,0xF2,0xFF,0x80,0x03,0xF8,0x00,0xF0,0x00,0xCC,0x27,0x00,
//0x00,0xFF,0xFC,0x00,0x00,0xF2,0xFF,0x00,0x01,0xF0,0x39,0xE0,0x00,0xCC,0x0F,0x00,
//0x00,0xFE,0x7C,0x00,0x00,0xF2,0xFE,0x00,0x03,0xE0,0xFD,0xC0,0x00,0xFC,0x1E,0x00,
//0x00,0xFE,0x78,0x20,0x00,0xF6,0xFC,0x00,0x03,0xC7,0xFF,0x80,0x00,0xFC,0x1E,0x00,
//0x00,0x7C,0x78,0x20,0x00,0xF6,0xFC,0x00,0x03,0xCF,0xFC,0x00,0x00,0xFC,0x3C,0x40,
//0x00,0x7C,0x78,0x20,0x00,0xFE,0x7B,0x80,0x03,0xDF,0x98,0x00,0x00,0xFC,0x7C,0x40,
//0x00,0x7C,0x78,0x30,0x00,0xFC,0x7F,0xC0,0x03,0x9E,0x18,0x00,0x00,0xD9,0xFC,0x60,
//0x00,0x3C,0x70,0x30,0x00,0x7E,0x7F,0xC0,0x03,0x98,0x76,0x00,0x00,0xC3,0xFC,0x60,
//0x00,0x7C,0x70,0x30,0x00,0xFE,0xFF,0xC0,0x01,0x80,0xFE,0x00,0x00,0xC7,0xFC,0x20,
//0x00,0x78,0xF0,0x20,0x00,0xFF,0xFF,0xC0,0x01,0x00,0xFF,0x00,0x00,0xC7,0xFC,0x20,
//0x00,0xF8,0x70,0x70,0x00,0xFF,0xFF,0x80,0x00,0x01,0xFF,0x00,0x00,0xC3,0xFC,0x20,
//0x04,0xF8,0xF0,0x70,0x00,0xFD,0xFF,0x80,0x00,0x07,0xFE,0x00,0x00,0xC1,0xD8,0x20,
//0x04,0xF8,0xF0,0x70,0x00,0x79,0xFF,0x80,0x00,0x1F,0xFE,0x00,0x00,0xC0,0xDC,0x30,
//0x05,0xF8,0xF0,0x70,0x00,0xF9,0xFF,0x80,0x00,0x1F,0xFC,0x00,0x00,0xC0,0xDC,0x30,
//0x07,0xF0,0x60,0x70,0x00,0x79,0xE0,0x00,0x00,0x1F,0xF0,0x00,0x00,0xC0,0xDC,0x30,
//0x0F,0xF0,0xF0,0x70,0x00,0xF1,0xC0,0x00,0x00,0x1C,0xF0,0x00,0x00,0xC1,0xDC,0x70,
//0x0F,0xF0,0x70,0xF0,0x00,0xF1,0x80,0x00,0x00,0x0C,0xF0,0x00,0x00,0xC1,0xDC,0x70,
//0x0F,0xE0,0x7F,0xF0,0x00,0xF0,0x00,0x00,0x00,0x00,0xF0,0x00,0x00,0xC1,0x9D,0xF0,
//0x1F,0xE0,0x7F,0xF0,0x00,0x70,0x00,0x00,0x00,0x00,0xF0,0x00,0x00,0xC3,0x9F,0xF0,
//0x1F,0xC0,0x7F,0xF0,0x00,0x60,0x00,0x00,0x00,0x00,0xF0,0x00,0x01,0xE3,0x1F,0xF0,
//0x1F,0x80,0x7F,0xF0,0x00,0x00,0x00,0x00,0x00,0x10,0xF0,0x00,0x01,0xE7,0x1F,0xF0,
//0x1F,0x80,0x3F,0xE0,0x00,0x00,0x00,0x00,0x00,0x1F,0xE0,0x00,0x01,0xEF,0x0F,0xE0,
//0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1F,0xE0,0x00,0x01,0xEF,0x0F,0xE0,
//0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0F,0xE0,0x00,0x00,0xEE,0x0F,0x00,
//0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0xC0,0x00,0x00,0xC0,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x80,0x00,0x00,0x40,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
//0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
//};

