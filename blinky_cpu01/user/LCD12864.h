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


// �궨��
#define uchar unsigned char

#define  BASIC_SET      0x30           //����ָ�  00110000
#define  EXTEND_SET     0x34           //��չָ�  00110100
#define  DRAW_ON        0x36           //��ͼ��ʾ��  00110110
#define  DRAW_OFF       0x34           //��ͼ��ʾ��  00110100
#define  DISP_ON        0x0F           //��ʾON,�α�ON,�α�λ����ON 00001111

#define  execute_72us   Delay_loop(400)     //ִ��ʱ��
#define  SCLK_quarter   Delay_loop(40)       //�ķ�֮һSCLKʱ�� SCLK��LCD����ͨ��ʱ��
#define  SCLK_half      Delay_loop(80)       //����֮һSCLKʱ�� SCLK��LCD����ͨ��ʱ��

#define CS_1        GpioDataRegs.GPBDAT.bit.GPIO32=1             //GPIO5��ΪCS
#define CS_0        GpioDataRegs.GPBDAT.bit.GPIO32=0             //GPIO5��ΪCS

#define SCLK_1      GpioDataRegs.GPADAT.bit.GPIO19=1             //GPIO4��ΪSCLK
#define SCLK_0      GpioDataRegs.GPADAT.bit.GPIO19=0             //GPIO4��ΪSCLK

#define SDATA_OUT   GpioCtrlRegs.GPADIR.bit.GPIO18=1             //GPIO2��ΪSDATA
#define SDATA_IN    GpioCtrlRegs.GPADIR.bit.GPIO18=0             //GPIO2��ΪSDATA
#define SDATA       GpioDataRegs.GPADAT.bit.GPIO18

#define RST_1       GpioDataRegs.GPCDAT.bit.GPIO67=1             //GPIO3��RST
#define RST_0       GpioDataRegs.GPCDAT.bit.GPIO67=0             //GPIO3��RST

// ���ֺ�Ӣ���ַ�,Uint16������16λ���룬�磺'0'=0x0030
extern void InitLcdGpio(void);
extern void InitLcd(void);
extern void LcdClearAll(void);
extern void TestLcd(void);
extern void TestLcd1(void);
extern void TestLcd2(void);
extern void TestLcd3(void);
extern void DispFloat(char row, char col, float a);

#endif /* LCD12864_H_ */
