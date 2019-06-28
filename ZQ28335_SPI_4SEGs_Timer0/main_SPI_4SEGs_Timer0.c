/********************************************************************
* 功能：数码管显示程序,定时器刷新，可显示数字、字符及混合形式
*
* 特别说明：
*    函数InitSpiaGpio()默认使用GPIO16-19脚，但本板使用GPIO54-57，需修改。
*    修改自带的函数，不要直接修改代码，修改后的函数也不要放在common文件夹中
*    本函数换成这个函数名，InitSpia54to57Gpio()，放在本文件中
*
* 描述：
* （1）设置数码管刷新频率：tintfreq，单位：Hz，每次刷新一位数码管
*     float tintfreq=200;          // 定时中断频率，Hz
* （2）每个字符的显示时间在CpuTimer0.InterruptCount中控制，默认为1秒（刷新tintfreq次）
*     CpuTimer0.InterruptCount==(Uint16)tintfreq
* （3）需显示的字符放在变量DispSym，顺序与视觉一致
*     unsigned char DispSym[]={"Aa19"};   // A~F,a~f,及0~9混合显示
* （4）函数GetSymCode查询DispSym的段码，默认为1秒更新一次；
* （5）函数GetNumCode查询DispData的段码，默认为1秒更新一次；
*
*硬件电路：SPI接口使用GPIO54-57,连接串并转换芯片74CH164,
*       GPIO54接SIMO,GPIO56接CLK，其他未使用
*       GPIO16-17接SEG1-2,GPIO62-63接SEG3-4;SEG1为高位
*控制方式：
*       定时刷新
*
*  Created on: 2016-11-19
*      Author: 九江学院电子工程学院  江金龙
**********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "SPI_4SEGs_Timer0.h"   // My Include File

interrupt void cpu_timer0_isr(void);

void main(void)
{

	InitSysCtrl();

	InitSpia54to57Gpio();  	//28335  SPIa引脚
	Init4SEGsGpio();        // 初始化4个数码管的片选引脚

	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();

	// 初始化中断向量表
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;

	InitCpuTimers();

	tintprd=1e6/tintfreq;     // 定时中断周期，us
	#if (CPU_FRQ_150MHZ)
	ConfigCpuTimer(&CpuTimer0, 150, tintprd);
	#endif
	#if (CPU_FRQ_100MHZ)
	ConfigCpuTimer(&CpuTimer0, 100, tintprd);
	#endif

	// 使能TINT0(在INT1.7)
	IER |= M_INT1;
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;

	// SPI模块初始化
	InitSpiFun();		   // init SPI
	InitSpiFifo();	   // Initialize the Spi FIFO

	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM

	StartCpuTimer0();  // 启动定时器

	for(;;);
}
