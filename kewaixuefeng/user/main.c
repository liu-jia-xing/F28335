#include "DSP28x_Project.h"
#include <math.h>
__interrupt void adc_isr(void);


Uint16 LoopCount;
Uint16 ConversionCount;
Uint16 Voltage1[10];
Uint16 Voltage2[10];
static float amplitude[400];
static float phaseA[400];
static Uint16 sampling_time=0;

void adc_config();
void start_sample(float *,float *,float *);
void cal_phase();
void clear_array();
void phase_cal();
void cal_frequency(float *Voltage1,float *Voltage2,float *amplitude);

//main
void main(void)
{

    InitSysCtrl();

    EALLOW;
    SysCtrlRegs.HISPCP.all = 0x3;
    EDIS;

    DINT;
    InitPieCtrl();
    //
    // Disable CPU interrupts and clear all CPU interrupt flags:
    //
    IER = 0x0000;
    IFR = 0x0000;

    InitPieVectTable();
    //load isr_address
    EALLOW;  // This is needed to write to EALLOW protected register
    PieVectTable.ADCINT = &adc_isr;
    EDIS;    // This is needed to disable write to EALLOW protected registers

    InitAdc();  // For this example, init the ADC

    //PIE enable
    PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
    IER |= M_INT1;      // Enable CPU Interrupt 1
    EINT;               // Enable Global interrupt INTM
    ERTM;               // Enable Global realtime interrupt DBGM

    //同步采样模式配置
    adc_config();
    clear_array();
    //frequency times;
    ConversionCount=0;
    DELAY_US(1000000);
    for(;;)
    {
    //    AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
        start_sample(Voltage1,Voltage1,amplitude);
     //    AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
    }
}

//
// adc_isr -
//
__interrupt void adc_isr(void)
{
    Voltage1[sampling_time] = AdcRegs.ADCRESULT0 >>4;
    Voltage2[sampling_time] = AdcRegs.ADCRESULT1 >>4;


    AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
    AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
    AdcRegs.ADCST.bit.INT_SEQ2_CLR = 1;       // Clear INT SEQ1 bit
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE
    return;
}

void adc_config()
{
    AdcRegs.ADCTRL3.bit.SMODE_SEL=0x1;      //同步采样模式
       AdcRegs.ADCTRL1.bit.SEQ_CASC=0x0;    //双排序器模式
       AdcRegs.ADCMAXCONV.all=0x0033;       //最大通道数
       AdcRegs.ADCCHSELSEQ1.bit.CONV00=0x0; //ADC输入通道    ADCINA0和ADCINB0;
       AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
       AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)
       AdcRegs.ADCTRL2.bit.INT_ENA_SEQ2 = 1;  // Enable SEQ2 interrupt (every EOS)
}

//函数功能，调用一次，采样五个点；
void start_sample(float *Voltage1,float *Voltage2,float *amplitude)
{
    if(sampling_time<5)
    {   while(AdcRegs.ADCST.bit.INT_SEQ1== 0)
        AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;
        sampling_time++;
    }
    else
    {
        sampling_time=0;
        cal_frequency(Voltage1,Voltage2,amplitude);
        phase_cal();
    }
}

//计算幅度
void cal_frequency(float *Voltage1,float *Voltage2,float *amplitude)
{
    Uint16 a=0;
    static float cal=0;
    static float add=0;
    static float64 com=0;

    for(a=0;a<5;a++)
    {
        cal=Voltage1[a]*3/4095.0;
        cal*=cal;
        add=cal;
        cal=Voltage2[a]*3/4095.0;
        cal*=cal;
        add+=cal;
        com+=(sqrt(add)/0.7);
    }
    amplitude[ConversionCount]=(20*log(com/5));
    com=0;
    ConversionCount++;
}

//计算相位
void phase_cal()
{
    Uint16 a=0;
    for(a=0;a<5;a++)
    {
        a+=Voltage1[a]*1.0/Voltage2[a];
    }
    phaseA[ConversionCount-1]=(atanl(a/5));
}

//清空预存值数组
void clear_array()
{
    Uint16 a;
    for(a=0;a<400;a++)
    {
        amplitude[a]=0;
        phaseA[a]=0;
    }
}
