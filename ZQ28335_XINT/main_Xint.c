//程序功能：
//   用XINT1控制GPIO0和1端口的LED灯亮暗,每按一次xint1按键，两个LED的亮暗就翻转一次。
//控制方式：
//   外中断控制,按键释放时输出按键检测结果
//硬件电路：
//    GPIO15引脚接有按键，按键按下时该引脚为L.

#include "DSP2833x_Device.h"     // Headerfile Include File
#include "DSP2833x_Examples.h"   // Examples Include File
#include "LED_M.h"               // LED状态宏定义
#include "LED.h"
#include "Xint1.h"
//
//#define CloseXint1   PieCtrlRegs.PIEIER1.bit.INTx4 = 0      // XINT1的中断使能位
//#define OpenXint1    PieCtrlRegs.PIEIER1.bit.INTx4 = 1
//#define ClearXint1Flag   PieCtrlRegs.PIEIFR1.bit.INTx4 = 0  // XINT1的中断标志位
//#define XINT1_STATUS	 GpioDataRegs.GPADAT.bit.GPIO15     // GPIO15的输入位，用于检测按键是否释放
//
//Uint16 XintInput1;  // 临时变量，用于检测按键是否释放
//Uint16 XintInput2;  // 临时变量，用于检测按键是否释放
//
//volatile Uint32 Xint1Count;  // 外中断1计数器，本例未使用
//
//void Xint1GPIOConfig(); //xint1引脚配置
//interrupt void xint1_isr(void);
unsigned char interrupt_flag=0;
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
void main(void)
。{
   InitSysCtrl();

   InitLedGpio();  //led引脚初始化
   Xint1GPIOConfig(); //xint1引脚配置

   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   EALLOW;
   PieVectTable.XINT1 = &xint1_isr; //xint1_isr函数入口地址加入到向量表的XINT1位置
   EDIS;
	
   // 使能XINT1中断,在INT1.4中
   PieCtrlRegs.PIECTRL.bit.ENPIE = 1;          // Enable the PIE block
   PieCtrlRegs.PIEIER1.bit.INTx4 = 1;          // Enable PIE Gropu 1 INT4

   // 使能INT1组中断
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
//	CloseXint1;// 关闭外中断1, 执行中断函数时一般不允许打扰
//	// 外中断功能
//	GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1; // GPIO0 电平翻转
//    GpioDataRegs.GPATOGGLE.bit.GPIO1 = 1; // GPIO1 电平翻转
//
//    while(1)  // 等待按键释放，检测两次是为了消抖
//    {
//    	XintInput1=XINT1_STATUS;  // 读XINT1口输入值
//    	DSP28x_usDelay(10000);    // 稍等
//		XintInput2=XINT1_STATUS;  // 再读XINT1口输入值
//		if (XintInput1==1 && XintInput2==1)  // 若两次均为H，表示按键已释放
//			break;
//    }
//    ClearXint1Flag;  // 清除现有的外中断1中断标志，避免连续中断
//    OpenXint1;       // 打开中断，等待下次中断
//    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
//}
//
////xint1引脚配置
//void Xint1GPIOConfig()
//{
//   // xint1拟从GPIO15引脚接入，配置GPIO15引脚特性
//   EALLOW;
//   GpioCtrlRegs.GPAMUX1.bit.GPIO15 = 0;       // GPIO
//   GpioCtrlRegs.GPADIR.bit.GPIO15 = 0;        // GPIO15=input
//   GpioCtrlRegs.GPAQSEL1.bit.GPIO15 = 2;      // XINT2 Qual using 6 samples
//   GpioCtrlRegs.GPACTRL.bit.QUALPRD0 = 0x80;  // Each sampling window is 128*SYSCLKOUT
//
//   GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 15; // Xint1 is GPIO15，GPIOXINT1SEL：5位寄存器，0-31
//   // Configure XINT1（配置XINT1的特性，包括触发极性、使能等）
//   // POLARITY = 0或2(下降沿)，1(上升沿)，3(上升沿和下降沿)，
//   XIntruptRegs.XINT1CR.bit.POLARITY = 0;      // Falling edge interrupt
//   XIntruptRegs.XINT1CR.bit.ENABLE = 1;        // Enable Xint1
//   EDIS;
//}
//

//===========================================================================
// No more.
//===========================================================================
