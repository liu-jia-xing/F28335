//�����ܣ�
//   ��XINT1����GPIO0��1�˿ڵ�LED������,ÿ��һ��xint1����������LED�������ͷ�תһ�Ρ�
//���Ʒ�ʽ��
//   ���жϿ���,�����ͷ�ʱ������������
//Ӳ����·��
//    GPIO15���Ž��а�������������ʱ������ΪL.

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "LED_M.h"               // LED״̬�궨��
#include "LED.h"
#include "Xint1.h"
//
//#define CloseXint1   PieCtrlRegs.PIEIER1.bit.INTx4 = 0      // XINT1���ж�ʹ��λ
//#define OpenXint1    PieCtrlRegs.PIEIER1.bit.INTx4 = 1
//#define ClearXint1Flag   PieCtrlRegs.PIEIFR1.bit.INTx4 = 0  // XINT1���жϱ�־λ
//#define XINT1_STATUS	 GpioDataRegs.GPADAT.bit.GPIO15     // GPIO15������λ�����ڼ�ⰴ���Ƿ��ͷ�
//
//Uint16 XintInput1;  // ��ʱ���������ڼ�ⰴ���Ƿ��ͷ�
//Uint16 XintInput2;  // ��ʱ���������ڼ�ⰴ���Ƿ��ͷ�
//
//volatile Uint32 Xint1Count;  // ���ж�1������������δʹ��
//
//void Xint1GPIOConfig(); //xint1��������
//interrupt void xint1_isr(void);
unsigned char interrupt_flag=0;
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
void main(void)
��{
   InitSysCtrl();

   InitLedGpio();  //led���ų�ʼ��
   Xint1GPIOConfig(); //xint1��������

   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   EALLOW;
   PieVectTable.XINT1 = &xint1_isr; //xint1_isr������ڵ�ַ���뵽�������XINT1λ��
   EDIS;
	
   // ʹ��XINT1�ж�,��INT1.4��
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Gropu 1 INT4

   // ʹ��INT1���ж�
   IER |= M_INT1;                              // Enable CPU int1
   EINT;                                       // Enable Global Interrupts
   ERTM;
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
   InitFlash();
   for(;;);
}
//
//interrupt void xint1_isr(void)
//{
//	Xint1Count;
//
//	CloseXint1;// �ر����ж�1, ִ���жϺ���ʱһ�㲻�������
//	// ���жϹ���
//	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1; // GPIO0 ��ƽ��ת
//    GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1; // GPIO1 ��ƽ��ת
//
//    while(1)  // �ȴ������ͷţ����������Ϊ������
//    {
//    	XintInput1=XINT1_STATUS;  // ��XINT1������ֵ
//    	DSP28x_usDelay(10000);    // �Ե�
//		XintInput2=XINT1_STATUS;  // �ٶ�XINT1������ֵ
//		if (XintInput1==1 && XintInput2==1)  // �����ξ�ΪH����ʾ�������ͷ�
//			break;
//    }
//    ClearXint1Flag;  // ������е����ж�1�жϱ�־�����������ж�
//    OpenXint1;       // ���жϣ��ȴ��´��ж�
//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//}
//
////xint1��������
//void Xint1GPIOConfig()
//{
//   // xint1���GPIO15���Ž��룬����GPIO15��������
//   EALLOW;
//   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;       // GPIO
//   GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;        // GPIO15=input
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 2;      // XINT2 Qual using 6 samples
//   GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 0x80;  // Each sampling window is 128*SYSCLKOUT
//
//   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 15; // Xint1 is GPIO15��GPIOXINT1SEL��5λ�Ĵ�����0-31
//   // Configure XINT1������XINT1�����ԣ������������ԡ�ʹ�ܵȣ�
//   // POLARITY = 0��2(�½���)��1(������)��3(�����غ��½���)��
//   XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // Falling edge interrupt
//   XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable Xint1
//   EDIS;
//}
//

//===========================================================================
// No more.
//===========================================================================
