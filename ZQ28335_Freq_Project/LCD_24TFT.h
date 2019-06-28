/*
 * LCD_24TFT.h
 *
 *  Created on: 2019年5月25日
 *      Author: sunxiaobing
 */

#ifndef LCD_24TFT_H_
#define LCD_24TFT_H_

#define u8 unsigned char
#define u16 unsigned int
/**********************************用户配置区 **********************************/
//支持横竖屏快速定义切换
#define USE_HORIZONTAL          1   //定义是否使用横屏      0,不使用.1,使用.

/*********************************屏幕物理像素设置******************************/
#define LCD_X_SIZE          240
#define LCD_Y_SIZE          320

#if USE_HORIZONTAL//如果定义了横屏
#define X_MAX_PIXEL         LCD_Y_SIZE
#define Y_MAX_PIXEL         LCD_X_SIZE
#else
#define X_MAX_PIXEL         LCD_X_SIZE
#define Y_MAX_PIXEL         LCD_Y_SIZE
#endif
/**********************************端口定义************************************/
#define SCLK_0     GpioDataRegs.GPBCLEAR.bit.GPIO57=1   //SCLK_0
#define SCLK_1     GpioDataRegs.GPBSET.bit.GPIO57=1     //SCLK_1

#define SDI_0      GpioDataRegs.GPBCLEAR.bit.GPIO55=1      //SDI
#define SDI_1      GpioDataRegs.GPBSET.bit.GPIO55=1

#define DC_0       GpioDataRegs.GPBCLEAR.bit.GPIO53=1   //DC_0
#define DC_1       GpioDataRegs.GPBSET.bit.GPIO53=1   //DC_1

#define RESET_0    GpioDataRegs.GPBCLEAR.bit.GPIO51=1    //RESET0
#define RESET_1    GpioDataRegs.GPBSET.bit.GPIO51=1      //RESET1

#define CS_0       GpioDataRegs.GPBCLEAR.bit.GPIO49=1   //CS_0
#define CS_1       GpioDataRegs.GPBSET.bit.GPIO49=1     //CS_1

/**********************************画笔颜色定义**********************************/
//1

//#define RED     0xf800
//#define GREEN   0x07e0
//#define BLUE    0x001f
//#define WHITE   0xffff
//#define BLACK   0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D
#define GRAY1   0x8410
#define GRAY2   0x4208
//2
#define WHITE            0xFFFF   //白色
#define BLACK            0x0000   //黑色
#define BLUE             0x001F   //蓝色
#define RED              0xF800   //红色
#define MAGENTA          0xF81F   //品红色
#define GREEN            0x07E0   //绿色
#define CYAN             0x7FFF   //蓝绿色
#define YELLOW           0xFFE0   //黄色
#define BROWN            0XBC40   //棕色
#define BRRED            0XFC07   //棕红色
#define GRAY             0X8430   //灰色
#define DARKBLUE         0X01CF   //深蓝色
#define LIGHTBLUE        0X7D7C   //浅蓝色
#define GRAYBLUE         0X5458   //灰蓝色
//以上三色为PANEL的颜色
#define LIGHTGREEN       0X841F   //浅绿色
#define LGRAY            0XC618   //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651   //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12   //浅棕蓝色(选择条目的反色)

/*****************************************函数声明*******************************/
void TFT_Gpio_Init(void);                     //端口定义
void SPIv_WriteData(u8 Data);                //模拟SPI写一个字节

void Lcd_WriteIndex(u8 Index);        //向TFT写单字节指令
void Lcd_WriteData(u8 Data);         //向TFT写单字节8位数据
void Lcd_WriteData_16Bit(u16 Data);    //向TFT写16位数据
void LCD_WriteReg(u8 Index,u16 Data);   //写寄存器数据
void Lcd_Reset(void);                             //TFT系统复位
void Lcd_Init(void);                              //TFT系统初始化

void Lcd_SetXY(u16 Xpos, u16 Ypos);       //屏幕定点
void Lcd_SetRegion(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd); //屏幕划定区域
void Lcd_Clear(u16 Color);                             //TFT整体清屏

void Gui_DrawPoint(u16 x,u16 y,u16 Data);     //画点函数

#endif /* LCD_24TFT_H_ */

