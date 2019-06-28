/********************************************************************
* 文件名： 	步进电机驱动程序
* 描述:  	执行该程序，步进电机旋转。例程编写了3种控制方式。
			调用不同的控制时序程序，观察控制效果
电路连接：  	GPIO8--(ULN2003)-->步进电机1脚，其中ULN2003为反相控制，in=H,out=L
		GPIO10-(ULN2003)-->步进电机2脚
		GPIO13-(ULN2003)-->步进电机3脚
		GPIO14-(ULN2003)-->步进电机4脚
		5V------>(ULN2003)+步进电机5脚


**********************************************************************/
/********************************************************************
程序说明：	
********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "key.h"
/**************************************宏定义************************************************/
#define PHA_ON		GpioDataRegs.GPASET.bit.GPIO8 = 1		//输出高电平到A相
#define PHA_OFF		GpioDataRegs.GPACLEAR.bit.GPIO8 = 1		//输出低电平到A相
#define PHB_ON		GpioDataRegs.GPASET.bit.GPIO10 = 1		//输出高电平到B相
#define PHB_OFF		GpioDataRegs.GPACLEAR.bit.GPIO10 = 1	//输出低电平到B相
#define PHC_ON		GpioDataRegs.GPASET.bit.GPIO13 = 1		//输出高电平到C相
#define PHC_OFF		GpioDataRegs.GPACLEAR.bit.GPIO13 = 1	//输出低电平到C相
#define PHD_ON		GpioDataRegs.GPASET.bit.GPIO14 = 1		//输出高电平到D相
#define PHD_OFF		GpioDataRegs.GPACLEAR.bit.GPIO14 = 1	//输出低电平到D相

#define DELAY_TIME	750000									//延时时间
#define STEP_TIME	15000									//步距
/*****************************************************************************************************/

/************************************定义步进电机运行时序**********************************************/
// 步进模式说明：16进制数0xABCD,每4位控制一相电平，其中高8位控制F28335输出低电平引脚时序，低8位控制F28335输出高电平引脚时序
// 数字是A,代表控制A相，数字是B,代表控制B相，0保持不变
//例如：0x0D0A----->D相变为低电平，A相变为高电平
//    0x0000----->保持电平状态
//	  0x0A0B----->A相变为低电平，B相变为高电平

//单四拍时序A-B-C-D
// A相位H时，它的前一相（D）要恢复L,依次类推；
const Uint16 StepMode1[8]     = {0x0D0A,0x0D0A,0x0A0B,0x0A0B,0x0B0C,0x0B0C,0x0C0D,0x0C0D};		//单四拍时序A-B-C-D-A
const Uint16 StepMode1_opposite[8]= {0x0B0A,0x0B0A,0x0A0D,0x0A0D,0x0D0C,0x0D0C,0x0C0B,0x0C0B};      //单四拍时序D-C-B-A-D
//双四拍时序AB-BC-CD-DA-AB
const Uint16 StepMode2[8] = {0xCDAB,0xDABC,0xABCD,0xBCDA,0xCDAB,0xDABC,0xABCD,0xBCDA};		//双四拍时序AB-BC-CD-DA-AB
//八拍时序A-AB-B-BC-C-CD-D-DA-A
const Uint16 StepMode3[8] = {0x0D0A,0x0DAB,0x0A0B,0x0ABC,0x0B0C,0x0BCD,0x0C0D,0x0CDA};		//八拍时序A-AB-B-BC-C-CD-D-DA-A

/*********************************************函数声明************************************************/
void InitStepMotorGpio(void);
void delay(Uint32 t);
void MotionControl1(void);
void MotionControl2(void);
void MotionControl3(void);
void PhaseOff(Uint16 i);
void PhaseOn(Uint16 i);
void key_contrl(Uint16 key);
/*****************************************************************************************************/
__interrupt void cpu_timer0_isr(void);


Uint16 timer_flag=0;
Uint16 timer_flag1=0;
void main(void)
{
   InitSysCtrl();

   InitStepMotorGpio();
   InitKeyGpio(); // 按键引脚初始化
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   EALLOW;  // This is needed to write to EALLOW protected registers
   PieVectTable.TINT0 = &cpu_timer0_isr;
   EDIS;    // This is needed to disable write to EALLOW protected registers

   InitCpuTimers();   // For this example, only initialize the Cpu Timers
   ConfigCpuTimer(&CpuTimer0, 150, 1000);
   CpuTimer0Regs.TCR.all = 0x4000; //write-only instruction to set TSS bit = 0

   EALLOW;
   GpioCtrlRegs.GPADIR.bit.GPIO0=1;
   EDIS;
   //
    // Enable CPU int1 which is connected to CPU-Timer 0, CPU int13
    // which is connected to CPU-Timer 1, and CPU int 14, which is connected
    // to CPU-Timer 2:
    //
    IER |= M_INT1;
    //
    // Enable TINT0 in the PIE: Group 1 interrupt 7
    //
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    //
    // Enable global Interrupts and higher priority real-time debug events:
    //
    EINT;   // Enable Global interrupt INTM
    ERTM;   // Enable Global realtime interrupt DBGM

    static Uint16 key_value=0;
    static Uint16 key_value1=0;
    while(1)	//死循环
    {
        	MotionControl2();
/*        key_value=GetKey();  // 得到按键Key的值
        if(key_value1 !=key_value)
        {
            key_contrl(key_value1);
            if(key_value!=0)
                key_value1=key_value;
        }*/

	}
}


//
// cpu_timer0_isr -
//
__interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    timer_flag++;
    //
    // Acknowledge this interrupt to receive more interrupts from group 1
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;

}

/*电机引脚控制IO初始化*******************************************/
//GPIO8 -->电机1脚（A相）
//GPIO10-->电机2脚（B相）
//GPIO13-->电机3脚（C相）
//GPIO14-->电机4脚（D相）


void InitStepMotorGpio(void)
{
	EALLOW;   
	//Phase A
	GpioCtrlRegs.GPAPUD.bit.GPIO8 = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO8 = 1;   									// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO8 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO8 = 1;   									// GPIO11 = output    
	//Phase B
	GpioCtrlRegs.GPAPUD.bit.GPIO13 = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO13 = 1;   									// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO13 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO13 = 1;   									// GPIO11 = output    
	//Phase C
	GpioCtrlRegs.GPAPUD.bit.GPIO10 = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO10 = 1;   									// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO10 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO10 = 1;   									// GPIO11 = output   
	//Phase D    
    GpioCtrlRegs.GPAPUD.bit.GPIO14 = 0;   									// Enable pullup on GPIO11
    GpioDataRegs.GPASET.bit.GPIO14 = 1;   									// Load output latch
    GpioCtrlRegs.GPAMUX1.bit.GPIO14 = 0;  									// GPIO11 = GPIO
    GpioCtrlRegs.GPADIR.bit.GPIO14 = 1;   									// GPIO11 = output    

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
				PHD_ON;	break;
		default:    	break;
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
// const Uint16 StepMode1[8] = {0x0D0A,0x0D0A,0x0A0B,0x0A0B,0x0B0C,0x0B0C,0x0C0D,0x0C0D};		//单四拍时序A-B-C-D-A
void MotionControl1(void)
{
	Uint16 loop = 0;

	for(loop = 0;loop<=7;loop++)
	{
		PhaseOn (StepMode1[loop]  & 0x000F);		//第一位：控制信号高电平判断
		PhaseOn ((StepMode1[loop] & 0x00F0)>>4);	//第二位：控制信号高电平判断
		PhaseOff((StepMode1[loop] & 0x0F00)>>8);	//第三位：控制信号低电平判断
		PhaseOff((StepMode1[loop] & 0xF000)>>12);	//第四位：控制信号低电平判断
		delay(STEP_TIME);
		//timer_flag=0;

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
       delay(STEP_TIME);
     //   while(timer_flag==0);
      //  timer_flag=0;
    }
}
/*****************************************************************************************************/

/******************************************双四拍控制时序*********************************************/
//双四拍时序AB-BC-CD-DA-AB
// const Uint16 StepMode2[8] = {0xCDAB,0xDABC,0xABCD,0xBCDA,0xCDAB,0xDABC,0xABCD,0xBCDA};		//双四拍时序AB-BC-CD-DA-AB
void MotionControl2(void)
{
	Uint16 loop = 0;
	for(loop = 0;loop<=7;loop++)
	{
		PhaseOff((StepMode2[loop] & 0x0F00)>>8);					//第三位：控制信号低电平判断
		PhaseOff((StepMode2[loop] & 0xF000)>>12);				    //第四位：控制信号低电平判断
		//delay(STEP_TIME);
		PhaseOn (StepMode2[loop] & 0x000F);							//第一位：控制信号高电平判断
		PhaseOn ((StepMode2[loop] & 0x00F0)>>4);					//第二位：控制信号高电平判断	
		delay(STEP_TIME);
	}	
}

/*****************************************************************************************************/

/******************************************八拍控制时序*********************************************/
//八拍时序A-AB-B-BC-C-CD-D-DA-A
// const Uint16 StepMode3[8] = {0x0D0A,0x0DAB,0x0A0B,0x0ABC,0x0B0C,0x0BCD,0x0C0D,0x0CDA};		//八拍时序A-AB-B-BC-C-CD-D-DA-A
void MotionControl3(void)
{
	Uint16 loop = 0;
	for(loop = 0;loop<=7;loop++)
	{
	    PhaseOff((StepMode2[loop] & 0xF000)>>12);                   //第四位：控制信号低电平判断
	        PhaseOff((StepMode2[loop] & 0x0F00)>>8);                    //第三位：控制信号低电平判断
	        PhaseOn ((StepMode2[loop] & 0x00F0)>>4);                    //第二位：控制信号高电平判断
	        PhaseOn (StepMode2[loop] & 0x000F);

		delay(STEP_TIME);
	}	
}

/*****************************************************************************************************/
void key_contrl(Uint16 key)
{
    switch(key)
    {
        case 0x04: MotionControl1_opposite(); break;
        case 0x06:  MotionControl1();break;
        case 9:break;
        default:break;
    }
}



