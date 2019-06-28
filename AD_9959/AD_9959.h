/*
 * AD_9959.h
 *
 *  Created on: 2019Äê5ÔÂ16ÈÕ
 *      Author: L9379
 */

#ifndef AD_9959_H_
#define AD_9959_H_
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

#define CSR_ADD   0x00   //CSR
#define FR1_ADD   0x01   //FR1
#define FR2_ADD   0x02   //FR2
#define CFR_ADD   0x03   //CFR
#define CFTW0_ADD 0x04   //CTW0
#define CPOW0_ADD 0x05   //CPW0
#define ACR_ADD   0x06   //ACR
#define LSRR_ADD  0x07   //LSR
#define RDW_ADD   0x08   //RDW
#define FDW_ADD   0x09   //FDW

#define CS_H GpioDataRegs.GPASET.bit.GPIO0 = 1
#define CS_L GpioDataRegs.GPACLEAR.bit.GPIO0 = 1

#define SCLK_H GpioDataRegs.GPASET.bit.GPIO1 = 1
#define SCLK_L GpioDataRegs.GPACLEAR.bit.GPIO1 = 1

#define UPDATE_H GpioDataRegs.GPASET.bit.GPIO2 = 1
#define UPDATE_L GpioDataRegs.GPACLEAR.bit.GPIO2 = 1

#define SDIO0_H GpioDataRegs.GPASET.bit.GPIO3 = 1
#define SDIO0_L GpioDataRegs.GPACLEAR.bit.GPIO3 = 1


#define AD9959_PWR_H GpioDataRegs,GPASET.bit.GPIO5 =1
#define AD9959_PWR_L GpioDataRegs.GPACLEAR.bit.GPIO5=1

#define RESET_H GpioDataRegs.GPASET.bit.GPIO6 = 1
#define RESET_L GpioDataRegs.GPACLEAR.bit.GPIO6 = 1

#define PS0_H GpioDataRegs.GPASET.bit.GPIO7 = 1
#define PS0_L GpioDataRegs.GPACLEAR.bit.GPIO7 = 1

#define PS1_H GpioDataRegs.GPASET.bit.GPIO8 = 1
#define PS1_L GpioDataRegs.GPACLEAR.bit.GPIO8 = 1

#define PS2_H GpioDataRegs.GPASET.bit.GPIO9 = 1
#define PS2_L GpioDataRegs.GPACLEAR.bit.GPIO9 = 1

#define PS3_H GpioDataRegs.GPASET.bit.GPIO10 = 1
#define PS3_L GpioDataRegs.GPACLEAR.bit.GPIO10 = 1

#define SDIO1_H GpioDataRegs.GPASET.bit.GPIO11 = 1
#define SDIO1_L GpioDataRegs.GPACLEAR.bit.GPIO11 = 1

#define SDIO2_H GpioDataRegs.GPASET.bit.GPIO12 = 1
#define SDIO2_L GpioDataRegs.GPACLEAR.bit.GPIO12 = 1

#define SDIO3_H GpioDataRegs.GPASET.bit.GPIO13 = 1
#define SDIO3_L GpioDataRegs.GPACLEAR.bit.GPIO13 = 1

void AD9959_IOInit();
void AD9959_Init();
void InitSERVE();
void InitReset();
void  IO_Update();
void WriteData_AD9959(unsigned char RegisterAddress, unsigned char NumberofRegisters, unsigned char *RegisterData,unsigned char temp);
void Write_frequence(unsigned char Channel,Uint32 Freq);
void Write_Amplitude(unsigned char Channel, Uint16 Ampli);
void Write_Phase(unsigned char Channel,Uint16 Phase);
#endif /* AD_9959_H_ */
