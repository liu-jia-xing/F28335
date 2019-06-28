//�����ܣ�
//   ��TIMER0����GPIO35�˿ڵķ�����(BUZZ),ʹ֮����һ��Ƶ�ʵĸ澯��
//���Ʒ�ʽ��
//       ͬ����ƣ�H��ƽʱBUZZ��ͨ���ߵ͵�ƽ�仯ʱ����һ��������
//Ӳ����·��
//    GPIO35����������·�����BUZZ��

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#define GEN_BUZZ_CLK	GpioDataRegs.GPBTOGGLE.bit.GPIO35 = 1 	//����������IO��IO��ƽ��ת��������������
#define BUZZ_OFF		GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1 	//�رշ�����
float freq0=1000;   // ��ʱ��0���ж�Ƶ�ʣ�Hz��
float prd0=0;      // ��ʱ��0���ж�����(sec)=1/freq0/2,���ڷ�����һ������Ҫ�ж�2��

void InitBuzzGpio(void);
interrupt void cpu_timer0_isr(void);

void main(void)
{

	// Step 1. ϵͳ���Ƴ�ʼ��
	InitSysCtrl();

   // ��������Buzz�����ų�ʼ��
   InitBuzzGpio();

	// Step 3. ��������жϡ���ʼ��PIE�������ر�cpu�ж�
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();

	// ��ʼ��TIMER0����
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;
	InitCpuTimers();
	prd0=1/(freq0*2);     // һ��ʱ�����ڣ�ǰ��ΪH��ƽ�����ΪL��ƽ��
	ConfigCpuTimer(&CpuTimer0, 150, 1000);//��ʱ���ڵ�λ��us
	IER |= M_INT1;     // ʹ��TINT0(TINT0��INT1�ĵ�7��)
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	EINT;  // ʹ��ȫ���ж�(EINT)
	ERTM;  // ʹ��ʵʱ�ж�(ERTM)

	StartCpuTimer0(); // ������ʱ��0

	for(;;);  // ��while(1);  ��ѭ����������CPUͣ����
}

/*****************************************��ʼ��IO�˿�************************************************/
void InitBuzzGpio(void)
{
	EALLOW;
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;  									// GPIO35 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;   // GPIO35 = output
    GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;   									// Enable pullup on GPIO35
    GpioDataRegs.GPBSET.bit.GPIO35 = 1;  									 // Load output latch
    EDIS;
}

interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   if( CpuTimer0.InterruptCount<1000)
   GEN_BUZZ_CLK; // ����ʱ����Ϊ500us�������ķ���Ƶ��Ϊ1kHz
   else if( CpuTimer0.InterruptCount<2000)
       GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
   else
       CpuTimer0.InterruptCount=0;

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//===========================================================================
// No more.
//===========================================================================
