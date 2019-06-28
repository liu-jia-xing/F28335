/********************************************************************
* 文件名： 	按键扫描程序
* 描述:  	运行程序，按下按键，观测变量Key的值（该值从从1~9,无键按下时为0)。
*       行线编号：1-3，未按下时编号为0
*       列线编号：1-3，未按下时编号为0
* 调用函数: ReadKey();

* 使用资源：
* 行线KX1~KX3，接入F28335的GPI053~GPI051
* 行线KY1~KY3，接入F28335的GPI050~GPI048

**********************************************************************/
/********************************************************************
程序说明：按键扫描原理：1、3列对应的IO设置为输出,3行对应的IO设置为输入。
				2、若无按键按下，3行输入IO，均为高电平（因为有外部上拉电阻）
				3、若按键按下，对应行IO被拉低（假设为第X行），检测为低电平。按键所在行（X行），被鉴别出来。
				4、此时，依次改变3列的输出为高，当遇到按键所在列时，第X行电平重新变为高。按键所在列。被鉴别出来。
********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "LED_M.h"               // LED状态宏定义
#include "LED.h"
#include "KEY.h"
#include "LCD12864.h"
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;
void main(void)
{
   InitSysCtrl();

   InitKeyGpio(); // 按键引脚初始化
   //InitLedGpio();
   InitLcdGpio(); // 按键引脚初始化，使用GPIO2~5。
   InitLcd();
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();
   MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
   InitFlash();

   while(1)	//死循环
	{
		GetKey();  // 得到按键Key的值
		TestLcd();

		// 补充按键功能

		DSP28x_usDelay(100000);  // 两次检测之间需0.1s间隔以上，才能准确检测，可以执行其他代码作延迟。
	}
}

//===========================================================================
// No more.
//===========================================================================

