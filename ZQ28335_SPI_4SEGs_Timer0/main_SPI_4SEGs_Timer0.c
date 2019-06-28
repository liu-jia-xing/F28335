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
#include "SPI_4SEGs_Timer0.h"   // My Include File

interrupt void cpu_timer0_isr(void);

void main(void)
{

	InitSysCtrl();

	InitSpia54to57Gpio();  	//28335  SPIa����
	Init4SEGsGpio();        // ��ʼ��4������ܵ�Ƭѡ����

	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();

	// ��ʼ���ж�������
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;

	InitCpuTimers();

	tintprd=1e6/tintfreq;     // ��ʱ�ж����ڣ�us
	#if (CPU_FRQ_150MHZ)
	ConfigCpuTimer(&CpuTimer0, 150, tintprd);
	#endif
	#if (CPU_FRQ_100MHZ)
	ConfigCpuTimer(&CpuTimer0, 100, tintprd);
	#endif

	// ʹ��TINT0(��INT1.7)
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	// SPIģ���ʼ��
	InitSpiFun();		   // init SPI
	InitSpiFifo();	   // Initialize the Spi FIFO

	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM

	StartCpuTimer0();  // ������ʱ��

	for(;;);
}
