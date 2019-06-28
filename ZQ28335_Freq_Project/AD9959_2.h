/*
 * AD9959_2.h
 *
 *  Created on: 2019-5-29
 *      Author: Administrator
 */

#ifndef AD9959_2_H_
#define AD9959_2_H_

#include "DSP28x_Project.h"


extern unsigned char CSR_DATA0[1]; //ʹ��CH0
extern unsigned char  CFTW0_DATA[4];
extern unsigned char  FR1_DATA[3];

//AD9959�Ĵ�����ַ����
#define CSR_ADD   0x00   //CSR   ͨ��ѡ��Ĵ���
#define FR1_ADD   0x01   //FR1   ���ܼĴ���1 ����Ĵ����������������ֽڡ�FR1���ڿ���оƬ�Ĺ�����ʽ��
#define FR2_ADD   0x02   //FR2   ���ܼĴ���2 ����Ĵ��������������ֽڡ�FR2���ڿ���AD9959�ĸ��ֹ��ܡ����Ժ�ģʽ��
#define CFR_ADD   0x03   //CFR   ͨ�����ܼĴ���
#define CFTW0_ADD 0x04   //CFTW0 ͨ��Ƶ��ת���ּĴ���
#define CPOW0_ADD 0x05   //CPOW0 ͨ����λת���ּĴ���
#define ACR_ADD   0x06   //ACR   ���ȿ��ƼĴ���
#define LSRR_ADD  0x07   //LSRR  ͨ������ɨ��Ĵ���
#define RDW_ADD   0x08   //RDW   ͨ����������ɨ��Ĵ���
#define FDW_ADD   0x09   //FDW   ͨ����������ɨ��Ĵ���

#define CSR 0x00               //ͨ��ѡ��Ĵ���
#define FR1 0x01               //���ܼĴ���1
#define FR2 0x02               //���ܼĴ���2
#define CFR 0x03               //ͨ�����ܼĴ���
#define CFTW0 0x04             //32λͨ��Ƶ��ת���ּĴ���
#define CPOW0 0x05             //14λͨ����λת���ּĴ���
#define ACR 0x06               //���ȿ��ƼĴ���
#define SRR 0x07               //����ɨ�趨ʱ��
#define RDW 0x08               //��������ɨ�趨ʱ��
#define FDW 0x09               //��������ɨ�趨ʱ��

#define CW1  0x0A               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW2  0x0B               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW3  0x0C               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW4  0x0D               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW5  0x0E               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW6  0x0F               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW7  0x10               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW8  0x11               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW9  0x12               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW10 0x13               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW11 0x14               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW12 0x15               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW13 0x16               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW14 0x17               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���
#define CW15 0x18               //���μĴ���(Profile Register)����FSK,ASK��PSK�Ļ���

//AD9959�˿�����

// ��������
// GPIO10--- PDC---0(default value),H��Ч
#define   AD9959_PWR_0   GpioDataRegs.GPACLEAR.bit.GPIO10 = 1
#define   AD9959_PWR_1   GpioDataRegs.GPASET.bit.GPIO10 = 1

// GPIO8 --- RESET---0(default value),H��Ч
#define   RESET_0        GpioDataRegs.GPACLEAR.bit.GPIO8 = 1
#define   RESET_1        GpioDataRegs.GPASET.bit.GPIO8 = 1

// GPIO6 --- UP_DATA-0(default value)
#define   UPDATE_0       GpioDataRegs.GPACLEAR.bit.GPIO6 = 1    //�����ط������ݴӴ��ڵ���Ĵ���
#define   UPDATE_1       GpioDataRegs.GPASET.bit.GPIO6 = 1

// GPIO4 --- CS------1(default value)��L��Ч
#define   CS_0           GpioDataRegs.GPACLEAR.bit.GPIO4 = 1
#define   CS_1           GpioDataRegs.GPASET.bit.GPIO4 = 1

// GPIO2 --- SCLK----0(default value)
#define   SCLK_0         GpioDataRegs.GPACLEAR.bit.GPIO2 = 1
#define   SCLK_1         GpioDataRegs.GPASET.bit.GPIO2 = 1

//��������:PS0��˵�����ϵ�P0,����ͬ
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

//��������
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


//AD9959��λ
void AD_Reset(void);
//AD9959���ݸ���
void AD_Update(void);
//IO�˿ڳ�ʼ��
void Init_AD_Gpio(void);
//д����ָ����Ϣ
void WriteToAD9959ViaSpi(Uint16 RegisterAddress, Uint16 NumberofRegisters, Uint16 *RegisterData,Uint16 temp);

void AD_Write_Data(unsigned char RegisterAddress, unsigned char NumberofRegisters, unsigned char *RegisterData,unsigned char temp);
void AD_Read_Data(unsigned char RegisterAddress, unsigned char NumberofRegisters, unsigned char *RegisterData);
//AD9959��ʼ��
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
