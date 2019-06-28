/*
 * F28335妞瑰崬濮㎜CD12864
 */

#include "F28x_Project.h"
#include "LCD12864.h"
#include "KEY4x4.h"
#include "Delay.h"

/******************************************************************************************/
void main(void)
{
   InitSysCtrl();

   InitLcdGpio(); // 閹稿鏁鏇″壖閸掓繂顫愰崠鏍电礉娴ｈ法鏁PIO2~5閵嗭拷
   InitKeyGpio(); // 按键引脚初始化

   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   InitLcd();

//   TestLcd1();

   while(1)
   {
       GetKey();
       Key_Process();
//       TestLcd();

   }
}

//===========================================================================
// No more.
//===========================================================================


