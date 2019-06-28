/*
 * LCD_24TFT.c
 *
 *  Created on: 2019��5��25��
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
* ��    �ƣ�void  SPIv_WriteData(u8 Data)
* ��    �ܣ�STM32_ģ��SPIдһ���ֽ����ݵײ㺯��
* ��ڲ�����Data
* ���ڲ�������
* ˵    ����STM32_ģ��SPI��дһ���ֽ����ݵײ㺯��
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
* ��    �ƣ�Lcd_WriteIndex(u8 Index)
* ��    �ܣ���Һ����дһ��8λָ��
* ��ڲ�����Index   �Ĵ�����ַ
* ���ڲ�������
* ˵    ��������ǰ����ѡ�п��������ڲ�����
****************************************************************************/
void Lcd_WriteIndex(u8 Index)
{
    CS_0;
    DC_0;
    SPIv_WriteData(Index);
    CS_1;
}
/****************************************************************************
* ��    �ƣ�Lcd_WriteData(u8 Data)
* ��    �ܣ���Һ����дһ��8λ����
* ��ڲ�����dat     �Ĵ�������
* ���ڲ�������
* ˵    �����������ָ����ַд�����ݣ��ڲ�����
****************************************************************************/
void Lcd_WriteData(u8 Data)
{
    CS_0;
    DC_1;
    SPIv_WriteData(Data);
    CS_1;
}
/****************************************************************************
* ��    �ƣ�void Lcd_WriteData_16Bit(u16 Data)
* ��    �ܣ���Һ����дһ��16λ����
* ��ڲ�����Data
* ���ڲ�������
* ˵    �����������ָ����ַд��һ��16λ����
****************************************************************************/
void Lcd_WriteData_16Bit(u16 Data)
{
    Lcd_WriteData(Data>>8);
    Lcd_WriteData(Data);
}

/****************************************************************************
* ��    �ƣ�void LCD_WriteReg(u8 Index,u16 Data)
* ��    �ܣ�д�Ĵ�������
* ��ڲ�����Index,Data
* ���ڲ�������
* ˵    ����������Ϊ��Ϻ�������Index��ַ�ļĴ���д��Dataֵ
****************************************************************************/
void LCD_WriteReg(u8 Index,u16 Data)
{
    Lcd_WriteIndex(Index);
    Lcd_WriteData_16Bit(Data);
}

/****************************************************************************
* ��    �ƣ�void Lcd_Reset(void)
* ��    �ܣ�Һ��Ӳ��λ����
* ��ڲ�������
* ���ڲ�������
* ˵    ����Һ����ʼ��ǰ��ִ��һ�θ�λ����
****************************************************************************/
void Lcd_Reset(void)
{
    RESET_0;
    Delay_ms(10);
    RESET_1;
    Delay_ms(10);
}
/****************************************************************************
* ��    �ƣ�void Lcd_Init(void)
* ��    �ܣ�Һ����ʼ������
* ��ڲ�������
* ���ڲ�������
* ˵    ����Һ����ʼ��_ILI9225_176X220
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
             Lcd_WriteData(0xE8);//��������
        #else
             Lcd_WriteData(0x48);//��������
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
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
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
��������LCD_Set_Region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
//������ʾ����
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
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
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
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�����xy�������ɫ����
����ֵ����
*************************************************/
void Gui_DrawPoint(u16 x,u16 y,u16 Data)
{
    Lcd_SetXY(x,y);
    Lcd_WriteData_16Bit(Data);

}




