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
#include "LCD12864.h"
#include "KEY.h"
#include "BUZZ.h"
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
extern unsigned char key_val; //获取键值
extern unsigned char real_val;//控制频率
extern unsigned char buzz_flag;//控制响的次数
//void key_deal(unsigned char com);


void main(void)
{
	InitSysCtrl();
	MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
	InitFlash();
	InitLedGpio(); // LED引脚初始化
	InitKeyGpio(); // 按键引脚初始化
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	InitLcdGpio(); // 按键引脚初始化，使用GPIO2~5。
	InitLcd();
	InitBuzzGpio();
	// 初始化TIMER0功能
	EALLOW;
	PieVectTable.TINT0 = &cpu_timer0_isr;
	EDIS;
	InitCpuTimers();
	//prd0=1/(freq0*2);     // 一个时钟周期，前半为H电平，后半为L电平。
	ConfigCpuTimer(&CpuTimer0, 150,1000);//定时周期单位：us
	IER |= M_INT1;     // 使能TINT0(TINT0在INT1的第7个)
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
	EINT;  // 使能全局中断(EINT)
	ERTM;  // 使能实时中断(ERTM)
	StartCpuTimer0(); // 启动定时器
	init_led();
	 TestLcd();
	while(1)
	{
	    key_val= GetKey();  // 得到按键Key的值
	    key_deal(key_val);

	}
}

