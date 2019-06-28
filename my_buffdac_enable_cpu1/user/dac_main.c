#include "F28x_Project.h"
#include <math.h>
uint16_t nums[32]={2048, 2447, 2831, 3185, 3496, 3750, 3940, 4056, 4095, 4056, 3940, 3750, 3496, 3185, 2831, 2447, 2048, 1648, 1264, 910, 599, 345, 155, 39, 0, 39, 155, 345, 599, 910, 1264, 1648
};
Uint16 bb[32]={1024, 1223, 1415, 1592, 1748, 1875, 1970, 2028, 2048, 2028, 1970, 1875, 1748, 1592, 1415, 1223, 1024, 825, 633, 456, 300, 173, 78, 20, 0, 20, 78, 173, 300, 456, 633, 825};
//
// Globals
//
volatile struct DAC_REGS* DAC_PTR[4] = {0x0,&DacaRegs,&DacbRegs,&DaccRegs};
Uint16 dacval = 2048;
//
// Defines
//
#define REFERENCE_VDAC      0
#define REFERENCE_VREF      1
#define DACA         1
#define DACB         2
#define DACC         3
//#define REFERENCE            REFERENCE_VDAC
#define REFERENCE            1
#define DAC_NUM              DACA



#define PI                            3.1415926
#define SAMPLENUMBER     128     // 采样点数

// InitForFFT(): 生成FFT运算所需的SIN和COS表
void InitForFFT();

// 生成输入数据：公式：INPUT[i]=sin(PI*2*i/SAMPLENUMBER*3)*1024; i=0:SAMPLENUMBER-1
void MakeWave();  // 生成一个sin波形到输入 INPUT变量中，代替输入采样波形的实部。

// fft算法，dataR:实部,dataI: 虚部,结果仍放在dataR和dataI中
void FFT(float dataR[SAMPLENUMBER],float dataI[SAMPLENUMBER]);

// INPUT: 输入数据的实部
int INPUT[SAMPLENUMBER];

// DATA: FFT运算后的幅度谱，从变量w中复制过来，其中
// w:FFT运算后的幅度谱，但只算出了前一半，后一半为0，可根据对称性复制。
int DATA[SAMPLENUMBER];

// fWaveR: fft运算的实部,fWaveI: fft运算的虚部,
float fWaveR[SAMPLENUMBER];
float fWaveI[SAMPLENUMBER];

// w:FFT运算后的幅度谱，但只算出了前一半，后一半为0，可根据对称性复制。
float w[SAMPLENUMBER];

// 正弦、余弦表在InitForFFT()函数中计算生成
float sin_tab[SAMPLENUMBER],cos_tab[SAMPLENUMBER];  //正弦、余弦表

// fft算法，dataR:实部,dataI: 虚部
void FFT(float dataR[SAMPLENUMBER],float dataI[SAMPLENUMBER])
{
    int x0,x1,x2,x3,x4,x5,x6,xx;
    int i,j,k,b,p,L;
    float TR,TI,temp;

    // 倒位序运算（ following code invert sequence ）
    for ( i=0;i<SAMPLENUMBER;i++ )
    {
        x0=x1=x2=x3=x4=x5=x6=0;
        x0=i&0x01; x1=(i/2)&0x01; x2=(i/4)&0x01; x3=(i/8)&0x01;x4=(i/16)&0x01; x5=(i/32)&0x01; x6=(i/64)&0x01;
        xx=x0*64+x1*32+x2*16+x3*8+x4*4+x5*2+x6;
        dataI[xx]=dataR[i];
    }
    for ( i=0;i<SAMPLENUMBER;i++ )
    {
        dataR[i]=dataI[i]; dataI[i]=0;
    }

    // FFT 变换（ following code FFT ）
    for ( L=1;L<=7;L++ )
    { /* for(1) */
        b=1; i=L-1;
        while ( i>0 )
        {
            b=b*2; i--;
        } /* b= 2^(L-1) */
        for ( j=0;j<=b-1;j++ ) /* for (2) */
        {
            p=1; i=7-L;
            while ( i>0 ) /* p=pow(2,7-L)*j; */
            {
                p=p*2; i--;
            }
            p=p*j;
            for ( k=j;k<128;k=k+2*b ) /* for (3) */
            {
                TR=dataR[k]; TI=dataI[k]; temp=dataR[k+b];
                dataR[k]=dataR[k]+dataR[k+b]*cos_tab[p]+dataI[k+b]*sin_tab[p];
                dataI[k]=dataI[k]-dataR[k+b]*sin_tab[p]+dataI[k+b]*cos_tab[p];
                dataR[k+b]=TR-dataR[k+b]*cos_tab[p]-dataI[k+b]*sin_tab[p];
                dataI[k+b]=TI+temp*sin_tab[p]-dataI[k+b]*cos_tab[p];
            } /* END for (3) */
        } /* END for (2) */
    } /* END for (1) */

    // 计算幅度谱
    for ( i=0;i<SAMPLENUMBER/2;i++ )
    {
        w[i]=sqrt(dataR[i]*dataR[i]+dataI[i]*dataI[i]);
    }
} /* END FFT */




void InitForFFT() // 生成FFT运算所需的SIN和COS表
{
    int i;

    for ( i=0;i<SAMPLENUMBER;i++ )
    {
        sin_tab[i]=sin(PI*2*i/SAMPLENUMBER);
        cos_tab[i]=cos(PI*2*i/SAMPLENUMBER);
    }
}

// 生成输入数据：公式：INPUT[i]=sin(PI*2*i/SAMPLENUMBER*3)*1024; i=0:SAMPLENUMBER-1
void MakeWave()// 生成一个sin波形到输入数据（int） INPUT变量中，代替输入采样波形的实部。
{
    int i;

    for ( i=0;i<SAMPLENUMBER;i++ )
    {
        INPUT[i]=sin(PI*2*i/SAMPLENUMBER*3)*1024;
    }
}


//
// Function Prototypes
//
void configureDAC(Uint16 dac_num);


void main(void)
{
//
// Initialize System Control:
// PLL, WatchDog, enable Peripheral Clocks
// This example function is found in the F2837xD_SysCtrl.c file.
//
    InitSysCtrl();

//
// Disable CPU interrupts
//
    DINT;

//
// Initialize the PIE control registers to their default state.
// The default state is all PIE interrupts disabled and flags are cleared.
// This function is found in the F2837xD_PieCtrl.c file.
//
    InitPieCtrl();

//
// Clear all interrupts and initialize PIE vector table:
//
    IER = 0x0000;
    IFR = 0x0000;
    InitPieVectTable();
//
// Configure DAC
//
    uint16_t i=0;
    configureDAC(DAC_NUM);
    Uint16 a=0;
    MakeWave();
    InitForFFT();   // 生成FFT运算所需的SIN和COS表
       MakeWave();    // 生成一个sin波形到输入 INPUT变量中，代替输入采样波形的实部。

       for ( i=0;i<SAMPLENUMBER;i++ )
       {
           fWaveR[i]=INPUT[i];     //得到FFT运算输入的实部
           fWaveI[i]=0.0f;         //得到FFT运算输入的虚部
           w[i]=0.0f;              // FFT输出缓冲区（幅度谱）清零
       }

       FFT(fWaveR,fWaveI);       // FFT运算

       // 复制幅度谱到DATA缓冲区
       while(1)
       for ( i=0;i<SAMPLENUMBER;i++ )
       {
           DATA[i]=w[i];
           DAC_PTR[DAC_NUM]->DACVALS.all =  DATA[i];
       }

      /* for(;;)
       {
       } //break point*/
   /* while(1)
    {   a=0;
       // DAC_PTR[DAC_NUM]->DACVALS.all = dacval;
       // DELAY_US(2);

        DAC_PTR[DAC_NUM]->DACVALS.all = bb[i];
        i++;
       // DELAY_US(2);
        if(i>31)
            i=0;
       for(a=0;a<50;a++);
    }*/
}

//
// configureDAC - Configure specified DAC output
//
void configureDAC(Uint16 dac_num)
{
    EALLOW;
    DAC_PTR[dac_num]->DACCTL.bit.DACREFSEL = 1;
  //  DAC_PTR[dac_num]->DACCTL.bit.DACREFSEL = 0;
    DAC_PTR[dac_num]->DACOUTEN.bit.DACOUTEN = 1;
    DAC_PTR[dac_num]->DACVALS.all = 0;
    DELAY_US(10); // Delay for buffered DAC to power up
    EDIS;
}


