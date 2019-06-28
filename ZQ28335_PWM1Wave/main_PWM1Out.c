/* 设置PWM1的频率为200Hz，设置CMPA和CMPB的值，
 * 改变PWM1A和PWM1B路的占空比，从而改变LED1和LED2的亮度
 * SYSCLK时钟经HSPCLKDIV和CLKDIV分频后，得到TBCLK
 * 增计数，设置PWM1_Freq改变频率，设置CMPA和CMPB改变占空比，LED驱动，L有效；
 * 本程序修改测试通过，江金龙，20111129
 */


#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "PWM1Out.h"


void main(void)
{
   InitSysCtrl();

   // EPWM模块每组输出2路波形，分别用A和B区分，默认它们反相；
   // EPWM1模块的EPWM1A输出引脚是GPIO0, EPWM1B输出引脚是GPIO1
   InitEPwm1Gpio();    // 若不需要输出波形，可不初始化该模块引脚
 
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   SetupEPwm1Parm();

    for(;;);
}
//===========================================================================
// No more.
//===========================================================================
