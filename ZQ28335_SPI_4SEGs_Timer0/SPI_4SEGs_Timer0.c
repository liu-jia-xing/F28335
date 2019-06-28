/********************************************************************
* ���ܣ��������ʾ����,��ʱ��ˢ�£�����ʾ���֡��ַ��������ʽ
*
* �ر�˵����
*    ����InitSpiaGpio()Ĭ��ʹ��GPIO16-19�ţ�������ʹ��GPIO54-57�����޸ġ�
*    �޸��Դ��ĺ�������Ҫֱ���޸Ĵ��룬�޸ĺ�ĺ���Ҳ��Ҫ����common�ļ�����
*    ���������������������InitSpia54to57Gpio()�����ڱ��ļ���
*
* ������
* ��1�����������ˢ��Ƶ�ʣ�tintfreq����λ��Hz��ÿ��ˢ��һλ�����
*     float tintfreq=200;          // ��ʱ�ж�Ƶ�ʣ�Hz
* ��2��ÿ���ַ�����ʾʱ����CpuTimer0.InterruptCount�п��ƣ�Ĭ��Ϊ1�루ˢ��tintfreq�Σ�
*     CpuTimer0.InterruptCount==(Uint16)tintfreq
* ��3������ʾ���ַ����ڱ���DispSym��˳�����Ӿ�һ��
*     unsigned char DispSym[]={"Aa19"};   // A~F,a~f,��0~9�����ʾ
* ��4������GetSymCode��ѯDispSym�Ķ��룬Ĭ��Ϊ1�����һ�Σ�
* ��5������GetNumCode��ѯDispData�Ķ��룬Ĭ��Ϊ1�����һ�Σ�
*
*Ӳ����·��SPI�ӿ�ʹ��GPIO54-57,���Ӵ���ת��оƬ74CH164,
*       GPIO54��SIMO,GPIO56��CLK������δʹ��
*       GPIO16-17��SEG1-2,GPIO62-63��SEG3-4;SEG1Ϊ��λ
*���Ʒ�ʽ��
*       ��ʱˢ��
*
*  Created on: 2016-11-19
*      Author: �Ž�ѧԺ���ӹ���ѧԺ  ������
**********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File


/********************************�궨�������λѡ IO �ӿ�*******************************************/
//���ݵ�4λ��SEG1�����λ��
#define  SET_BIT4	GpioDataRegs.GPASET.bit.GPIO16	 = 1 		//������� 8_LEDS ���ӵ� IO16 ��Ӧ
#define  RST_BIT4	GpioDataRegs.GPACLEAR.bit.GPIO16 = 1		//������� 8_LEDS ���ӵ� IO16��Ӧ

//���ݵ�3λ��SEG2
#define  SET_BIT3   GpioDataRegs.GPASET.bit.GPIO17	 = 1		//������� 8_LEDS ���ӵ� IO17 ��Ӧ
#define  RST_BIT3	GpioDataRegs.GPACLEAR.bit.GPIO17 = 1		//������� 8_LEDS ���ӵ� IO17 ��Ӧ

//���ݵ�2λ��SEG3
#define  SET_BIT2   GpioDataRegs.GPBSET.bit.GPIO62	 = 1		//������� 8_LEDS ���ӵ� IO62 ��Ӧ
#define  RST_BIT2	GpioDataRegs. GPBCLEAR.bit.GPIO62 = 1		//������� 8_LEDS ���ӵ� IO62 ��Ӧ

//���ݵ�1λ��SEG4�����λ��
#define  SET_BIT1   GpioDataRegs.GPBSET.bit.GPIO63	 = 1		//������� 8_LEDS ���ӵ� IO63 ��Ӧ
#define  RST_BIT1	GpioDataRegs.GPBCLEAR.bit.GPIO63 = 1		//������� 8_LEDS ���ӵ� IO63 ��Ӧ
/*****************************************************************************************************/


/*********************************************��������************************************************/
void InitSpia54to57Gpio(void);  //SPI���ų�ʼ��
void Init4SEGsGpio(void);      // ����SEG1~4Ƭѡ���ų�ʼ��ΪL(H��Ч��ʾ)
void InitSpiFun(void);           // SPI���ܳ�ʼ��
void InitSpiFifo(void);      // SPI��FIFO���ܳ�ʼ����ʹ��FIFO���͹��ܣ����չ���δ����

void DispDataSplit(Uint16 data);// 4λ�����Ϊ4���������浽����DispDataBit�У�[0]Ϊ��λ
void SellectSeg(Uint16 i);     // �����λѡ������iΪ0~3����ӦSEG1~4
void SpiXmitData(Uint16 a);        // ������a���͵�SPITXBUF

interrupt void cpu_timer0_isr(void);

/*****************************************************************************************************/

/************************************������ر���*********************************************/
unsigned char msg[10]={0xC0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};	//�������룺0~9
unsigned char msg2_ABC[6]={0x88, 0x80, 0xc6, 0xc0, 0x86, 0x8e};	//�������룺A~F
unsigned char msg2_abc[6]={0xa0, 0x83, 0xa7, 0xa1, 0x84, 0x8e};	//�������룺a~f
unsigned char msg3_N[1]={0xbf};	//�������룺����
unsigned char Syms[6][4]= {"aaaa","bbbb","cccc","dddd","eeee","ffff"};
unsigned char Syms2[6][4]= {"AAAA","BBBB","CCCC","DDDD","EEEE","FFFF"};

//unsigned char msg[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};	//�������룺0~9
//unsigned char msg2_ABC[6]={0x77, 0x7f, 0x39, 0x3f, 0x79, 0x71};	//�������룺A~F
//unsigned char msg2_abc[6]={0x5F, 0x83, 0x58, 0xa1, 0x7b, 0x8e};	//�������룺a~f
// �ַ�0~9�ı���Ϊ��0x30~0x39;  �ַ�A~Z�ı���Ϊ��0x41~0x5A;  �ַ�a~z�ı���Ϊ��0x61~0x7A;
unsigned char DispSym[]={"Aa19"};   // A~F,a~f,��0~9�����ʾ

unsigned int DispDataBit[4] = {0};											//��Ų�ֺ����λ����
Uint16 DispData = 0;															//��ʾ������
int16  Nnum =-3;
//Uint16 Loop = 0;															//ѭ��ɨ�����
//Uint16 loop = 0;

/*****************************************************************************************************/


float tintfreq=200;       // ��ʱ�ж�Ƶ�ʣ�Hz
float tintprd=0;          // ��ʱ�ж����ڣ�us

static Uint16 Loop_8SEG=0; // 8�������ר�����
Uint16 Disp_8SEG_Code[4]={0xC0,0xC0,0xC0,0xC0};   // 8λ�������ʾ���ֶα���,��ʼ��Ϊ4��0


// ��ʼ��SPIAģ�����ţ�SPISIMOA��SPISOMIA��SPICLKA��SPISTEA�ֱ���GPIO16~19��GPIO54~57
void InitSpia54to57Gpio()
{

   EALLOW;
//    GpioCtrlRegs.GPAPUD.bit.GPIO16 = 0;   // Enable pull-up on GPIO16 (SPISIMOA)
//    GpioCtrlRegs.GPAPUD.bit.GPIO17 = 0;   // Enable pull-up on GPIO17 (SPISOMIA)
//    GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;   // Enable pull-up on GPIO18 (SPICLKA)
//    GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;   // Enable pull-up on GPIO19 (SPISTEA)


    GpioCtrlRegs.GPBPUD.bit.GPIO54 = 0;   // Enable pull-up on GPIO54 (SPISIMOA)
    GpioCtrlRegs.GPBPUD.bit.GPIO55 = 0;   // Enable pull-up on GPIO55 (SPISOMIA)
    GpioCtrlRegs.GPBPUD.bit.GPIO56 = 0;   // Enable pull-up on GPIO56 (SPICLKA)
    GpioCtrlRegs.GPBPUD.bit.GPIO57 = 0;   // Enable pull-up on GPIO57 (SPISTEA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.  (SPI)
// Comment out other unwanted lines.
// ÿ�����ŷ�4��ģʽ��0������SYSCLKͬ����1:    3���������ڣ�2�� 6���������ڣ�3����ͬ��

//    GpioCtrlRegs.GPAQSEL2.bit.GPIO16 = 3; // Asynch input GPIO16 (SPISIMOA)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO17 = 3; // Asynch input GPIO17 (SPISOMIA)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO18 = 3; // Asynch input GPIO18 (SPICLKA)
//    GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3; // Asynch input GPIO19 (SPISTEA)

    GpioCtrlRegs.GPBQSEL2.bit.GPIO54 = 3; // Asynch input GPIO16 (SPISIMOA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO55 = 3; // Asynch input GPIO17 (SPISOMIA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO56 = 3; // Asynch input GPIO18 (SPICLKA)
    GpioCtrlRegs.GPBQSEL2.bit.GPIO57 = 3; // Asynch input GPIO19 (SPISTEA)


/* Configure SPI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SPI functional pins.
// Comment out other unwanted lines.

//    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 1; // Configure GPIO16 as SPISIMOA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 1; // Configure GPIO17 as SPISOMIA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 1; // Configure GPIO18 as SPICLKA
//    GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 1; // Configure GPIO19 as SPISTEA

    GpioCtrlRegs.GPBMUX2.bit.GPIO54 = 1; // Configure GPIO54 as SPISIMOA
    GpioCtrlRegs.GPBMUX2.bit.GPIO55 = 1; // Configure GPIO55 as SPISOMIA
    GpioCtrlRegs.GPBMUX2.bit.GPIO56 = 1; // Configure GPIO56 as SPICLKA
    GpioCtrlRegs.GPBMUX2.bit.GPIO57 = 1; // Configure GPIO57 as SPISTEA

    EDIS;
}

/******************************�����λѡ IO �ӿڳ�ʼ��*******************************************/
// ����SEG1~4ΪI/O���������ʼ��ΪL
void Init4SEGsGpio(void)
{  
    EALLOW;
	
	//GpioCtrlRegs.GPBPUD.bit.GPIO16= 0;   	// Enable pullup on GPIO16
    GpioCtrlRegs.GPAMUX2.bit.GPIO16 = 0;  	// GPIO16 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO16 = 1;   	// GPIO16 = output
    GpioDataRegs.GPASET.bit.GPIO16 = 1;   	// Load output latch

	//GpioCtrlRegs.GPBPUD.bit.GPIO59= 0;   	// Enable pullup on GPIO17
    GpioCtrlRegs.GPAMUX2.bit.GPIO17 = 0;  	// GPIO17 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO17 = 1;   	// GPIO17 = output
    GpioDataRegs.GPASET.bit.GPIO17 = 1;   	// Load output latch
    
	//GpioCtrlRegs.GPBPUD.bit.GPIO62 = 0;   	// Enable pullup on GPIO62
    GpioCtrlRegs.GPBMUX2.bit.GPIO62 = 0;  	// GPIO62 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO62 = 1;   	// GPIO62 = output
    GpioDataRegs.GPBSET.bit.GPIO62 = 1;   	// Load output latch
    
    //GpioCtrlRegs.GPBPUD.bit.GPIO63 = 0;   	// Enable pullup on GPIO63
    GpioCtrlRegs.GPBMUX2.bit.GPIO63 = 0;  	// GPIO63 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO63 = 1;   	// GPIO63 = output
    GpioDataRegs.GPBSET.bit.GPIO63 = 1;   	// Load output latch

    EDIS;  
    
    RST_BIT1;
    RST_BIT2;
    RST_BIT3;
    RST_BIT4;   
}
/*****************************************************************************************************/

/*********************************************Spi��ʼ��************************************************/
// spiģ�鹦�ܳ�ʼ����
// SPICCR.all =0x004F;�����½�����������������룬���ݳ���16λ
// SPICTL.all =0x0006; ����ģʽ��ʱ����λ������ʹ��4��SPI����
// SPIBRR =0x007F;���ò�����=LSPCLK/128=150M/4/128=292K(�����Ǳ�����)
// SPICCR.all =0x00DF;��·ģʽ���������
void InitSpiFun(void)
{
	SpiaRegs.SPICCR.all =0x004F;	    // Reset on, Falling Edge out, 16-bit char bits
	                                    //0x000F��ӦRising Edge��0x004F��ӦFalling Edge
	SpiaRegs.SPICTL.all =0x0006;    	// Enable master mode, normal phase,
                                        // enable talk, and SPI int disabled.
	SpiaRegs.SPIBRR =0x007F;            // �����ʼĴ�����������=LSPCLK/(SPIBRR+1)��
	                                    // ���ΪLSPCLK/4����СΪLSPCLK/128��0x7F��
    SpiaRegs.SPICCR.all =0x00DF;		// Relinquish SPI from Reset(��·ģʽ)
    SpiaRegs.SPIPRI.bit.FREE = 1;       // Set so breakpoints don't disturb xmission���������У�
}
/*****************************************************************************************************/

/****************************************Spiģ��FIFO����**********************************************/

// ʹ��FIFO���͹��ܣ����չ���δ����
void InitSpiFifo()
{
	// Initialize SPI FIFO registers
    SpiaRegs.SPIFFTX.all=0xE040;
    SpiaRegs.SPIFFRX.all=0x204f;
    SpiaRegs.SPIFFCT.all=0x0;
}


/******************************�����λѡ�������ӵ�λ����λɨ�裩***************************************************/
// �����λѡ������BIT1 ��Ӧ���ұߵ�һλ
void SellectSeg(Uint16 i)
{
	switch(i)
	{
		case 0:
			SET_BIT1;	//ѡͨ��������λ
			RST_BIT2;	//�ض����������λ
			RST_BIT3;
			RST_BIT4;
			break;

		case 1:
			RST_BIT1;
			SET_BIT2;  //ѡͨ����ܵڶ�λ
			RST_BIT3;
			RST_BIT4;
			break;

		case 2:
			RST_BIT1;
			RST_BIT2;
			SET_BIT3;  //ѡͨ����ܵ���λ
			RST_BIT4;
			break;

		case 3:
			RST_BIT1;
			RST_BIT2;
			RST_BIT3;
			SET_BIT4;  //ѡͨ��������λ
			break;

		default:
			break;
	}
}
/*****************************************************************************************************/

/************************** ���Ҫ��ʾ����λ�����浽����DispDataSplit����*****************************/
// 4λ�����Ϊ4���������浽����DispDataBit[4],DispDataSplit[0]Ϊ��λ
void DispDataSplit(Uint16 data)
{
	DispDataBit[3] = data / 1000;						//ǧλ��
	DispDataBit[2] = data % 1000 / 100 ;				//��λ��
	DispDataBit[1] = data % 100 / 10;					//ʮλ��
	DispDataBit[0] = data % 10;							//��λ��
}
/*****************************************************************************************************/

/*****************************************************************************************************/
//
/*********************************************Spi����************************************************/
// ������a���͵�SPITXBUF
void SpiXmitData(Uint16 a)
{
    SpiaRegs.SPITXBUF=a;
}    


void Refresh_8SEG(void)
{
	//static Uint16 Loop=0;
	SellectSeg(Loop_8SEG);								//ѡ��Ҫɨ��������λ
	SpiXmitData(Disp_8SEG_Code[Loop_8SEG]);					//�������Ҫ��ʾ������
	Loop_8SEG++;Loop_8SEG%=4;
}

// �ַ�0~9�ı���Ϊ��0x30~0x39;
// �ַ�A~Z�ı���Ϊ��0x41~0x5A;
// �ַ�a~z�ı���Ϊ��0x61~0x7A;
//unsigned char msg[10]={0xC0,0xf9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90};	//�������룺0~9
//unsigned char msg2_ABC[6]={0x88, 0x80, 0xc6, 0xc0, 0x86, 0x8e};	//�������룺A~F
//unsigned char msg2_abc[6]={0xa0, 0x83, 0xa7, 0xa1, 0x84, 0x8e};	//�������룺a~f

void GetSymCode(unsigned char*  DispSym)
{
	unsigned int i=0;

	for (i=0;i<4;i++)
	{
		if (DispSym[i]>0x60)
			Disp_8SEG_Code[3-i]=msg2_abc[DispSym[i]-'a']; // Сд��ĸ
		else if (DispSym[i]>0x40)
			Disp_8SEG_Code[3-i]=msg2_ABC[DispSym[i]-'A']; // ��д��ĸ
		else
			Disp_8SEG_Code[3-i]=msg[DispSym[i]-'0'];      // ����
	}

}

// �õ�4λ�Ǹ��������ַ�����
void GetNumCode(Uint16 DispData)
{
	Uint16 loop;

	DispDataSplit(DispData);         // ���Ϊ4�����֣�����DispDataBit[4]��
	for (loop=0;loop<4;loop++)   // ��ѯ4λ�����ֱ���
	{
		Disp_8SEG_Code[loop]=msg[DispDataBit[loop]];
	}
}

// �õ�4λ���������ַ�����
void GetNNumCode(int16 DispData)
{
	Uint16 loop;
	Uint16 tmp;

	if (DispData<0)
	{
		tmp=-DispData;
	}

	DispDataSplit(tmp);         // ���Ϊ4�����֣�����DispDataBit[4]��
	for (loop=0;loop<4;loop++)   // ��ѯ4λ�����ֱ���
	{
		Disp_8SEG_Code[loop]=msg[DispDataBit[loop]];
	}

	if (DispData<0)
		Disp_8SEG_Code[3]=msg3_N[0];
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;

    if (CpuTimer0.InterruptCount==(Uint16)tintfreq)//ÿ�����ʱ������ʾ�ַ�����
    {
    	CpuTimer0.InterruptCount=0;

		// ����������ʾ����DispData
		DispData++;	DispData%=10000;     // DispData�Զ���1�����9999��
		GetNumCode(DispData);


    	//GetNNumCode(Nnum);  // Nnum=-3 ��ʾ����
    	//GetSymCode(DispSym);// ��ʾDispSym�е��ַ�

		//DispData++;	DispData%=6;
    	//GetSymCode(Syms[DispData]);    // ��ʾSyms�е��ַ�

		//DispData++;	DispData%=6;
    	//GetSymCode(Syms2[DispData]);    // ��ʾSyms2�е��ַ�
   }

    Refresh_8SEG();

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1; //�ز����٣�������������ж�
}


//===========================================================================
// No more.
//===========================================================================

