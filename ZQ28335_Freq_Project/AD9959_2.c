#include "DSP28x_Project.h"
#include "AD9959_2.h"
 /*-----------------------------------------------
  ���ƣ�AD9958��������
  ��д��Liu
  ���ڣ�2014.8
  �޸ģ���
  ���ݣ�
------------------------------------------------*/
//AD9958�ڼĴ��������б�
//-----------------------------------------------------------------------------------------------------------------
#define ChannelSelectRegisterAddress  0x00       //CSR ͨ��ѡ��Ĵ���������ͨ��ѡ�񣬴��� 3 �� ͨ��ģʽ�����ݴ������ȸߵ�λ����
                                                 // default Value = 0xF0 ��ϸ��μ�AD9958 datasheet Table 27	
																			           //Bit7: CH1 enable,Bit6: CH0 enable,
//Uint16 ChannelSelectRegisterdata0[1] =  {0x10};     // �� CH0

Uint16 ChannelSelectRegisterdata0[1] =  {0x50};     // �� CH0�� CH2, �� CH1  �� CH3
Uint16 ChannelSelectRegisterdata1[1] =  {0xa0};      // �� CH1�� CH3, �� CH0  �� CH2

																			
#define FunctionRegister1Address  0x01   //FR1 ���ܼĴ���1,��ϸ��μ�AD9958 datasheet Table 27																		
Uint16 FunctionRegister1data[3] = {0xD0,0x00,0x00};//default Value = 0x000000;   20��Ƶ;  Charge pump control = 75uA
                                                  //FR1<23> -- VCO gain control =0ʱ system clock below 160 MHz; 
                                                  //             =1ʱ, the high range (system clock above 255 MHz
#define FunctionRegister2Address 0x02   //FR2 ���ܼĴ���2 ��ϸ��μ�AD9958 datasheet Table 27	
Uint16 FunctionRegister2data[2] = {0x20,0x00};//default Value = 0x0000

#define ChannelFunctionRegisterAddress 0x03   //CFR ͨ�����ܼĴ���,��ϸ��μ�AD9958 datasheet Table 28																		
Uint16 ChannelFunctionRegisterdata[3] = {0x00,0x23,0x37};//default Value = 0x000302

#define ChannelFrequencyTuningWord0Address 0x04   //CTW0 ͨ��Ƶ��ת���ּĴ���,��ϸ��μ�AD9958 datasheet Table 28		

//25MHZ ��Դ����20��Ƶ
//Uint16 ChannelFrequencyTuningWord0data[4] = {0x00,0x0d,0x1B,0x71}; //OUT 100KHZ
//Uint16 ChannelFrequencyTuningWord0data[4] = {0x00,0x83,0x12,0x6F}; //OUT 1MHZ
//Uint16 ChannelFrequencyTuningWord0data[4] = {0x05,0x1E,0xB8,0x52};   //OUT 10MHZ
//Uint16 ChannelFrequencyTuningWord0data[4] = {0x0a,0x3d,0x70,0xa4};   //OUT 20MHZ
//Uint16 ChannelFrequencyTuningWord0data[4] = {0x28,0xF5,0xC2,0x8F}; //OUT 80MHZ
Uint16 ChannelFrequencyTuningWord0data0[4] = {0x33,0x33,0x33,0x33}; //OUT 100MHZ

//Uint16 ChannelFrequencyTuningWord0data1[4] = {0x00,0x0d,0x1B,0x71}; //OUT 100KHZ
//Uint16 ChannelFrequencyTuningWord0data1[4] = {0x00,0x83,0x12,0x6F}; //OUT 1MHZ
//Uint16 ChannelFrequencyTuningWord0data1[4] = {0x05,0x1E,0xB8,0x52};   //OUT 10MHZ
//Uint16 ChannelFrequencyTuningWord0data1[4] = {0x28,0xF5,0xC2,0x8F}; //OUT 80MHZ
Uint16 ChannelFrequencyTuningWord0data1[4] = {0x33,0x33,0x33,0x33}; //OUT 100MHZ

#define ChannelPhaseOffsetWord0Address 0x05   //CPW0 ͨ����λת���ּĴ���,��ϸ��μ�AD9958 datasheet Table 28																		
Uint16 ChannelPhaseOffsetWord0data[2] = {0x00,0x00};//default Value = 0x0000   @ = POW/2^14*360

#define AmplitudeControlAddress 0x06   //ACR ���ȿ��ƼĴ���,��ϸ��μ�AD9958 datasheet Table 28																		
Uint16 AmplitudeControldata[3] = {0x00,0x10,0x00};//default Value = 0x--0000 Rest = 18.91/Iout

#define LinearSweepRampRateAddress 0x07   //LSR ͨ������ɨ��Ĵ���,��ϸ��μ�AD9958 datasheet Table 28																		
Uint16 LinearSweepRampRatedata[2] = {0x00,0x00};//default Value = 0x----


#define LSRRisingDeltaAddress 0x08   //RDW ͨ����������ɨ��Ĵ���,��ϸ��μ�AD9958 datasheet Table 28																		
Uint16 LSRRisingDeltadata[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------

#define LSRFallingDeltaAddress 0x09   //FDW ͨ����������ɨ��Ĵ���,��ϸ��μ�AD9958 datasheet Table 28																		
Uint16 LSRFallingDeltadata[4] = {0x00,0x00,0x00,0x00};//default Value = 0x--------


//-------------- �������� --------------------------------

void delay1 (Uint16 length);
void delay2 (void);
void IO_Update(void);



void WriteToAD9958ViaSpi(Uint16 RegisterAddress, Uint16 NumberofRegisters, Uint16 *RegisterData,Uint16 temp);

void delay1 (Uint16 length)
{
	length = length*12;
    while(length--);
}

/*
 *  IO�˿ڳ�ʼ��
 */
void Init_AD_Gpio(void)
{
    EALLOW;
//     SDIO3  GPIO0  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO0 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO0 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
//     SDIO2  GPIO1  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO1 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO1 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO1 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO1 = 1;
//     SCLK GPIO2  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO2 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO2 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO2 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO2 = 1;
//     SDIO1 GPIO3  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO3 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO3 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO3 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO3 = 1;
//     CS GPIO4  Init = 1
       GpioCtrlRegs.GPAPUD.bit.GPIO4 =1;
       GpioDataRegs.GPASET.bit.GPIO4 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO4 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO4 = 1;
//     SDIO0 GPIO5  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO5 = 1;
       GpioDataRegs.GPASET.bit.GPIO5 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO5 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO5 = 1;
//     UP_DATA GPIO6  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO6 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO6 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO6 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO6 = 1;
//     PS3 GPIO7  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO7 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO7 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO7 = 1;
//     RESET GPIO8  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO8 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO8 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;
//     PS2 GPIO9  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO9 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO9 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO9 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO9 = 1;
//     PDC GPIO10  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO10 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO10 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;
//     PS1 GPIO11  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO11 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO11 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO11 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO11 = 1;
//     PS0 GPIO12  Init = 0
       GpioCtrlRegs.GPAPUD.bit.GPIO12 = 1;
       GpioDataRegs.GPACLEAR.bit.GPIO12 = 1;
       GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 0;
       GpioCtrlRegs.GPADIR.bit.GPIO12 = 1;

       // ��������
       AD9959_PWR_0; RESET_0; UPDATE_0; CS_1; SCLK_0;
       // ��������
       PS0_0;PS1_0;PS2_0;PS3_0;
       // ��������
       SDIO0_0;SDIO1_0;SDIO2_0;SDIO3_0;

       EDIS;

       //InitReset();
}


void InitReset(void)
{
    RESET_0;
	delay1(1);
	RESET_1;
	delay1(30);
	RESET_0;
}

void IO_Update(void)
{
	UPDATE_0;  //UPDATE = 0;
	delay1(2);
	UPDATE_1;
	delay1(4);
	UPDATE_0;
}



void InitServe(void)
{

    // ��������
    AD9959_PWR_0; RESET_0; UPDATE_0; CS_1; SCLK_0;
    // ��������
    PS0_0;PS1_0;PS2_0;PS3_0;
    // ��������
    SDIO0_0;SDIO1_0;SDIO2_0;SDIO3_0;
    
//	  PWR =0;
//    CS  = 1;
//    SCLK_0;
//    UPDATE = 0;
//
//    PS0 = 0;
//    PS1 = 0;
//    PS2 = 0;
//    PS3 = 0;
//
//    SDIO0 = 0;
//    SDIO1 = 0;
//    SDIO2 = 0;
//    SDIO3 = 0;
}

// ����FR1
void Init_AD9959(void)
{
	InitServe();
	InitReset();

	//CSR address 0x00.Only 3-Wire Communication,SDIO0 Input and SDIO2 output,
	//CH0 ��  and CH1 �ر� ���ݴ���Ӹ�λ����λ
	//WriteToAD9958ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata0,0);

	//FR1 address 0x01.  25MHZ����,20��Ƶ,SYNC_CLK = 125MHz System Clock = 500MHz
	//�����⼤��, SYNC CLOCK ����
	// PLL ���� 75uA
	//VCO gain control =0 ϵͳƵ�� <160MHZ��
	//VCO gain control =1 ϵͳƵ�� <2500MHZ��
	WriteToAD9959ViaSpi(FunctionRegister1Address,3,FunctionRegister1data,1);


	//  WriteToAD9958ViaSpi(ChannelFrequencyTuningWord0Address,4,ChannelFrequencyTuningWord0data0,0);
	// CTW0 address 0x04.���10MHZƵ��

	//  WriteToAD9958ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata1,0);
	// CSR address 0x00.Only 3-Wire Communication,SDIO0 Input and SDIO2 output,
																																								   //CH0 �ر� and CH1 �� ���ݴ���Ӹ�λ����λ
	//  WriteToAD9958ViaSpi(ChannelFrequencyTuningWord0Address,4,ChannelFrequencyTuningWord0data1,1);
	// CTW0 address 0x04.���100KHZƵ��

}


//---------------------------------
//WriteToAD9959ViaSpi();
//---------------------------------
//�������ܣ�������ͨ��SPI��AD9958д����
// 
//RegisterAddress ---- �Ĵ�����ַ
//NumberofRegisters---- �����ֽ���
//*RegisterData ----- ������ʼ��ַ
//temp ----- �Ƿ����IO�Ĵ���
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void WriteToAD9959ViaSpi(Uint16 RegisterAddress, Uint16 NumberofRegisters, Uint16 *RegisterData,Uint16 temp)
{
	Uint16	ControlValue = 0;
	Uint16	ValueToWrite = 0;
	Uint16	RegisterIndex = 0;
	Uint16	i = 0;

	//Create the 8-bit header
	ControlValue = RegisterAddress;

	SCLK_0;  //SCLK_0;
	CS_0;	 //bring CS low
	//Write out the control word
	for(i=0; i<8; i++)
	{
		SCLK_0;
		if(0x80 == (ControlValue & 0x80))
		{
			SDIO0_1;  //= 1;	  //Send one to SDIO0pin
		}
		else
		{
			SDIO0_0;  //= 0;	  //Send zero to SDIO0pin
		}
		SCLK_1;  // = 1;
		ControlValue <<= 1;	//Rotate data
	}

	SCLK_0;
	//And then the data
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			SCLK_0;
			if(0x80 == (ValueToWrite & 0x80))
			{
				SDIO0_1;  //= 1;	  //Send one to SDIO0pin
			}
			else
			{
				SDIO0_0;  //= 0;	  //Send zero to SDIO0pin
			}
			SCLK_1;  // = 1;
			ValueToWrite <<= 1;	//Rotate data
		}
		SCLK_0;
	}

	if(temp==1)
	{
		IO_Update();
	}	
	CS_1;  // = 1;	//bring CS high again
} 

Uint16 ChannelFrequencyTuningWord0data[4] ={0x00,0x00,0x00,0x00};	//Ƶ�ʿ�����0
//=====================================================================
void FreTuning(Uint32 Freq,Uint16 *FreData)
{
	Uint32 Temp;

	Temp=(Uint32)Freq*8.589934592;	   //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�  4.294967296=(2^32)/500000000
	FreData[3]=(Uint16)(Temp & 0xFF);
	FreData[2]=(Uint16)((Temp>>8)  & 0xFF);
	FreData[1]=(Uint16)((Temp>>16) & 0xFF);
	FreData[0]=(Uint16)((Temp>>24) & 0xFF);
}

//===================����Ƶƫ�֡�Ƶ���ֺͷ��ͳ���======================
// Uint32 Freq: 0~200MHz,����LPF�Ľ�ֹƵ��
void Write_frequence(Uint16 Channel,Uint32 Freq)
{
	Uint16 ChannelFrequencyTuningWord0data[4] ={0x00,0x00,0x00,0x00};	//�м����

	FreTuning(Freq,ChannelFrequencyTuningWord0data);  // �õ�Ƶ�ʿ�����

	if(Channel==0)
	{
		//���ƼĴ���д��CH0ͨ��
		WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata0,0);
		//CTW0 address 0x04.���CH0�趨Ƶ��
		WriteToAD9959ViaSpi(ChannelFrequencyTuningWord0Address,4,ChannelFrequencyTuningWord0data,0);
	}

	if(Channel==1)
	{
		WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata1,0); //���ƼĴ���д��CH1ͨ��																																							   //CH0 �ر� and CH1 �� ���ݴ���Ӹ�λ����λ
		WriteToAD9959ViaSpi(ChannelFrequencyTuningWord0Address,4,ChannelFrequencyTuningWord0data,0); //CTW0 address 0x04.���CH1�趨Ƶ��
	}																																					 //CH0 ��  and CH1 �ر� ���ݴ���Ӹ�λ����λ
	
}

//==============���·���====================================
// int16 Ampli: 0~1023(10λ)
void Write_Amplitude(Uint16 Channel, int16 Ampli)
{
	Uint16 A_temp;//=0x23ff;
	A_temp=Ampli|0x1000;
	AmplitudeControldata[2]=(Uint16)(A_temp & 0xff);  //��λ����
	AmplitudeControldata[1]=(Uint16)((A_temp>>8) & 0xff); //��λ����

	if(Channel==0)
	{
		WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata0,0); //���ƼĴ���д��CH0ͨ��
		WriteToAD9959ViaSpi(AmplitudeControlAddress,3,AmplitudeControldata,0); //
	}
	if(Channel==1)
	{
		WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata1,0); //���ƼĴ���д��CH1ͨ��																																						   //CH0 �ر� and CH1 �� ���ݴ���Ӹ�λ����λ
		WriteToAD9959ViaSpi(AmplitudeControlAddress,3,AmplitudeControldata,0); //
	}
}

void PhaTuning(int16 Phase, Uint16 *PhaData)
{
	Uint16 P_temp=0;

	P_temp=(Uint16)Phase*45.511111;//��������λ��д�룬����1�ȣ�45.511111=2^14��/360
	PhaData[1]=(Uint16)(P_temp & 0xff);
	PhaData[0]=(Uint16)((P_temp>>8) & 0xff);
}

// Phase:int16 0~360��
void Write_Phase(Uint16 Channel, int16 Phase)
{
//	Uint16 P_temp=0;
//
//	P_temp=(Uint16)Phase*45.511111;//��������λ��д�룬����1�ȣ�45.511111=2^14��/360
//	ChannelPhaseOffsetWord0data[1]=(Uint16)(P_temp & 0xff);
//	ChannelPhaseOffsetWord0data[0]=(Uint16)((P_temp>>8) & 0xff);
	PhaTuning(Phase, ChannelPhaseOffsetWord0data);

	if(Channel==0)
	{
		WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata0,0);  //���ƼĴ���д��CH1ͨ����																																							   //CH0 �ر� and CH1 �� ���ݴ���Ӹ�λ����λ
		WriteToAD9959ViaSpi(ChannelPhaseOffsetWord0Address,2,ChannelPhaseOffsetWord0data,0); //
	}

	if(Channel==1)
	{
		WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata1,0);  //���ƼĴ���д��CH1ͨ����																																							   //CH0 �ر� and CH1 �� ���ݴ���Ӹ�λ����λ
		WriteToAD9959ViaSpi(ChannelPhaseOffsetWord0Address,2,ChannelPhaseOffsetWord0data,0); //
	}
}

//����ģʽ*************************************************

//-----------------------------------------------------------------
// void AD9959_SetFreMod2(double fre1,double fre2)
//-----------------------------------------------------------------
//
// ��������: ����2FSKģʽ�����Ƶ��
// ��ڲ���: double fre1�����Ƶ��1
//					 double fre2�����Ƶ��2
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: WriteToAD9959ViaSpi(CFTW0,4,FreData,1);;FreTuning(fre,FreData);
// ע������:
//
//-----------------------------------------------------------------
// ���Ʋ���
// CFR[23:22]=1��AM��,2(FM),3(PM)
// CFR[14] = 0;
// FR1[9:8] = 0(��������1��4������2��8������3��16����
// �������룺P0->CH0;  P1->CH1;  P2->CH2;P3->CH3;
// ���ƹ�ϵ������0->f1, 1->f2

void AD9959_SetFreMod2(Uint16 Channel, Uint32 Freq1, Uint32 Freq2)
{
	Uint16 Fre1Data[4];
	Uint16 Fre2Data[4];
	Uint16 CSRval[1];

	CSRval[0] = Channel<<4;

	// ѡ��ͨ��
	WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,CSRval,0);  //���ƼĴ���д�����ͨ��


	Uint16 CFRData[3] = {0x80,0x23,0x30};
	Uint16 FR1Data[3] = {0xd0,0x00,0x00};
	WriteToAD9959ViaSpi(FR1,3,FR1Data,0);
	WriteToAD9959ViaSpi(CFR,3,CFRData,0);

	FreTuning(Freq1,Fre1Data);
	FreTuning(Freq2,Fre2Data);
//
//
//
//	if(Channel==0)
//	{
//		WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata0,0);  //���ƼĴ���д��CH0��2ͨ����
//	}
//
//	if(Channel==1)
//	{
//		WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,ChannelSelectRegisterdata1,0);  //���ƼĴ���д��CH1��3ͨ����																																							   //CH0 �ر� and CH1 �� ���ݴ���Ӹ�λ����λ
//	}

	WriteToAD9959ViaSpi(CFTW0,4,Fre1Data,0);
	WriteToAD9959ViaSpi(CW1,4,Fre2Data,0);   // д�����Ĵ���CW1
}


//-----------------------------------------------------------------
// void AD9959_SetPhaMod2(double fre,int pha1,int pha2)
//-----------------------------------------------------------------
//
// ��������: ����2PSKģʽ�����Ƶ��,��λ
// ��ڲ���: double fre�����Ƶ��1
//					 int pha1����λ1
//					 int pha2����λ2
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: WriteToAD9959ViaSpi(CFTW0,4,FreData,1);;FreTuning(fre,FreData);PhaTuning(pha1,PhaData);
// ע������:
//
//-----------------------------------------------------------------
void AD9959_SetPhaMod2(Uint16 Channel, Uint32 Freq, int16 Phase1, int16 Phase2)
{
	Uint16 PhaData[2];
	Uint16 FreData[4];
	Uint16 CSRval[1];

	CSRval[0] = Channel<<4;

	// ����ͨ��
	WriteToAD9959ViaSpi(ChannelSelectRegisterAddress,1,CSRval,0);  //ʹ�����ͨ����																																							   //CH0 �ر� and CH1 �� ���ݴ���Ӹ�λ����λ


	// ��λ��������
	Uint16 CFRData[3] = {0xc0,0x23,0x31};
	Uint16 FR1Data[3] = {0xd0,0x00,0x00};
	WriteToAD9959ViaSpi(FR1,3,FR1Data,0);
	WriteToAD9959ViaSpi(CFR,3,CFRData,0);

	// ����������λ
	PhaTuning(Phase1, PhaData);
	WriteToAD9959ViaSpi(CPOW0,2,PhaData,0);
	PhaTuning(Phase2, PhaData);
	WriteToAD9959ViaSpi(CW1,4,PhaData,0);

	// ����Ƶ��
	FreTuning(Freq,FreData);
	WriteToAD9959ViaSpi(CFTW0,4,FreData,0);
}

