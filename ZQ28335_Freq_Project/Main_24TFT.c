/*
 * Main_24TFT.c
 *
 *  Created on: 2019年5月25日
 *      Author: sunxiaobing
 */
#include "DSP28x_Project.h"
#include "LCD_24TFT.h"
#include "GUI.h"
#include "Delay.h"
#include "KEY.h"
#include "LED_M.h"               // LED状态宏定义
#include "LED.h"
static unsigned int Precent=0;                  //设置为静态很重要，为了保留上一次采样值
unsigned int yback=0;                           //用于存放前一次显示点的Y坐标
extern unsigned char MenuFlag ;         //菜单状态标志位
extern unsigned char SubSel_Flag ;      //子菜单选择标志

extern unsigned char Sub_Flag1;          //子菜单1数值输入切换标志位
extern unsigned char Sub_Flag2;          //子菜单2数值输入切换标志位
extern unsigned char Sub_Flag3;          //子菜单3数值输入切换标志位
extern unsigned char Sub_Flag4;          //子菜单4数值输入切换标志位


Uint16 Wave[]={0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,0x7ff,
               0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,0x000,
               0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff,0xfff};
Uint16 Sin[]={
              2047, 2446, 2831, 3185, 3495, 3749, 3939, 4055,
              4095, 4055, 3939, 3749, 3495, 3185, 2831, 2446,
              2047, 1648, 1263,  909,  599,  345,  155,   39,
                 0,   39,  155,  345,  599,  909, 1263, 1648,
};
/****************************************************************************
* 名称：void Show_Wave(unsigned int x0,unsigned int y0,unsigned int *Num,unsigned int Lnum,unsigned int bcolor)
* 功能：根据数组中的数据绘图显示
* 入口参数： x0       原点横坐标
*           y0       原点纵坐标
*           Num      数组名称  12位数据类型
*           Lnum     数组长度
*           bcolor      背景颜色
*
* 出口参数：无
****************************************************************************/
void Show_Wave(unsigned int x0,unsigned int y0,unsigned int *Num,unsigned int Lnum)
{
    unsigned int i;
    for(i=0;i<Lnum;i++)
    {
        yback = y0-(0.8*Precent);                               //为了波形平滑，要保留前一次采样值对应的坐标点
        Precent = ((Uint32)(Num[i])*100)/0xFFF;                   //算出百分比
        Gui_DrawPoint(x0+i,y0-(0.8*Precent),RED);              //把电压值打点在坐标区域，采用百分比算法
        Gui_DrawLine(x0+i-1,yback,x0+i,y0-(0.8*Precent),RED);  //将每个点之间用直线连接，形成波形
        Delay_ms(200);
    }

}
void main()
{
    InitSysCtrl();

    TFT_Gpio_Init();            //TFT显示模块端口配置
    InitKeyGpio();
    InitLedGpio();

    DINT;
    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    Lcd_Init();              //TFT初始化
    Lcd_Clear(WHITE );       Delay_ms(500);
    Draw_Mainmenu();
   // Draw_Submenu3();
  //  WavePicture();
    Delay_ms(500);
    data_init();
   // Show_Wave(20,100,Wave,48);
   // Show_Wave(20,220,Sin,32);
    while(1)
    {
        GetKey();  // 得到按键Key的值
        KeyFun(); //  按键功能
//        DSP28x_usDelay(100000);  // 两次检测之间需0.1s间隔以上，才能准确检测，可以执行其他代码作延迟。
    }

}




