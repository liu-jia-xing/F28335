/*
 * LCD_24TFT.c
 *
 *  Created on: 2019年5月25日
 *      Author: sunxiaobing
 */
#include "LCD_24TFT.h"
#include "DSP28x_Project.h"
#include "Delay.h"

void TFT_Gpio_Init(void)
{
    EALLOW;
    //CS  GPIO57 Init=0
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO57 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO57 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 0;

    //RESET GPIO55  Init=0
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO55 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO55 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 0;

    //DC GPIO53 Init=0
    GpioCtrlRegs.GPBPUD.bit.GPIO53 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO53 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO53 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO53 = 0;

    //SDI GPIO21 Init=0
    GpioCtrlRegs.GPBPUD.bit.GPIO51 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO51 = 1;
    GpioDataRegs.GPBCLEAR.bit.GPIO51 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO51 = 0;

    //SCK GPIO30 Init=0
    GpioCtrlRegs.GPBPUD.bit.GPIO49 = 0;
    GpioCtrlRegs.GPBDIR.bit.GPIO49 = 1;
    GpioDataRegs.GPBSET.bit.GPIO49 = 1;
    GpioCtrlRegs.GPBMUX2.bit.GPIO49 = 0;

    EDIS;
}
/****************************************************************************
* 名    称：void  SPIv_WriteData(u8 Data)
* 功    能：STM32_模拟SPI写一个字节数据底层函数
* 入口参数：Data
* 出口参数：无
* 说    明：STM32_模拟SPI读写一个字节数据底层函数
****************************************************************************/
void  SPIv_WriteData(u8 Data)
{
    unsigned char i=0;
    for(i=0;i<8;i++)
    {
        SCLK_0;
        if(Data&0x80)
            SDI_1;
        else
            SDI_0;

        SCLK_1;
        Data<<=1;
    }
    SCLK_0;
}
/****************************************************************************
* 名    称：Lcd_WriteIndex(u8 Index)
* 功    能：向液晶屏写一个8位指令
* 入口参数：Index   寄存器地址
* 出口参数：无
* 说    明：调用前需先选中控制器，内部函数
****************************************************************************/
void Lcd_WriteIndex(u8 Index)
{
    CS_0;
    DC_0;
    SPIv_WriteData(Index);
    CS_1;
}
/****************************************************************************
* 名    称：Lcd_WriteData(u8 Data)
* 功    能：向液晶屏写一个8位数据
* 入口参数：dat     寄存器数据
* 出口参数：无
* 说    明：向控制器指定地址写入数据，内部函数
****************************************************************************/
void Lcd_WriteData(u8 Data)
{
    CS_0;
    DC_1;
    SPIv_WriteData(Data);
    CS_1;
}
/****************************************************************************
* 名    称：void Lcd_WriteData_16Bit(u16 Data)
* 功    能：向液晶屏写一个16位数据
* 入口参数：Data
* 出口参数：无
* 说    明：向控制器指定地址写入一个16位数据
****************************************************************************/
void Lcd_WriteData_16Bit(u16 Data)
{
    Lcd_WriteData(Data>>8);
    Lcd_WriteData(Data);
}

/****************************************************************************
* 名    称：void LCD_WriteReg(u8 Index,u16 Data)
* 功    能：写寄存器数据
* 入口参数：Index,Data
* 出口参数：无
* 说    明：本函数为组合函数，向Index地址的寄存器写入Data值
****************************************************************************/
void LCD_WriteReg(u8 Index,u16 Data)
{
    Lcd_WriteIndex(Index);
    Lcd_WriteData_16Bit(Data);
}

/****************************************************************************
* 名    称：void Lcd_Reset(void)
* 功    能：液晶硬复位函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化前需执行一次复位操作
****************************************************************************/
void Lcd_Reset(void)
{
    RESET_0;
    Delay_ms(10);
    RESET_1;
    Delay_ms(10);
}
/****************************************************************************
* 名    称：void Lcd_Init(void)
* 功    能：液晶初始化函数
* 入口参数：无
* 出口参数：无
* 说    明：液晶初始化_ILI9225_176X220
****************************************************************************/
void Lcd_Init(void)
{
    Lcd_Reset(); //Reset before LCD Init.

    //2.2inch TM2.2-G2.2 Init 20171020
        Lcd_WriteIndex(0x11);
        Lcd_WriteData(0x00);

        Lcd_WriteIndex(0xCF);
        Lcd_WriteData(0X00);
        Lcd_WriteData(0XC1);
        Lcd_WriteData(0X30);

        Lcd_WriteIndex(0xED);
        Lcd_WriteData(0X64);
        Lcd_WriteData(0X03);
        Lcd_WriteData(0X12);
        Lcd_WriteData(0X81);

        Lcd_WriteIndex(0xE8);
        Lcd_WriteData(0X85);
        Lcd_WriteData(0X11);
        Lcd_WriteData(0X78);

        Lcd_WriteIndex(0xF6);
        Lcd_WriteData(0X01);
        Lcd_WriteData(0X30);
        Lcd_WriteData(0X00);

        Lcd_WriteIndex(0xCB);
        Lcd_WriteData(0X39);
        Lcd_WriteData(0X2C);
        Lcd_WriteData(0X00);
        Lcd_WriteData(0X34);
        Lcd_WriteData(0X05);

        Lcd_WriteIndex(0xF7);
        Lcd_WriteData(0X20);

        Lcd_WriteIndex(0xEA);
        Lcd_WriteData(0X00);
        Lcd_WriteData(0X00);

        Lcd_WriteIndex(0xC0);
        Lcd_WriteData(0X20);

        Lcd_WriteIndex(0xC1);
        Lcd_WriteData(0X11);

        Lcd_WriteIndex(0xC5);
        Lcd_WriteData(0X31);
        Lcd_WriteData(0X3C);

        Lcd_WriteIndex(0xC7);
        Lcd_WriteData(0XA9);

        Lcd_WriteIndex(0x3A);
        Lcd_WriteData(0X55);

      Lcd_WriteIndex(0x36);
        #if USE_HORIZONTAL
             Lcd_WriteData(0xE8);//横屏参数
        #else
             Lcd_WriteData(0x48);//竖屏参数
        #endif

        Lcd_WriteIndex(0xB1);
        Lcd_WriteData(0X00);
        Lcd_WriteData(0X18);

        Lcd_WriteIndex(0xB4);
        Lcd_WriteData(0X00);
        Lcd_WriteData(0X00);

        Lcd_WriteIndex(0xF2);
        Lcd_WriteData(0X00);

        Lcd_WriteIndex(0x26);
        Lcd_WriteData(0X01);

        Lcd_WriteIndex(0xE0);
        Lcd_WriteData(0X0F);
        Lcd_WriteData(0X17);
        Lcd_WriteData(0X14);
        Lcd_WriteData(0X09);
        Lcd_WriteData(0X0C);
        Lcd_WriteData(0X06);
        Lcd_WriteData(0X43);
        Lcd_WriteData(0X75);
        Lcd_WriteData(0X36);
        Lcd_WriteData(0X08);
        Lcd_WriteData(0X13);
        Lcd_WriteData(0X05);
        Lcd_WriteData(0X10);
        Lcd_WriteData(0X0B);
        Lcd_WriteData(0X08);


        Lcd_WriteIndex(0xE1);
        Lcd_WriteData(0X00);
        Lcd_WriteData(0X1F);
        Lcd_WriteData(0X23);
        Lcd_WriteData(0X03);
        Lcd_WriteData(0X0E);
        Lcd_WriteData(0X04);
        Lcd_WriteData(0X39);
        Lcd_WriteData(0X25);
        Lcd_WriteData(0X4D);
        Lcd_WriteData(0X06);
        Lcd_WriteData(0X0D);
        Lcd_WriteData(0X0B);
        Lcd_WriteData(0X33);
        Lcd_WriteData(0X37);
        Lcd_WriteData(0X0F);

        Lcd_WriteIndex(0x29);
}
/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd_SetXY(u16 Xpos, u16 Ypos)
{
    Lcd_WriteIndex(0x2A);
    Lcd_WriteData_16Bit(Xpos);
    Lcd_WriteIndex(0x2B);
    Lcd_WriteData_16Bit(Ypos);
    Lcd_WriteIndex(0x2c);
}
/*************************************************
函数名：LCD_Set_Region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
//设置显示窗口
void Lcd_SetRegion(u16 xStar, u16 yStar,u16 xEnd,u16 yEnd)
{
    Lcd_WriteIndex(0x2A);
    Lcd_WriteData_16Bit(xStar);
    Lcd_WriteData_16Bit(xEnd);
    Lcd_WriteIndex(0x2B);
    Lcd_WriteData_16Bit(yStar);
    Lcd_WriteData_16Bit(yEnd);
    Lcd_WriteIndex(0x2c);
}

/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd_Clear(u16 Color)
{
    unsigned int i,m;
    Lcd_SetRegion(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
       for(i=0;i<X_MAX_PIXEL;i++)
        for(m=0;m<Y_MAX_PIXEL;m++)
        {
            Lcd_WriteData_16Bit(Color);
        }
}

/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：xy坐标和颜色数据
返回值：无
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
    Lcd_SetXY(x,y);
    Lcd_WriteData_16Bit(Data);

}




