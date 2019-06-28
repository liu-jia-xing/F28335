/*
 * AD9959_2.h
 *
 *  Created on: 2019-5-29
 *      Author: Administrator
 */

#ifndef AD9959_2_H_
#define AD9959_2_H_

#include "DSP28x_Project.h"


extern unsigned char CSR_DATA0[1]; //使能CH0
extern unsigned char  CFTW0_DATA[4];
extern unsigned char  FR1_DATA[3];

//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR   通道选择寄存器
#define FR1_ADD   0x01   //FR1   功能寄存器1 这个寄存器被分配了三个字节。FR1用于控制芯片的工作方式。
#define FR2_ADD   0x02   //FR2   功能寄存器2 这个寄存器分配了两个字节。FR2用于控制AD9959的各种功能、特性和模式。
#define CFR_ADD   0x03   //CFR   通道功能寄存器
#define CFTW0_ADD 0x04   //CFTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPOW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR   幅度控制寄存器
#define LSRR_ADD  0x07   //LSRR  通道线性扫描寄存器
#define RDW_ADD   0x08   //RDW   通道线性向上扫描寄存器
#define FDW_ADD   0x09   //FDW   通道线性向下扫描寄存器

#define CSR 0x00               //通道选择寄存器
#define FR1 0x01               //功能寄存器1
#define FR2 0x02               //功能寄存器2
#define CFR 0x03               //通道功能寄存器
#define CFTW0 0x04             //32位通道频率转换字寄存器
#define CPOW0 0x05             //14位通道相位转换字寄存器
#define ACR 0x06               //幅度控制寄存器
#define SRR 0x07               //线性扫描定时器
#define RDW 0x08               //线性向上扫描定时器
#define FDW 0x09               //线性向下扫描定时器

#define CW1  0x0A               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW2  0x0B               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW3  0x0C               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW4  0x0D               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW5  0x0E               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW6  0x0F               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW7  0x10               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW8  0x11               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW9  0x12               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW10 0x13               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW11 0x14               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW12 0x15               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW13 0x16               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW14 0x17               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带
#define CW15 0x18               //外形寄存器(Profile Register)，用FSK,ASK和PSK的基带

//AD9959端口配置

// 控制引脚
// GPIO10--- PDC---0(default value),H有效
#define   AD9959_PWR_0   GpioDataRegs.GPACLEAR.bit.GPIO10 = 1
#define   AD9959_PWR_1   GpioDataRegs.GPASET.bit.GPIO10 = 1

// GPIO8 --- RESET---0(default value),H有效
#define   RESET_0        GpioDataRegs.GPACLEAR.bit.GPIO8 = 1
#define   RESET_1        GpioDataRegs.GPASET.bit.GPIO8 = 1

// GPIO6 --- UP_DATA-0(default value)
#define   UPDATE_0       GpioDataRegs.GPACLEAR.bit.GPIO6 = 1    //上升沿发送数据从串口到活动寄存器
#define   UPDATE_1       GpioDataRegs.GPASET.bit.GPIO6 = 1

// GPIO4 --- CS------1(default value)，L有效
#define   CS_0           GpioDataRegs.GPACLEAR.bit.GPIO4 = 1
#define   CS_1           GpioDataRegs.GPASET.bit.GPIO4 = 1

// GPIO2 --- SCLK----0(default value)
#define   SCLK_0         GpioDataRegs.GPACLEAR.bit.GPIO2 = 1
#define   SCLK_1         GpioDataRegs.GPASET.bit.GPIO2 = 1

//调制引脚:PS0即说明书上的P0,以下同
// GPIO12--- PS0---0(default value)
#define   PS0_0          GpioDataRegs.GPACLEAR.bit.GPIO12 = 1
#define   PS0_1          GpioDataRegs.GPASET.bit.GPIO12 = 1

// GPIO11--- PS1---0(default value)
#define   PS1_0          GpioDataRegs.GPACLEAR.bit.GPIO11 = 1
#define   PS1_1          GpioDataRegs.GPASET.bit.GPIO11 = 1

// GPIO9 --- PS2---0(default value)
#define   PS2_0          GpioDataRegs.GPACLEAR.bit.GPIO9 = 1
#define   PS2_1          GpioDataRegs.GPASET.bit.GPIO9 = 1

// GPIO7 --- PS3-----0(default value)
#define   PS3_0          GpioDataRegs.GPACLEAR.bit.GPIO7 = 1
#define   PS3_1          GpioDataRegs.GPASET.bit.GPIO7 = 1

//数据引脚
// GPIO5 --- SDIO0---0(default value)
#define   SDIO0_0        GpioDataRegs.GPACLEAR.bit.GPIO5 = 1
#define   SDIO0_1        GpioDataRegs.GPASET.bit.GPIO5 = 1
#define   SDIO_DATA      GpioDataRegs.GPADAT.bit.GPIO5

// GPIO3 --- SDIO1---0(default value)
#define   SDIO1_0        GpioDataRegs.GPACLEAR.bit.GPIO3 = 1
#define   SDIO1_1        GpioDataRegs.GPASET.bit.GPIO3 = 1

// GPIO1 --- SDIO2---0(default value)
#define   SDIO2_0        GpioDataRegs.GPACLEAR.bit.GPIO1 = 1
#define   SDIO2_1        GpioDataRegs.GPASET.bit.GPIO1 = 1

// GPIO0 --- SDIO3---0(default value)
#define   SDIO3_0        GpioDataRegs.GPACLEAR.bit.GPIO0 = 1
#define   SDIO3_1        GpioDataRegs.GPASET.bit.GPIO0 = 1


//AD9959复位
void AD_Reset(void);
//AD9959数据更新
void AD_Update(void);
//IO端口初始化
void Init_AD_Gpio(void);
//写数据指令信息
void WriteToAD9959ViaSpi(Uint16 RegisterAddress, Uint16 NumberofRegisters, Uint16 *RegisterData,Uint16 temp);

void AD_Write_Data(unsigned char RegisterAddress, unsigned char NumberofRegisters, unsigned char *RegisterData,unsigned char temp);
void AD_Read_Data(unsigned char RegisterAddress, unsigned char NumberofRegisters, unsigned char *RegisterData);
//AD9959初始化
void AD9959_Init(void);

//void Write_Frequence(unsigned char Channel,unsigned long Freq);
//void Write_Amplitude(unsigned char Channel, unsigned int Ampli);
//void Write_Phase(unsigned char Channel,unsigned int Phase);

void Write_frequence(Uint16 Channel,Uint32 Freq);
void Write_Amplitude(Uint16 Channel, int16  Ampli);
void Write_Phase(Uint16 Channel,int16  Phase);

void InitReset(void);
void Init_AD9959(void);
void delay1 (Uint16 length);
void IO_Update(void);

extern void AD9959_SetFreMod2(Uint16 Channel, Uint32 Freq1, Uint32 Freq2);

#endif /* AD9959_2_H_ */
