/*
 * F28335驱动LCD12864
 */

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "LCD12864.h"
#include "KEY.h"
#include "LED.h"
/******************************************************************************************/
void main(void)
{

    InitSysCtrl();

   InitLcdGpio(); // 按键引脚初始化，使用GPIO2~5。

   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();
   InitKeyGpio(); // 按键引脚初始化
   InitLcd();

   for(;;)
   {   
       TestLcd();
       GetKey();  // 得到按键Key的值

       // 补充按键功能
       //  key_show(GetKey());

       DSP28x_usDelay(100000);  // 两次检测之间需0.1s间隔以上，才能准确检测，可以执行其他代码作延迟。
   }
} 	

//===========================================================================
// No more.
//===========================================================================


