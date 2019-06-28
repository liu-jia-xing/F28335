//程序功能：
//   用AD模块测量A0通道的输入电压。
//工作方式：
//   循环检测
//硬件电路：
//    电位器VR1一端接3.3V,另一端接地.
//    的中间抽头通过隔离电阻R67接入到A0通道，模拟输入信号.

#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// HSPCLK = 25.0 MHz
#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
  #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/(2*ADC_MODCLK2) = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_100MHZ)
  #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/(2*ADC_MODCLK2) = 100/(2*2)   = 25.0 MHz
#endif

// ADC CLK = 12.5MHz
#define ADC_CKPS   0x1   // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
#define ADC_SHCLK  0xf   // S/H width in ADC module periods                        = 16 ADC clocks

#define TIMES      5  // 采样次数
#define BUF_SIZE   5  // 采样缓冲区大小

// Global variable for this example
Uint16 SampleTable[BUF_SIZE];  // 存放采用数据
float Vin[BUF_SIZE];  //输入电压(浮点数)
float VinAvg=0;       //平均输入电压

void SetAdcParm(void); // 设置ADC模块有关参数

main()
{
   Uint16 i;

   InitSysCtrl();

   // ADC引脚均为专用引脚，不需要初始化

   EALLOW;
   SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/ADC_MODCLK
   EDIS;
   EALLOW;
   GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
   EDIS;
   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   InitAdc();    // 使能ADC模块，并上电
   SetAdcParm(); // 设置ADC模块有关参数

   // 变量初始化
   for (i=0; i<BUF_SIZE; i++)
   {
     SampleTable[i] = 0;
   }

   // Start SEQ1
   AdcRegs.ADCTRL2.bit.SOC_SEQ1=1;

   // Take ADC data and log the in SampleTable array
	for(;;)
	{
		for (i=0; i<TIMES; i++)
		{
			while (AdcRegs.ADCST.bit.INT_SEQ1== 0)
			{
				// Wait for interrupt,No Command
			}

			AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;
			SampleTable[i] =((AdcRegs.ADCRESULT0>>4) );
			//  转换成实际电压，7.326e-4是3/4095，直接用后者会编译为整数运算
			Vin[i]=7.326e-4*SampleTable[i];

			DSP28x_usDelay(1000);  // 为方便观察，在本行设置断点
		}
	}
}


// 设置ADC模块有关参数
void SetAdcParm(void)
{
	// Specific ADC setup for this example:
	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;  // S/H 采样保持时间,16个ADCCLK
	AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1  Cascaded mode
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;   // 采样A0通道
	//AdcRegs.ADCCHSELSEQ1.bit.CONV01  = 0x1;// 采样A1通道
	//AdcRegs.ADCCHSELSEQ1.bit.CONV02  = 0x2;
	//AdcRegs.ADCCHSELSEQ1.bit.CONV03  = 0x3;
	//AdcRegs.ADCCHSELSEQ2.bit.CONV04  = 0x4;
	//AdcRegs.ADCCHSELSEQ2.bit.CONV05  = 0x5;
	//AdcRegs.ADCCHSELSEQ2.bit.CONV06  = 0x6;
	//AdcRegs.ADCCHSELSEQ2.bit.CONV07  = 0x7;
	//AdcRegs.ADCCHSELSEQ3.bit.CONV08  = 0x8;
	//AdcRegs.ADCCHSELSEQ3.bit.CONV09  = 0x9;
	//AdcRegs.ADCCHSELSEQ3.bit.CONV10  = 0xA;
	//AdcRegs.ADCCHSELSEQ3.bit.CONV11  = 0xB;
	//AdcRegs.ADCCHSELSEQ4.bit.CONV12  = 0xC;
	//AdcRegs.ADCCHSELSEQ4.bit.CONV13  = 0xD;
	//AdcRegs.ADCCHSELSEQ4.bit.CONV14  = 0xE;
	//AdcRegs.ADCCHSELSEQ4.bit.CONV15  = 0xF;  // 采样B7通道

	// 设置SEQ1的最大通道数，最多可设置16个输入通道
	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0;  //实际值比设置值大1

	AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       // Setup continuous run
}
//===========================================================================
// No more.
//===========================================================================

