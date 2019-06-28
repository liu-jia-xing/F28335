//程序功能：
//   用TIMER0控制GPIO35端口的蜂鸣器(BUZZ),使之发出一定频率的告警声
//控制方式：
//       同相控制，H电平时BUZZ导通，高低电平变化时发出一定的声音
//硬件电路：
//    GPIO35输出经反相电路后加在BUZZ上

#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"

#define GEN_BUZZ_CLK	GpioDataRegs.GPBTOGGLE.bit.GPIO35 = 1 	//蜂鸣器控制IO，IO电平翻转，产生控制脉冲
#define BUZZ_OFF		GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1 	//关闭蜂鸣器
float freq0=1000;   // 定时器0的中断频率（Hz）
float prd0=0;      // 定时器0的中断周期(sec)=1/freq0/2,对于方波，一个周期要中断2次

void InitBuzzGpio(void);
interrupt void cpu_timer0_isr(void);

void main(void)
{

	// Step 1. 系统控制初始化
	InitSysCtrl();

   // 蜂鸣器（Buzz）引脚初始化
   InitBuzzGpio();

	// Step 3. 清除所有中断、初始化PIE向量表，关闭cpu中断
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();

	// 初始化TIMER0功能
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;
	InitCpuTimers();
	prd0=1/(freq0*2);     // 一个时钟周期，前半为H电平，后半为L电平。
	ConfigCpuTimer(&CpuTimer0, 150, 1000);//定时周期单位：us
	IER |= M_INT1;     // 使能TINT0(TINT0在INT1的第7个)
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	EINT;  // 使能全局中断(EINT)
	ERTM;  // 使能实时中断(ERTM)

	StartCpuTimer0(); // 启动定时器0

	for(;;);  // 或while(1);  死循环，不能让CPU停下来
}

/*****************************************初始化IO端口************************************************/
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
   GEN_BUZZ_CLK; // 当定时周期为500us，产生的方波频率为1kHz
   else if( CpuTimer0.InterruptCount<2000)
       GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
   else
       CpuTimer0.InterruptCount=0;

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

//===========================================================================
// No more.
//===========================================================================
