/*
 * F28335椹卞姩LCD12864
 */

#include "F28x_Project.h"
#include "LCD12864.h"
#include "KEY4x4.h"
#include "Delay.h"

/******************************************************************************************/
void main(void)
{
   InitSysCtrl();

   InitLcdGpio(); // 鎸夐敭寮曡剼鍒濆鍖栵紝浣跨敤GPIO2~5銆�
   InitKeyGpio(); // �������ų�ʼ��

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


