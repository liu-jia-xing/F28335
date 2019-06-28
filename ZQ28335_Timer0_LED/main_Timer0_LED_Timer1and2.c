//�����ܣ�
//   ��TIMER0����GPIO0~3�˿�LED����
//   ��TIMER1����GPIO4�˿�LED����
//���Ʒ�ʽ��
//       ������ƣ�L��ƽLED��
//Ӳ����·��
//    8��LED������3.3v,���������������ֱ�ӵ�GPIO0~5/GPIO12/GPIO36

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "LED_M.h"               // LED״̬�궨��
#include "LED.h"
#include "step.h"
#include "key.h"
interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);
static Uint16 key_value=0;
static Uint16 key_value1=0;
unsigned int Count,Flag;
static Uint32 a=0;
static Uint32 step_freq=0;
extern StepMode3[8];
extern StepMode1[8];
extern StepMode2[8];
void main(void)
{

    // Step 1. ϵͳ���Ƴ�ʼ��
    InitSysCtrl();

    // Step 2. ���ų�ʼ��
    InitLedGpio();

    // Step 3. ��������жϡ���ʼ��PIE�������ر�cpu�ж�
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    // ��ʼ���ж�������������2���ж�
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    PieVectTable.XINT13 = &cpu_timer1_isr;  //XINT13;    // XINT13 / CPU-Timer1
    PieVectTable.TINT2 = &cpu_timer2_isr;  // CPU-Timer2
    EDIS;

    Count = 0;                  //��ʼ������
    Flag=0;

    // Step 4. ��ʼ��TIMER0��TIMER1
    InitCpuTimers();

    #if (CPU_FRQ_150MHZ)
    ConfigCpuTimer(&CpuTimer0, 150, 1000000);
    ConfigCpuTimer(&CpuTimer1, 150, 1000);  //Timer1�ж�����Ϊ2��
    ConfigCpuTimer(&CpuTimer2, 150, 4000000);  //Timer2�ж�����Ϊ4��
    #endif

    #if (CPU_FRQ_100MHZ)
    ConfigCpuTimer(&CpuTimer0, 100, 500000);
    ConfigCpuTimer(&CpuTimer1, 100, 2000000);  //Timer1�ж�����Ϊ2��
    ConfigCpuTimer(&CpuTimer2, 100, 4000000);  //Timer2�ж�����Ϊ4��
    #endif


    StartCpuTimer0(); // ��CpuTimer0Regs.TCR.all = 0x4001;
    StartCpuTimer1();
    StartCpuTimer2();

    // ʹ��INT1.7(TINT0��INT1�ĵ�7��)��INT13(timer1 and xint13)
    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT13;    // timer1 and xint13
    IER |= M_INT14;    // timer2

    // ʹ��ȫ���жϺ�ʵʱ�ж�
    EINT;
    ERTM;
    InitStepMotorGpio();
    InitKeyGpio(); // �������ų�ʼ��
    // Step 6. IDLE a%8,wait interrupt.
    while(1){
        //    MotionControl3();
              /*  key_value=GetKey();  // �õ�����Key��ֵ
                if(key_value1 !=key_value)
                {
                    //key_contrl(key_value1);
                    if(key_value!=0)
                        key_value1=key_value;
                }*/
    }
}


interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   //Uint32 a=CpuTimer0.InterruptCount;
   a++;
   switch(a%4)
   {
   case 0:GpioDataRegs.GPADAT.all=0x000E;break;
   case 1:GpioDataRegs.GPADAT.all=0x000d;break;
   case 2:GpioDataRegs.GPADAT.all=0x000B;break;
   case 3:GpioDataRegs.GPADAT.all=0x0007;break;
   default:break;
   }

  /* GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1; // Toggle GPIO0 once per 500 milliseconds
   GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1; // Toggle GPIO1 once per 500 milliseconds
   GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; // Toggle GPIO2 once per 500 milliseconds
   GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; // Toggle GPIO3 once per 500 milliseconds
   // Acknowledge this interrupt to receive more interrupts from group 1*/
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer1_isr(void)
{
   CpuTimer0.InterruptCount++;
   step_freq++;
   if(step_freq>12228)
       step_freq=0;
   switch(step_freq/4096)
   {
   case 0:
       PhaseOn (StepMode1[step_freq%8]  & 0x000F);        //��һλ�������źŸߵ�ƽ�ж�
       PhaseOn ((StepMode1[step_freq%8] & 0x00F0)>>4);    //�ڶ�λ�������źŸߵ�ƽ�ж�
       PhaseOff((StepMode1[step_freq%8] & 0x0F00)>>8);    //����λ�������źŵ͵�ƽ�ж�
       PhaseOff((StepMode1[step_freq%8] & 0xF000)>>12);   //����λ�������źŵ͵�ƽ�ж�break;
       break;
   case 1:
       PhaseOn (StepMode1[step_freq%8]  & 0x000F);        //��һλ�������źŸߵ�ƽ�ж�
             PhaseOn ((StepMode1[step_freq%8] & 0x00F0)>>4);    //�ڶ�λ�������źŸߵ�ƽ�ж�
             PhaseOff((StepMode1[step_freq%8] & 0x0F00)>>8);    //����λ�������źŵ͵�ƽ�ж�
             PhaseOff((StepMode1[step_freq%8] & 0xF000)>>12);   //����λ�������źŵ͵�ƽ�ж�break;
       break;
   case 2:
       PhaseOn (StepMode3[step_freq%8] & 0x000F);         //��һλ�������źŸߵ�ƽ�ж�
       PhaseOn ((StepMode3[step_freq%8] & 0x00F0)>>4);    //�ڶ�λ�������źŸߵ�ƽ�ж�
       PhaseOff((StepMode3[step_freq%8] & 0x0F00)>>8);    //����λ�������źŵ͵�ƽ�ж�
       PhaseOff((StepMode3[step_freq%8] & 0xF000)>>12);   //����λ�������źŵ͵�ƽ�ж�break;
       break;
   default:step_freq=0;break;
  // PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   }
 //  GpioDataRegs.GPATOGGLE.bit.GPIO4 = 1; // Toggle GPIO0 once per 2000 milliseconds
   //PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer2_isr(void)
{
   CpuTimer0.InterruptCount++;
 //  GpioDataRegs.GPATOGGLE.bit.GPIO5 = 1; // Toggle GPIO0 once per 2000 milliseconds
   //PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
//===========================================================================
// No more.
//===========================================================================
