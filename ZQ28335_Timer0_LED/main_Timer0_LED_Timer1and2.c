//程序功能：
//   用TIMER0控制GPIO0~3端口LED亮灭
//   用TIMER1控制GPIO4端口LED亮灭
//控制方式：
//       反相控制，L电平LED亮
//硬件电路：
//    8个LED正极接3.3v,负极接限流电阻后分别接到GPIO0~5/GPIO12/GPIO36

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "LED_M.h"               // LED状态宏定义
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

    // Step 1. 系统控制初始化
    InitSysCtrl();

    // Step 2. 引脚初始化
    InitLedGpio();

    // Step 3. 清除所有中断、初始化PIE向量表，关闭cpu中断
    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    // 初始化中断向量，本例有2个中断
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    PieVectTable.XINT13 = &cpu_timer1_isr;  //XINT13;    // XINT13 / CPU-Timer1
    PieVectTable.TINT2 = &cpu_timer2_isr;  // CPU-Timer2
    EDIS;

    Count = 0;                  //初始化变量
    Flag=0;

    // Step 4. 初始化TIMER0和TIMER1
    InitCpuTimers();

    #if (CPU_FRQ_150MHZ)
    ConfigCpuTimer(&CpuTimer0, 150, 1000000);
    ConfigCpuTimer(&CpuTimer1, 150, 1000);  //Timer1中断周期为2秒
    ConfigCpuTimer(&CpuTimer2, 150, 4000000);  //Timer2中断周期为4秒
    #endif

    #if (CPU_FRQ_100MHZ)
    ConfigCpuTimer(&CpuTimer0, 100, 500000);
    ConfigCpuTimer(&CpuTimer1, 100, 2000000);  //Timer1中断周期为2秒
    ConfigCpuTimer(&CpuTimer2, 100, 4000000);  //Timer2中断周期为4秒
    #endif


    StartCpuTimer0(); // 或CpuTimer0Regs.TCR.all = 0x4001;
    StartCpuTimer1();
    StartCpuTimer2();

    // 使能INT1.7(TINT0在INT1的第7个)和INT13(timer1 and xint13)
    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT13;    // timer1 and xint13
    IER |= M_INT14;    // timer2

    // 使能全局中断和实时中断
    EINT;
    ERTM;
    InitStepMotorGpio();
    InitKeyGpio(); // 按键引脚初始化
    // Step 6. IDLE a%8,wait interrupt.
    while(1){
        //    MotionControl3();
              /*  key_value=GetKey();  // 得到按键Key的值
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
       PhaseOn (StepMode1[step_freq%8]  & 0x000F);        //第一位：控制信号高电平判断
       PhaseOn ((StepMode1[step_freq%8] & 0x00F0)>>4);    //第二位：控制信号高电平判断
       PhaseOff((StepMode1[step_freq%8] & 0x0F00)>>8);    //第三位：控制信号低电平判断
       PhaseOff((StepMode1[step_freq%8] & 0xF000)>>12);   //第四位：控制信号低电平判断break;
       break;
   case 1:
       PhaseOn (StepMode1[step_freq%8]  & 0x000F);        //第一位：控制信号高电平判断
             PhaseOn ((StepMode1[step_freq%8] & 0x00F0)>>4);    //第二位：控制信号高电平判断
             PhaseOff((StepMode1[step_freq%8] & 0x0F00)>>8);    //第三位：控制信号低电平判断
             PhaseOff((StepMode1[step_freq%8] & 0xF000)>>12);   //第四位：控制信号低电平判断break;
       break;
   case 2:
       PhaseOn (StepMode3[step_freq%8] & 0x000F);         //第一位：控制信号高电平判断
       PhaseOn ((StepMode3[step_freq%8] & 0x00F0)>>4);    //第二位：控制信号高电平判断
       PhaseOff((StepMode3[step_freq%8] & 0x0F00)>>8);    //第三位：控制信号低电平判断
       PhaseOff((StepMode3[step_freq%8] & 0xF000)>>12);   //第四位：控制信号低电平判断break;
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
