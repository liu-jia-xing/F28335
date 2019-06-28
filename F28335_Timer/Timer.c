//程序功能：
//   LED1~LED4在Timer0控制下定时闪烁，定时中断周期为0.5秒。
//控制方式：
//   定时中断控制
//硬件电路：
//    GPIO0~3分别接LED1~4.

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "LED_M.h"               // LED状态宏定义
#include "LED.h"
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
interrupt void cpu_timer0_isr(void);

void main(void)
{
	InitSysCtrl();

	InitLedGpio(); // LED引脚初始化

	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();

	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;

	InitCpuTimers();

	#if (CPU_FRQ_150MHZ)
	ConfigCpuTimer(&CpuTimer0, 150, 500000);
	#endif
	#if (CPU_FRQ_100MHZ)
	ConfigCpuTimer(&CpuTimer0, 100, 500000);
	#endif

	StartCpuTimer0();

	// 使能TINT0(在INT1.7)
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;


	EINT;  // 使能全局中断
	ERTM;  // 使能实时中断
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	InitFlash();
	for(;;);
}


interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;

   GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1; // Toggle GPIO0 once per 500 milliseconds
   GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1; // Toggle GPIO1 once per 500 milliseconds
   GpioDataRegs.GPATOGGLE.bit.GPIO2 = 1; // Toggle GPIO2 once per 500 milliseconds
   GpioDataRegs.GPATOGGLE.bit.GPIO3 = 1; // Toggle GPIO3 once per 500 milliseconds

   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}


//===========================================================================
// No more.
//===========================================================================
