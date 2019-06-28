/*
 * main.c
 *
 *  Created on: 2019��6��17��
 *      Author: ToFFF
 */
#include "TheHead.h"

#define GEN_BUZZ_CLK    GpioDataRegs.GPBTOGGLE.bit.GPIO35 = 1   //����������IO��IO��ƽ��ת��������������
#define BUZZ_OFF        GpioDataRegs.GPBCLEAR.bit.GPIO35 = 1    //�رշ�����

interrupt void cpu_timer0_isr(void);
interrupt void cpu_timer1_isr(void);
interrupt void cpu_timer2_isr(void);

extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadEnd;
extern Uint16 RamfuncsRunStart;

float freq0=1000, freq1=2000;   // ��ʱ��0���ж�Ƶ�ʣ�Hz��
float prd0=0, prd1=0;      // ��ʱ��0���ж�����(sec)=1/freq0/2,���ڷ�����һ������Ҫ�ж�2��
float t1 = 1, T1 = 1;
float t2 = 3, T2 = 3;
Uint16 N1 = 0, M1 = 0;
Uint16 N2 = 0, M2 = 0;
Uint16 WarnTimes0 = 0, WarnTimes1 = 0;
extern Uint16 KEY;

#define PHA_ON      GpioDataRegs.GPASET.bit.GPIO8 = 1       //����ߵ�ƽ��A��
#define PHA_OFF     GpioDataRegs.GPACLEAR.bit.GPIO8 = 1     //����͵�ƽ��A��
#define PHB_ON      GpioDataRegs.GPASET.bit.GPIO10 = 1      //����ߵ�ƽ��B��
#define PHB_OFF     GpioDataRegs.GPACLEAR.bit.GPIO10 = 1    //����͵�ƽ��B��
#define PHC_ON      GpioDataRegs.GPASET.bit.GPIO13 = 1      //����ߵ�ƽ��C��
#define PHC_OFF     GpioDataRegs.GPACLEAR.bit.GPIO13 = 1    //����͵�ƽ��C��
#define PHD_ON      GpioDataRegs.GPASET.bit.GPIO14 = 1      //����ߵ�ƽ��D��
#define PHD_OFF     GpioDataRegs.GPACLEAR.bit.GPIO14 = 1    //����͵�ƽ��D��

#define DELAY_TIME  750000                                  //��ʱʱ��
#define STEP_TIME   15000                                   //����
/*****************************************************************************************************/

/************************************���岽���������ʱ��**********************************************/
// ����ģʽ˵����16������0xABCD,ÿ4λ����һ���ƽ�����и�8λ����F28335����͵�ƽ����ʱ�򣬵�8λ����F28335����ߵ�ƽ����ʱ��
// ������A,�������A�࣬������B,�������B�࣬0���ֲ���
//���磺0x0D0A----->D���Ϊ�͵�ƽ��A���Ϊ�ߵ�ƽ
//    0x0000----->���ֵ�ƽ״̬
//    0x0A0B----->A���Ϊ�͵�ƽ��B���Ϊ�ߵ�ƽ

//������ʱ��A-B-C-D
// A��λHʱ������ǰһ�ࣨD��Ҫ�ָ�L,�������ƣ�
const Uint16 StepMode1[8]     = {0x0D0A,0x0D0A,0x0A0B,0x0A0B,0x0B0C,0x0B0C,0x0C0D,0x0C0D};      //������ʱ��A-B-C-D-A
const Uint16 StepMode1_opposite[8]= {0x0B0A,0x0B0A,0x0A0D,0x0A0D,0x0D0C,0x0D0C,0x0C0B,0x0C0B};      //������ʱ��D-C-B-A-D
//˫����ʱ��AB-BC-CD-DA-AB
const Uint16 StepMode2[8] = {0xCDAB,0xDABC,0xABCD,0xBCDA,0xCDAB,0xDABC,0xABCD,0xBCDA};      //˫����ʱ��AB-BC-CD-DA-AB
//����ʱ��A-AB-B-BC-C-CD-D-DA-A
const Uint16 StepMode3[8] = {0x0D0A,0x0DAB,0x0A0B,0x0ABC,0x0B0C,0x0BCD,0x0C0D,0x0CDA};      //����ʱ��A-AB-B-BC-C-CD-D-DA-A

/*********************************************��������************************************************/
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

    //�������ų�ʼ��
    InitKeyGpio();

    //LED���ų�ʼ��
    InitLedGpio();

//    InitLcdGpio(); // �������ų�ʼ����ʹ��GPIO2~5��

    InitBuzzGpio();

    DINT;
    InitPieCtrl();
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();

    // ��ʼ���ж�������������2���ж�
    EALLOW;
    PieVectTable.TINT0 = &cpu_timer0_isr;
    PieVectTable.XINT13 = &cpu_timer1_isr;  //XINT13;    // XINT13 / CPU-Timer1
    PieVectTable.TINT2 = &cpu_timer2_isr;  // CPU-Timer2
    EDIS;

    //��ʼ����ʱ��
    InitCpuTimers();

    //����1
    prd0=1/(freq0*2);     // һ��ʱ�����ڣ�ǰ��ΪH��ƽ�����ΪL��ƽ��
    ConfigCpuTimer(&CpuTimer0, 150, prd0*1e6);//��ʱ���ڵ�λ��us

    //����2
    prd1=1/(freq1*2);     // һ��ʱ�����ڣ�ǰ��ΪH��ƽ�����ΪL��ƽ��
    ConfigCpuTimer(&CpuTimer1, 150, prd1*1e6);//��ʱ���ڵ�λ��us

    // ʹ��INT1.7(TINT0��INT1�ĵ�7��)��INT13(timer1 and xint13)
    IER |= M_INT1;
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT13;    // timer1 and xint13
    IER |= M_INT14;    // timer2

    // ʹ��ȫ���жϺ�ʵʱ�ж�
    EINT;
    ERTM;

//    // LCD�ϵ縴λ��Լ80ms��L��ƽ
//    RST_0;
//    delay_loopN(632000); //80ms����ʱ �ϵ���ʱ
//    RST_1;
//    delay_loopN(632000);
//
//    // ����LCDģ���йع��ܲ�����������
//    InitLcdFun();
//    LcdClearTxt();
//    LcdClearBmp();

    //value calu
    N1=(Uint16)(t1/prd0);
    N2=(Uint16)((t1+t2)/prd0);

    M1=(Uint16)(T1/prd1);
    M2=(Uint16)((T1+T2)/prd1);

    for(;;){
        GetKey();  // �õ�����Key��ֵ
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
        DSP28x_usDelay(100000);  // ���μ��֮����0.1s������ϣ�����׼ȷ��⣬����ִ�������������ӳ١�
    };
}

interrupt void cpu_timer0_isr(void)
{
    CpuTimer0.InterruptCount++;
    if(CpuTimer0.InterruptCount<=N1){
        GEN_BUZZ_CLK; // ����ʱ����Ϊ500us�������ķ���Ƶ��Ϊ1kHz
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
        GEN_BUZZ_CLK; // ����ʱ����Ϊ500us�������ķ���Ƶ��Ϊ1kHz
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
        PhaseOff((StepMode2[loop] & 0x0F00)>>8);                    //����λ�������źŵ͵�ƽ�ж�
        PhaseOff((StepMode2[loop] & 0xF000)>>12);                   //����λ�������źŵ͵�ƽ�ж�
        //delay(STEP_TIME);
        PhaseOn (StepMode2[loop] & 0x000F);                         //��һλ�������źŸߵ�ƽ�ж�
        PhaseOn ((StepMode2[loop] & 0x00F0)>>4);                    //�ڶ�λ�������źŸߵ�ƽ�ж�
        delay(STEP_TIME);
    }
}

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
                PHD_ON; break;
        default:        break;
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
