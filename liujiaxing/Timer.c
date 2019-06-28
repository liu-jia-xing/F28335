//�����ܣ�
//   LED1~LED4��Timer0�����¶�ʱ��˸����ʱ�ж�����Ϊ0.5�롣
//���Ʒ�ʽ��
//   ��ʱ�жϿ���
//Ӳ����·��
//    GPIO0~3�ֱ��LED1~4.

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "LED_M.h"               // LED״̬�궨��
#include "LED.h"
#include "LCD12864.h"
#include "KEY.h"
#include "BUZZ.h"
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern unsigned char key_val; //��ȡ��ֵ
extern unsigned char real_val;//����Ƶ��
extern unsigned char buzz_flag;//������Ĵ���
//void key_deal(unsigned char com);


void main(void)
{
	InitSysCtrl();
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	InitFlash();
	InitLedGpio(); // LED���ų�ʼ��
	InitKeyGpio(); // �������ų�ʼ��
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	InitLcdGpio(); // �������ų�ʼ����ʹ��GPIO2~5��
	InitLcd();
	InitBuzzGpio();
	// ��ʼ��TIMER0����
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;
	InitCpuTimers();
	//prd0=1/(freq0*2);     // һ��ʱ�����ڣ�ǰ��ΪH��ƽ�����ΪL��ƽ��
	ConfigCpuTimer(&CpuTimer0, 150,1000);//��ʱ���ڵ�λ��us
	IER |= M_INT1;     // ʹ��TINT0(TINT0��INT1�ĵ�7��)
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	EINT;  // ʹ��ȫ���ж�(EINT)
	ERTM;  // ʹ��ʵʱ�ж�(ERTM)
	StartCpuTimer0(); // ������ʱ��
	init_led();
	 TestLcd();
	while(1)
	{
	    key_val= GetKey();  // �õ�����Key��ֵ
	    key_deal(key_val);

	}
}

