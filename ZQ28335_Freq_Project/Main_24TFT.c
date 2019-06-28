/*
 * Main_24TFT.c
 *
 *  Created on: 2019��5��25��
 *      Author: sunxiaobing
 */
#include "DSP28x_Project.h"
#include "LCD_24TFT.h"
#include "GUI.h"
#include "Delay.h"
#include "KEY.h"
#include "LED_M.h"               // LED״̬�궨��
#include "LED.h"
static unsigned int Precent=0;                  //����Ϊ��̬����Ҫ��Ϊ�˱�����һ�β���ֵ
unsigned int yback=0;                           //���ڴ��ǰһ����ʾ���Y����
extern unsigned char MenuFlag ;         //�˵�״̬��־λ
extern unsigned char SubSel_Flag ;      //�Ӳ˵�ѡ���־

extern unsigned char Sub_Flag1;          //�Ӳ˵�1��ֵ�����л���־λ
extern unsigned char Sub_Flag2;          //�Ӳ˵�2��ֵ�����л���־λ
extern unsigned char Sub_Flag3;          //�Ӳ˵�3��ֵ�����л���־λ
extern unsigned char Sub_Flag4;          //�Ӳ˵�4��ֵ�����л���־λ


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
* ���ƣ�void Show_Wave(unsigned int x0,unsigned int y0,unsigned int *Num,unsigned int Lnum,unsigned int bcolor)
* ���ܣ����������е����ݻ�ͼ��ʾ
* ��ڲ����� x0       ԭ�������
*           y0       ԭ��������
*           Num      ��������  12λ��������
*           Lnum     ���鳤��
*           bcolor      ������ɫ
*
* ���ڲ�������
****************************************************************************/
void Show_Wave(unsigned int x0,unsigned int y0,unsigned int *Num,unsigned int Lnum)
{
    unsigned int i;
    for(i=0;i<Lnum;i++)
    {
        yback = y0-(0.8*Precent);                               //Ϊ�˲���ƽ����Ҫ����ǰһ�β���ֵ��Ӧ�������
        Precent = ((Uint32)(Num[i])*100)/0xFFF;                   //����ٷֱ�
        Gui_DrawPoint(x0+i,y0-(0.8*Precent),RED);              //�ѵ�ѹֵ������������򣬲��ðٷֱ��㷨
        Gui_DrawLine(x0+i-1,yback,x0+i,y0-(0.8*Precent),RED);  //��ÿ����֮����ֱ�����ӣ��γɲ���
        Delay_ms(200);
    }

}
void main()
{
    InitSysCtrl();

    TFT_Gpio_Init();            //TFT��ʾģ��˿�����
    InitKeyGpio();
    InitLedGpio();

    DINT;
    InitPieCtrl();

    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    Lcd_Init();              //TFT��ʼ��
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
        GetKey();  // �õ�����Key��ֵ
        KeyFun(); //  ��������
//        DSP28x_usDelay(100000);  // ���μ��֮����0.1s������ϣ�����׼ȷ��⣬����ִ�������������ӳ١�
    }

}




