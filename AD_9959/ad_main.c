#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File
#include "AD_9959.h"

void main()
{
  //static unsigned char DATA[5]={0x66,0x67,0x68,0x69};
   InitSysCtrl();
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();
   AD9959_Init();
   while(1)
   {
       //WriteData_AD9959(0x01,3,DATA,1);
   }
}
