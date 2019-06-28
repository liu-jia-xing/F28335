/*
 * main.c
 *
 *  Created on: 2019年6月17日
 *      Author: ToFFF
 */
#include "TheHead.h"

#define GEN_BUZZ_CLK    GpioDataRegs.GPBTOGGLE.bit.GPIO35 = 1   //蜂鸣器控制IO，IO电平翻转，产生控制脉冲
#define BUZZ_OFF        GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1    //关闭蜂鸣器

interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

float freq0=1000, freq1=2000;   // 定时器0的中断频率（Hz）
float prd0=0, prd1=0;      // 定时器0的中断周期(sec)=1/freq0/2,对于方波，一个周期要中断2次
float t1 = 1, T1 = 1;
float t2 = 3, T2 = 3;
Uint16 N1 = 0, M1 = 0;
Uint16 N2 = 0, M2 = 0;
Uint16 WarnTimes0 = 0, WarnTimes1 = 0;
extern Uint16 KEY;

#define PHA_ON      GpioDataRegs.GPASET.bit.GPIO8 = 1       //输出高电平到A相
#define PHA_OFF     GpioDataRegs.GPACLEAR.bit.GPIO8 = 1     //输出低电平到A相
#define PHB_ON      GpioDataRegs.GPASET.bit.GPIO10 = 1      //输出高电平到B相
#define PHB_OFF     GpioDataRegs.GPACLEAR.bit.GPIO10 = 1    //输出低电平到B相
#define PHC_ON      GpioDataRegs.GPASET.bit.GPIO13 = 1      //输出高电平到C相
#define PHC_OFF     GpioDataRegs.GPACLEAR.bit.GPIO13 = 1    //输出低电平到C相
#define PHD_ON      GpioDataRegs.GPASET.bit.GPIO14 = 1      //输出高电平到D相
#define PHD_OFF     GpioDataRegs.GPACLEAR.bit.GPIO14 = 1    //输出低电平到D相

#define DELAY_TIME  750000                                  //延时时间
#define STEP_TIME   15000                                   //步距
/*****************************************************************************************************/

/************************************定义步进电机运行时序**********************************************/
// 步进模式说明：16进制数0xABCD,每4位控制一相电平，其中高8位控制F28335输出低电平引脚时序，低8位控制F28335输出高电平引脚时序
// 数字是A,代表控制A相，数字是B,代表控制B相，0保持不变
//例如：0x0D0A----->D相变为低电平，A相变为高电平
//    0x0000----->保持电平状态
//    0x0A0B----->A相变为低电平，B相变为高电平

//单四拍时序A-B-C-D
// A相位H时，它的前一相（D）要恢复L,依次类推；
const Uint16 StepMode1[8]     = {0x0D0A,0x0D0A,0x0A0B,0x0A0B,0x0B0C,0x0B0C,0x0C0D,0x0C0D};      //单四拍时序A-B-C-D-A
const Uint16 StepMode1_opposite[8]= {0x0B0A,0x0B0A,0x0A0D,0x0A0D,0x0D0C,0x0D0C,0x0C0B,0x0C0B};      //单四拍时序D-C-B-A-D
//双四拍时序AB-BC-CD-DA-AB
const Uint16 StepMode2[8] = {0xCDAB,0xDABC,0xABCD,0xBCDA,0xCDAB,0xDABC,0xABCD,0xBCDA};      //双四拍时序AB-BC-CD-DA-AB
//八拍时序A-AB-B-BC-C-CD-D-DA-A
const Uint16 StepMode3[8] = {0x0D0A,0x0DAB,0x0A0B,0x0ABC,0x0B0C,0x0BCD,0x0C0D,0x0CDA};      //八拍时序A-AB-B-BC-C-CD-D-DA-A

/*********************************************函数声明************************************************/
void InitStepMotorGpio(void);
void delay(Uint32 t);
void MotionControl1(void);
void MotionControl2(void);
void MotionControl3(void);
void PhaseOff(Uint16 i);
void PhaseOn(Uint16 i);
void key_contrl(Uint16 key);
unsigned char flag=0;
void main(void){
    MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
    InitFlash();

    InitSysCtrl();

    //按键引脚初始化
    InitKeyGpio();

    //LED引脚初始化
    InitLedGpio();

//    InitLcdGpio(); // 按键引脚初始化，使用GPIO2~5。

    InitBuzzGpio();

    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    // 初始化中断向量，本例有2个中断
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    PieVectTable.XINT13 = &cpu_timer1_isr;  //XINT13;    // XINT13 / CPU-Timer1
    PieVectTable.TINT2 = &cpu_timer2_isr;  // CPU-Timer2
    EDIS;

    //初始化定时器
    InitCpuTimers();

    //功能1
    prd0=1/(freq0*2);     // 一个时钟周期，前半为H电平，后半为L电平。
    ConfigCpuTimer(&CpuTimer0, 150, prd0*1e6);//定时周期单位：us

    //功能2
    prd1=1/(freq1*2);     // 一个时钟周期，前半为H电平，后半为L电平。
    ConfigCpuTimer(&CpuTimer1, 150, prd1*1e6);//定时周期单位：us

    // 使能INT1.7(TINT0在INT1的第7个)和INT13(timer1 and xint13)
    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT13;    // timer1 and xint13
    IER |= M_INT14;    // timer2

    // 使能全局中断和实时中断
    EINT;
    ERTM;

//    // LCD上电复位，约80ms的L电平
//    RST_0;
//    delay_loopN(632000); //80ms的延时 上电延时
//    RST_1;
//    delay_loopN(632000);
//
//    // 设置LCD模块有关功能参数，并清屏
//    InitLcdFun();
//    LcdClearTxt();
//    LcdClearBmp();

    //value calu
    N1=(Uint16)(t1/prd0);
    N2=(Uint16)((t1+t2)/prd0);

    M1=(Uint16)(T1/prd1);
    M2=(Uint16)((T1+T2)/prd1);

    for(;;){
        GetKey();  // 得到按键Key的值
        if(KEY == 1){
            CpuTimer0.InterruptCount = 0;
            StartCpuTimer0();
            LED1_ON;LED2_OFF;LED3_OFF;LED4_OFF;
            LED5_OFF;LED6_OFF;LED7_OFF;LED8_OFF;
            flag==0;
        }
        if(KEY == 2){
            CpuTimer1.InterruptCount = 0;
            StartCpuTimer1();
            LED1_OFF;LED2_ON;LED3_OFF;LED4_OFF;
            LED5_OFF;LED6_OFF;LED7_OFF;LED8_OFF;
            flag==0;
        }
        if(KEY == 3 || flag==1)
        {
            flag=1;
            MotionControl2();
            StopCpuTimer1();
        }
        if(KEY == 4){
            StopCpuTimer0();
            StopCpuTimer1();
            LED1_OFF;LED2_OFF;LED3_OFF;LED4_OFF;
            LED5_OFF;LED6_OFF;LED7_OFF;LED8_OFF;
            flag==0;
        }
        DSP28x_usDelay(100000);  // 两次检测之间需0.1s间隔以上，才能准确检测，可以执行其他代码作延迟。
    };
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    if(CpuTimer0.InterruptCount<=N1){
        GEN_BUZZ_CLK; // 当定时周期为500us，产生的方波频率为1kHz
    }else if(CpuTimer0.InterruptCount<=N2){
        BUZZ_OFF;
    }else{
        CpuTimer0.InterruptCount = 0;
        WarnTimes0++;
        if(WarnTimes0 > 2){
            StopCpuTimer0();
            WarnTimes0 = 0;
        }
    }
    // Acknowledge this interrupt to receive more interrupts from group 1
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer1_isr(void)
{
    CpuTimer1.InterruptCount++;
    if(CpuTimer1.InterruptCount<=M1){
        GEN_BUZZ_CLK; // 当定时周期为500us，产生的方波频率为1kHz
    }else if(CpuTimer1.InterruptCount<=M2){
        BUZZ_OFF;
    }else{
        CpuTimer1.InterruptCount = 0;
        WarnTimes1++;
        if(WarnTimes1 > 2){
            StopCpuTimer1();
            WarnTimes1 = 0;
        }
    }
    //PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

interrupt void cpu_timer2_isr(void)
{
    CpuTimer0.InterruptCount++;
    //PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

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

void MotionControl2(void)
{
    Uint16 loop = 0;
    for(loop = 0;loop<=7;loop++)
    {
        PhaseOff((StepMode2[loop] & 0x0F00)>>8);                    //第三位：控制信号低电平判断
        PhaseOff((StepMode2[loop] & 0xF000)>>12);                   //第四位：控制信号低电平判断
        //delay(STEP_TIME);
        PhaseOn (StepMode2[loop] & 0x000F);                         //第一位：控制信号高电平判断
        PhaseOn ((StepMode2[loop] & 0x00F0)>>4);                    //第二位：控制信号高电平判断
        delay(STEP_TIME);
    }
}

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
