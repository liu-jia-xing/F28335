/*
 * Cal_config.c
 *
 *  Created on: 2019年6月30日
 *      Author: L9379
 */

#include "Cal_config.h"
extern Uint16 LoopCount;
extern Uint16 ConversionCount;
extern Uint16 Voltage1[10];
extern Uint16 Voltage2[10];
extern  float amplitude[400];
extern  float phaseA[400];
extern  Uint16 sampling_time=0;

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
