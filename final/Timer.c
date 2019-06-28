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
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
#define GEN_BUZZ_CLK    GpioDataRegs.GPBTOGGLE.bit.GPIO35 = 1   //����������IO��IO��ƽ��ת��������������
#define BUZZ_OFF        GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1    //�رշ�����

unsigned char key_val=0;
unsigned char real_val=0;
interrupt void cpu_timer0_isr(void);
void init_led();
unsigned char buzz_flag=0;
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
	while(1)
	{
	    key_val= GetKey();  // �õ�����Key��ֵ
	    key_deal(key_val);
	    TestLcd();
	    // ���䰴������
	/*    if(buzz_flag==3)
	        StopCpuTimer0();*/
	   // DSP28x_usDelay(100000);  // ���μ��֮����0.1s������ϣ�����׼ȷ��⣬����ִ�������������ӳ١�
	}
}

/*****************************************��ʼ��IO�˿�************************************************/
void InitBuzzGpio(void)
{
    EALLOW;
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;                                    // GPIO35 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;   // GPIO35 = output
    GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;                                     // Enable pullup on GPIO35
    GpioDataRegs.GPBSET.bit.GPIO35 = 1;                                      // Load output latch
    EDIS;
}

interrupt void cpu_timer0_isr(void)
{    unsigned char a=0;
   CpuTimer0.InterruptCount++;
   if(real_val==1)
   {
   if(CpuTimer0.InterruptCount<500)
       GEN_BUZZ_CLK;
   else if(CpuTimer0.InterruptCount<1000)
       {
       GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
       }
   else{
       CpuTimer0.InterruptCount=0;
       buzz_flag++;
       }
   }
   if(real_val==2){
      if( CpuTimer0.InterruptCount<1000)
          GEN_BUZZ_CLK;
      else if( CpuTimer0.InterruptCount<2000)
          GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
      else{
          CpuTimer0.InterruptCount=0;
          buzz_flag++;
      }
      }
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   if(buzz_flag==3)
              StopCpuTimer0();
}

void key_deal(unsigned char com)
{
    switch(com)
    {
    case 1:StopCpuTimer0();
            ConfigCpuTimer(&CpuTimer0, 150,500);//��ʱ���ڵ�λ��us
            LED1_ON;
            LED2_OFF;
            StartCpuTimer0();
            buzz_flag=0;
            real_val=1;
            DispENG(0,6,"1",1);
        break;
    case 2:
        LED1_OFF;
        LED2_ON;
        StopCpuTimer0();
        ConfigCpuTimer(&CpuTimer0, 150,250);//��ʱ���ڵ�λ��us
        StartCpuTimer0();
        real_val=2;
        buzz_flag=0;
        DispENG(0,6,"2",1);
        break;
    case 4:StopCpuTimer0();
    init_led();
    DispENG(0,6,"0",1);break;
    default:break;
    }
}
void init_led()
{
    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
    LED5_OFF;
    LED6_OFF;
    LED7_OFF;
}

