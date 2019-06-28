/*
 * BUZZ.c
 *
 *  Created on: 2019��6��18��
 *      Author: L9379
 */

#include "BUZZ.h"
extern unsigned char key_val; //��ȡ��ֵ
extern unsigned char real_val;//����Ƶ��
extern unsigned char buzz_flag;//������Ĵ���
/*****************************************��ʼ��IO�˿�************************************************/
void InitBuzzGpio(void)
{
    EALLOW;
    GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 0;                                    // GPIO35 = GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO35 = 1;   // GPIO35 = output
    GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;                                     // Enable pullup on GPIO35
    GpioDataRegs.GPBSET.bit.GPIO35 = 1;                                      // Load output latch
    EDIS;
}


interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   if(real_val==1)
   {
   if(CpuTimer0.InterruptCount<500)
       GEN_BUZZ_CLK;
   else if(CpuTimer0.InterruptCount<1000)
       {
       GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
       }
   else{
       CpuTimer0.InterruptCount=0;
       buzz_flag++;
       }
   }
   if(real_val==2){
      if( CpuTimer0.InterruptCount<1000)
          GEN_BUZZ_CLK;
      else if( CpuTimer0.InterruptCount<2000)
          GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1;
      else{
          CpuTimer0.InterruptCount=0;
          buzz_flag++;
      }
      }
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
   if(buzz_flag==3)
              StopCpuTimer0();
}
