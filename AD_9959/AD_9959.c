/*
 * AD_9959.c
 *
 *  Created on: 2019年5月16日
 *      Author: L9379
 */
#include "AD_9959.h"

unsigned char CSR_DATA0[1] = {0x10};      // ¿ª CH0
unsigned char CSR_DATA1[1] = {0x20};      // ¿ª CH1
unsigned char CSR_DATA2[1] = {0x40};      // ¿ª CH2
unsigned char CSR_DATA3[1] = {0x80};      // ¿ª CH3

unsigned char FR2_DATA[2] = {0x00,0x00};//default Value = 0x0000
unsigned char CFR_DATA[3] = {0x00,0x03,0x02};//default Value = 0x000302

unsigned char CPOW0_DATA[2] = {0x00,0x00};//default Value = 0x0000   @ = POW/2^14*360

unsigned char LSRR_DATA[2] = {0x00,0x00};//default Value = 0x----

unsigned char RDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------

unsigned char FDW_DATA[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------
void AD9959_IOInit()
{
    EALLOW;
    GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
   // GpioCtrlRegs.GPAPUD.bit.GPIO0 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
   // GpioCtrlRegs.GPAPUD.bit.GPIO1 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;
  //  GpioCtrlRegs.GPAPUD.bit.GPIO2 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
   // GpioCtrlRegs.GPAPUD.bit.GPIO3 = 0;


    GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;
   // GpioCtrlRegs.GPAPUD.bit.GPIO4 = 0;


    GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;
   // GpioCtrlRegs.GPAPUD.bit.GPIO5 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;
  //  GpioCtrlRegs.GPAPUD.bit.GPIO6 = 0;


    GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;
   // GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;
 //   GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
  //  GpioCtrlRegs.GPAPUD.bit.GPIO9 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;
  //  GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;
    //GpioCtrlRegs.GPAPUD.bit.GPIO11 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;
  //  GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;

    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;
   // GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;
    EDIS;
}

void AD9959_Init()
{
    AD9959_IOInit();
    unsigned char FR1_DATA[3]={0x00,0x00,0xab,};
    InitSERVE();
    InitReset();
    WriteData_AD9959(FR1_ADD,3,FR1_DATA,1);
    Write_frequence(3,100000);
        Write_frequence(0,100000);
        Write_frequence(1,100000);
        Write_frequence(2,100000);
    //  u32 SinFre[5] = {50, 50, 50, 50};
    //  u32 SinAmp[5] = {1023, 1023, 1023, 1023};
    //  u32 SinPhr[5] = {0, 4095, 4095*3, 4095*2};
    //  u32 SinFre0[4] = {1000};

    ////    Write_frequence(3,50);
    ////    Write_frequence(0,50);
    ////    Write_frequence(1,50);
    ////    Write_frequence(2,50);

        Write_Phase(3, 0);
        Write_Phase(0, 0);
        Write_Phase(1, 0);
        Write_Phase(2, 0);

        Write_Amplitude(3, 1023);
        Write_Amplitude(0,1023);
        Write_Amplitude(1, 1023);
        Write_Amplitude(2, 1023);
}

void InitSERVE()
{
    AD9959_PWR_L;
    CS_H;
    SCLK_L;
    UPDATE_L;
    PS0_L;
    PS1_L;
    PS2_L;
    PS3_L;
    SDIO0_L;
    SDIO1_L;
    SDIO2_L;
    SDIO3_L;
}

void InitReset()
{
    RESET_L;
    DELAY_US(1);
    RESET_H;
    DELAY_US(1);
    DELAY_US(30);
    RESET_L;
}

void  IO_Update()
{
    UPDATE_L;
    DELAY_US(2);
    UPDATE_H;
    DELAY_US(4);
    UPDATE_L;
}

void AD9959_write_data(unsigned char register_adress,unsigned char data_number, unsigned char *register_date,unsigned char temp)
{
    unsigned char cmp;
    write_datas(register_adress);
    for(cmp=0;cmp<data_number;cmp++)
    {
        write_datas(register_date[cmp]);
    }
    if(temp==1)
    {
        IO_Update();
    }
}
void WriteData_AD9959(unsigned char RegisterAddress, unsigned char NumberofRegisters, unsigned char *RegisterData,unsigned char temp)
{
    unsigned char  ControlValue = 0;
    unsigned char  ValueToWrite = 0;
    unsigned char  RegisterIndex = 0;
    unsigned char  i = 0;
    ControlValue = RegisterAddress;
   // SCLK_L;
   // CS_L;
    write_datas(ControlValue);
    DELAY_US(1);
   /* for(i=0; i<8; i++)
    {
        SCLK_L;
        if(0x80 == (ControlValue & 0x80))
        SDIO0_H;
        else
        SDIO0_L;
        SCLK_H;
      //  DELAY_US(1);
        ControlValue <<= 1;
    }*/
  //  SCLK_L;
  //  DELAY_US(1);
    for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
    {
        ValueToWrite = RegisterData[RegisterIndex];
        write_datas(ValueToWrite);
        DELAY_US(5);
        /*for (i=0; i<8; i++)
        {
            SCLK_L;
          //  DELAY_US(3);
            if(0x80 == (ValueToWrite & 0x80))
            SDIO0_H;
            else
            SDIO0_L;
            SCLK_H;
         //   DELAY_US(3);
            ValueToWrite <<= 1;
        }
        SCLK_L;*/
    }
    if(temp==1)
    IO_Update();
   // CS_H;
}

void Write_frequence(unsigned char Channel,Uint32 Freq)
{
    unsigned char CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};
    Uint32 Temp;
    //    Temp=(Uint32)Freq*8.589934592;
    Temp=(Uint32)Freq*4.294967296;
    CFTW0_DATA[3]=(unsigned char)Temp;
    CFTW0_DATA[2]=(unsigned char)(Temp>>8);
    CFTW0_DATA[1]=(unsigned char)(Temp>>16);
    CFTW0_DATA[0]=(unsigned char)(Temp>>24);
    if(Channel==0)
    {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);
        WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);
    }
    else if(Channel==1)
    {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);
        WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);
    }
    else if(Channel==2)
    {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);
        WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,1);
    }
    else if(Channel==3)
    {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);
        WriteData_AD9959(CFTW0_ADD,4,CFTW0_DATA,3);
    }

}

void Write_Amplitude(unsigned char Channel, Uint16 Ampli)
{
    Uint16 A_temp;//=0x23ff;
    unsigned char ACR_DATA[3] = {0x00,0x00,0x00};

    A_temp=Ampli|0x1000;
    ACR_DATA[2] = (unsigned char)A_temp;
    ACR_DATA[1] = (unsigned char)(A_temp>>8);
    if(Channel==0)
    {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA0,1);
        WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
    }
    else if(Channel==1)
    {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA1,1);
        WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
    }
    else if(Channel==2)
    {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA2,1);
        WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
    }
    else if(Channel==3)
    {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA3,1);
        WriteData_AD9959(ACR_ADD,3,ACR_DATA,1);
    }

}

void Write_Phase(unsigned char Channel,Uint16 Phase)
{
    Uint16 P_temp=0;
  P_temp=(Uint16)Phase;
    CPOW0_DATA[1]=(unsigned char)P_temp;
    CPOW0_DATA[0]=(unsigned char)(P_temp>>8);
    if(Channel==0)
  {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA0,0);
    WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  }
  else if(Channel==1)
  {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA1,0);
    WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  }
  else if(Channel==2)
  {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA2,0);
    WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  }
  else if(Channel==3)
  {
        WriteData_AD9959(CSR_ADD,1,CSR_DATA3,0);
    WriteData_AD9959(CPOW0_ADD,2,CPOW0_DATA,0);
  }
}


void write_datas(unsigned int data)
{
    int i;
    CS_L;
    for(i=0;i<8;i++)
    {
        SCLK_L;
        if((data & 0x80)==0x80)
           SDIO0_H;
        else
            SDIO0_L;
        data<<=1;
        DELAY_US(1);
            SCLK_H;
        DELAY_US(1);
    }
    CS_H;
    DELAY_US(1);
}

