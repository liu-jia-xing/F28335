/********************************************************************
* �ļ����� 	���������������
* ����:  	ִ�иó��򣬲��������ת�����̱�д��3�ֿ��Ʒ�ʽ��
			���ò�ͬ�Ŀ���ʱ����򣬹۲����Ч��
��·���ӣ�  	GPIO8--(ULN2003)-->�������1�ţ�����ULN2003Ϊ������ƣ�in=H,out=L
		GPIO10-(ULN2003)-->�������2��
		GPIO13-(ULN2003)-->�������3��
		GPIO14-(ULN2003)-->�������4��
		5V------>(ULN2003)+�������5��


**********************************************************************/
/********************************************************************
����˵����	
********************************************************************/

#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File
#include "key.h"
/**************************************�궨��************************************************/
#define PHA_ON		GpioDataRegs.GPASET.bit.GPIO8 = 1		//����ߵ�ƽ��A��
#define PHA_OFF		GpioDataRegs.GPACLEAR.bit.GPIO8 = 1		//����͵�ƽ��A��
#define PHB_ON		GpioDataRegs.GPASET.bit.GPIO10 = 1		//����ߵ�ƽ��B��
#define PHB_OFF		GpioDataRegs.GPACLEAR.bit.GPIO10 = 1	//����͵�ƽ��B��
#define PHC_ON		GpioDataRegs.GPASET.bit.GPIO13 = 1		//����ߵ�ƽ��C��
#define PHC_OFF		GpioDataRegs.GPACLEAR.bit.GPIO13 = 1	//����͵�ƽ��C��
#define PHD_ON		GpioDataRegs.GPASET.bit.GPIO14 = 1		//����ߵ�ƽ��D��
#define PHD_OFF		GpioDataRegs.GPACLEAR.bit.GPIO14 = 1	//����͵�ƽ��D��

#define DELAY_TIME	750000									//��ʱʱ��
#define STEP_TIME	15000									//����
/*****************************************************************************************************/

/************************************���岽���������ʱ��**********************************************/
// ����ģʽ˵����16������0xABCD,ÿ4λ����һ���ƽ�����и�8λ����F28335����͵�ƽ����ʱ�򣬵�8λ����F28335����ߵ�ƽ����ʱ��
// ������A,�������A�࣬������B,�������B�࣬0���ֲ���
//���磺0x0D0A----->D���Ϊ�͵�ƽ��A���Ϊ�ߵ�ƽ
//    0x0000----->���ֵ�ƽ״̬
//	  0x0A0B----->A���Ϊ�͵�ƽ��B���Ϊ�ߵ�ƽ

//������ʱ��A-B-C-D
// A��λHʱ������ǰһ�ࣨD��Ҫ�ָ�L,�������ƣ�
const Uint16 StepMode1[8]     = {0x0D0A,0x0D0A,0x0A0B,0x0A0B,0x0B0C,0x0B0C,0x0C0D,0x0C0D};		//������ʱ��A-B-C-D-A
const Uint16 StepMode1_opposite[8]= {0x0B0A,0x0B0A,0x0A0D,0x0A0D,0x0D0C,0x0D0C,0x0C0B,0x0C0B};      //������ʱ��D-C-B-A-D
//˫����ʱ��AB-BC-CD-DA-AB
const Uint16 StepMode2[8] = {0xCDAB,0xDABC,0xABCD,0xBCDA,0xCDAB,0xDABC,0xABCD,0xBCDA};		//˫����ʱ��AB-BC-CD-DA-AB
//����ʱ��A-AB-B-BC-C-CD-D-DA-A
const Uint16 StepMode3[8] = {0x0D0A,0x0DAB,0x0A0B,0x0ABC,0x0B0C,0x0BCD,0x0C0D,0x0CDA};		//����ʱ��A-AB-B-BC-C-CD-D-DA-A

/*********************************************��������************************************************/
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
   InitKeyGpio(); // �������ų�ʼ��
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
    while(1)	//��ѭ��
    {
        	MotionControl2();
/*        key_value=GetKey();  // �õ�����Key��ֵ
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

/*������ſ���IO��ʼ��*******************************************/
//GPIO8 -->���1�ţ�A�ࣩ
//GPIO10-->���2�ţ�B�ࣩ
//GPIO13-->���3�ţ�C�ࣩ
//GPIO14-->���4�ţ�D�ࣩ


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

/*��ʱ����************************************************/
void delay(Uint32 t)
{
	Uint32 i = 0;
	for (i = 0; i < t; i++);
}
/********************************************************/

// ָ����i�ർͨ��IO���H��ƽ��ULN2003���������鹩�硣
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

// ָ����i��Ͽ���IO���L��ƽ��ULN2003��������鹩���ֹ��
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

/******************************************�����Ŀ���ʱ��*********************************************/
//������ʱ��A-B-C-D-A
// const Uint16 StepMode1[8] = {0x0D0A,0x0D0A,0x0A0B,0x0A0B,0x0B0C,0x0B0C,0x0C0D,0x0C0D};		//������ʱ��A-B-C-D-A
void MotionControl1(void)
{
	Uint16 loop = 0;

	for(loop = 0;loop<=7;loop++)
	{
		PhaseOn (StepMode1[loop]  & 0x000F);		//��һλ�������źŸߵ�ƽ�ж�
		PhaseOn ((StepMode1[loop] & 0x00F0)>>4);	//�ڶ�λ�������źŸߵ�ƽ�ж�
		PhaseOff((StepMode1[loop] & 0x0F00)>>8);	//����λ�������źŵ͵�ƽ�ж�
		PhaseOff((StepMode1[loop] & 0xF000)>>12);	//����λ�������źŵ͵�ƽ�ж�
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
        PhaseOn (StepMode1_opposite[loop]  & 0x000F);        //��һλ�������źŸߵ�ƽ�ж�
        PhaseOn ((StepMode1_opposite[loop] & 0x00F0)>>4);    //�ڶ�λ�������źŸߵ�ƽ�ж�
        PhaseOff((StepMode1_opposite[loop] & 0x0F00)>>8);    //����λ�������źŵ͵�ƽ�ж�
        PhaseOff((StepMode1_opposite[loop] & 0xF000)>>12);   //����λ�������źŵ͵�ƽ�ж�
       delay(STEP_TIME);
     //   while(timer_flag==0);
      //  timer_flag=0;
    }
}
/*****************************************************************************************************/

/******************************************˫���Ŀ���ʱ��*********************************************/
//˫����ʱ��AB-BC-CD-DA-AB
// const Uint16 StepMode2[8] = {0xCDAB,0xDABC,0xABCD,0xBCDA,0xCDAB,0xDABC,0xABCD,0xBCDA};		//˫����ʱ��AB-BC-CD-DA-AB
void MotionControl2(void)
{
	Uint16 loop = 0;
	for(loop = 0;loop<=7;loop++)
	{
		PhaseOff((StepMode2[loop] & 0x0F00)>>8);					//����λ�������źŵ͵�ƽ�ж�
		PhaseOff((StepMode2[loop] & 0xF000)>>12);				    //����λ�������źŵ͵�ƽ�ж�
		//delay(STEP_TIME);
		PhaseOn (StepMode2[loop] & 0x000F);							//��һλ�������źŸߵ�ƽ�ж�
		PhaseOn ((StepMode2[loop] & 0x00F0)>>4);					//�ڶ�λ�������źŸߵ�ƽ�ж�	
		delay(STEP_TIME);
	}	
}

/*****************************************************************************************************/

/******************************************���Ŀ���ʱ��*********************************************/
//����ʱ��A-AB-B-BC-C-CD-D-DA-A
// const Uint16 StepMode3[8] = {0x0D0A,0x0DAB,0x0A0B,0x0ABC,0x0B0C,0x0BCD,0x0C0D,0x0CDA};		//����ʱ��A-AB-B-BC-C-CD-D-DA-A
void MotionControl3(void)
{
	Uint16 loop = 0;
	for(loop = 0;loop<=7;loop++)
	{
	    PhaseOff((StepMode2[loop] & 0xF000)>>12);                   //����λ�������źŵ͵�ƽ�ж�
	        PhaseOff((StepMode2[loop] & 0x0F00)>>8);                    //����λ�������źŵ͵�ƽ�ж�
	        PhaseOn ((StepMode2[loop] & 0x00F0)>>4);                    //�ڶ�λ�������źŸߵ�ƽ�ж�
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



