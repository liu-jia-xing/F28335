/*
 * step.c
 *
 *  Created on: 2019年5月20日
 *      Author: L9379
 */
#include "step.h"
//#include "DSP2833x_Device.h"
//#include "DSP2833x_Examples.h"


//单四拍时序A-B-C-D
// A相位H时，它的前一相（D）要恢复L,依次类推；
const Uint16 StepMode1[8]     = {0x0D0A,0x0D0A,0x0A0B,0x0A0B,0x0B0C,0x0B0C,0x0C0D,0x0C0D};      //单四拍时序A-B-C-D-A
const Uint16 StepMode1_opposite[8]= {0x0B0A,0x0B0A,0x0A0D,0x0A0D,0x0D0C,0x0D0C,0x0C0B,0x0C0B};      //单四拍时序D-C-B-A-D
//双四拍时序AB-BC-CD-DA-AB
const Uint16 StepMode2[8] = {0xCDAB,0xDABC,0xABCD,0xBCDA,0xCDAB,0xDABC,0xABCD,0xBCDA};      //双四拍时序AB-BC-CD-DA-AB
//八拍时序A-AB-B-BC-C-CD-D-DA-A
const Uint16 StepMode3[8] = {0x0D0A,0x0DAB,0x0A0B,0x0ABC,0x0B0C,0x0BCD,0x0C0D,0x0CDA};
void InitStepMotorGpio(void)
{
    EALLOW;
    //Phase A
    GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;                                      // Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO8 = 1;                                      // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;                                     // GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;                                      // GPIO11 = output
    //Phase B
    GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO13 = 1;                                     // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;                                     // GPIO11 = output
    //Phase C
    GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO10 = 1;                                     // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;                                     // GPIO11 = output
    //Phase D
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;                                     // Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO14 = 1;                                     // Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;                                    // GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;                                     // GPIO11 = output

    EDIS;
    PHA_OFF;
    PHB_OFF;
    PHC_OFF;
    PHD_OFF;

}


/*延时函数************************************************/
void delay(Uint32 t)
{
    Uint32 i = 0;
    for (i = 0; i < t; i++);
}
/********************************************************/

// 指定第i相导通，IO输出H电平，ULN2003反相后给绕组供电。
void PhaseOn(Uint16 i)
{
    switch(i)
    {
        case 0xA:
                PHA_ON; break;
        case 0xB:
                PHB_ON; break;

        case 0xC:
                PHC_ON; break;
        case 0xD:
                PHD_ON; break;
        default:        break;
    }
}

// 指定第i相断开，IO输出L电平，ULN2003反相后绕组供电截止。
void PhaseOff(Uint16 i)
{
    switch(i)
    {
        case 0xA:
                PHA_OFF; break;
        case 0xB:
                PHB_OFF; break;

        case 0xC:
                PHC_OFF; break;
        case 0xD:
                PHD_OFF; break;
        default:         break;
    }
}

/******************************************单四拍控制时序*********************************************/
//单四拍时序A-B-C-D-A
// const Uint16 StepMode1[8] = {0x0D0A,0x0D0A,0x0A0B,0x0A0B,0x0B0C,0x0B0C,0x0C0D,0x0C0D};       //单四拍时序A-B-C-D-A
void MotionControl1(void)
{
    Uint16 loop = 0;

    for(loop = 0;loop<=7;loop++)
    {
        PhaseOn (StepMode1[loop]  & 0x000F);        //第一位：控制信号高电平判断
        PhaseOn ((StepMode1[loop] & 0x00F0)>>4);    //第二位：控制信号高电平判断
        PhaseOff((StepMode1[loop] & 0x0F00)>>8);    //第三位：控制信号低电平判断
        PhaseOff((StepMode1[loop] & 0xF000)>>12);   //第四位：控制信号低电平判断
        delay(STEP_TIME);
        //timer_flag=0;
    //    while(timer_flag>100);
        GpioDataRegs.GPATOGGLE.bit.GPIO0=1;
    }
}

void MotionControl1_opposite(void)
{
    Uint16 loop = 0;

    for(loop = 0;loop<=7;loop++)
    {
        PhaseOn (StepMode1_opposite[loop]  & 0x000F);        //第一位：控制信号高电平判断
        PhaseOn ((StepMode1_opposite[loop] & 0x00F0)>>4);    //第二位：控制信号高电平判断
        PhaseOff((StepMode1_opposite[loop] & 0x0F00)>>8);    //第三位：控制信号低电平判断
        PhaseOff((StepMode1_opposite[loop] & 0xF000)>>12);   //第四位：控制信号低电平判断
      // delay(STEP_TIME);
     //   while(timer_flag==0);
      //  timer_flag=0;
    }
}
/*****************************************************************************************************/

/******************************************双四拍控制时序*********************************************/
//双四拍时序AB-BC-CD-DA-AB
// const Uint16 StepMode2[8] = {0xCDAB,0xDABC,0xABCD,0xBCDA,0xCDAB,0xDABC,0xABCD,0xBCDA};       //双四拍时序AB-BC-CD-DA-AB
void MotionControl2(void)
{
    Uint16 loop = 0;
    for(loop = 0;loop<=7;loop++)
    {
        PhaseOff((StepMode2[loop] & 0x0F00)>>8);                    //第三位：控制信号低电平判断
        PhaseOff((StepMode2[loop] & 0xF000)>>12);                   //第四位：控制信号低电平判断
        delay(STEP_TIME);
        PhaseOn (StepMode2[loop] & 0x000F);                         //第一位：控制信号高电平判断
        PhaseOn ((StepMode2[loop] & 0x00F0)>>4);                    //第二位：控制信号高电平判断
        //delay(STEP_TIME);
    }
}

/*****************************************************************************************************/

/******************************************八拍控制时序*********************************************/
//八拍时序A-AB-B-BC-C-CD-D-DA-A
// const Uint16 StepMode3[8] = {0x0D0A,0x0DAB,0x0A0B,0x0ABC,0x0B0C,0x0BCD,0x0C0D,0x0CDA};       //八拍时序A-AB-B-BC-C-CD-D-DA-A
void MotionControl3(void)
{
    Uint16 loop = 0;
    for(loop = 0;loop<=7;loop++)
    {
        PhaseOn (StepMode3[loop] & 0x000F);         //第一位：控制信号高电平判断
        PhaseOn ((StepMode3[loop] & 0x00F0)>>4);    //第二位：控制信号高电平判断
        PhaseOff((StepMode3[loop] & 0x0F00)>>8);    //第三位：控制信号低电平判断
        PhaseOff((StepMode3[loop] & 0xF000)>>12);   //第四位：控制信号低电平判断
        delay(STEP_TIME);
    }
}

