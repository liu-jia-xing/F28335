/*
 * LCD12864.h
 *
 *  Created on: 2018-8-4
 *      Author: Administrator
 */

#ifndef LCD12864_H_
#define LCD12864_H_

#define uchar unsigned char
#define uint  Uint16


// 宏定义
#define uchar unsigned char

#define  BASIC_SET      0x30           //基本指令集  00110000
#define  EXTEND_SET     0x34           //扩展指令集  00110100
#define  DRAW_ON        0x36           //绘图显示开  00110110
#define  DRAW_OFF       0x34           //绘图显示关  00110100
#define  DISP_ON        0x0F           //显示ON,游标ON,游标位反白ON 00001111

#define  execute_72us   Delay_loop(400)     //执行时间
#define  SCLK_quarter   Delay_loop(40)       //四分之一SCLK时钟 SCLK是LCD串行通信时钟
#define  SCLK_half      Delay_loop(80)       //二分之一SCLK时钟 SCLK是LCD串行通信时钟

#define CS_1        GpioDataRegs.GPBDAT.bit.GPIO32=1             //GPIO5作为CS
#define CS_0        GpioDataRegs.GPBDAT.bit.GPIO32=0             //GPIO5作为CS

#define SCLK_1      GpioDataRegs.GPADAT.bit.GPIO19=1             //GPIO4作为SCLK
#define SCLK_0      GpioDataRegs.GPADAT.bit.GPIO19=0             //GPIO4作为SCLK

#define SDATA_OUT   GpioCtrlRegs.GPADIR.bit.GPIO18=1             //GPIO2作为SDATA
#define SDATA_IN    GpioCtrlRegs.GPADIR.bit.GPIO18=0             //GPIO2作为SDATA
#define SDATA       GpioDataRegs.GPADAT.bit.GPIO18

#define RST_1       GpioDataRegs.GPCDAT.bit.GPIO67=1             //GPIO3做RST
#define RST_0       GpioDataRegs.GPCDAT.bit.GPIO67=0             //GPIO3做RST

// 数字和英文字符,Uint16会生成16位编码，如：'0'=0x0030
extern void InitLcdGpio(void);
extern void InitLcd(void);
extern void LcdClearAll(void);
extern void TestLcd(void);
extern void TestLcd1(void);
extern void TestLcd2(void);
extern void TestLcd3(void);
extern void DispFloat(char row, char col, float a);

#endif /* LCD12864_H_ */
