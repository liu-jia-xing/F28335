//�����ܣ�
//   ��ADģ�����A0ͨ���������ѹ��
//������ʽ��
//   ѭ�����
//Ӳ����·��
//    ��λ��VR1һ�˽�3.3V,��һ�˽ӵ�.
//    ���м��ͷͨ���������R67���뵽A0ͨ����ģ�������ź�.

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

#define TIMES      5  // ��������
#define BUF_SIZE   5  // ������������С

// Global variable for this example
Uint16 SampleTable[BUF_SIZE];  // ��Ų�������
float Vin[BUF_SIZE];  //�����ѹ(������)
float VinAvg=0;       //ƽ�������ѹ

void SetAdcParm(void); // ����ADCģ���йز���

main()
{
   Uint16 i;

   InitSysCtrl();

   // ADC���ž�Ϊר�����ţ�����Ҫ��ʼ��

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

   InitAdc();    // ʹ��ADCģ�飬���ϵ�
   SetAdcParm(); // ����ADCģ���йز���

   // ������ʼ��
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
			//  ת����ʵ�ʵ�ѹ��7.326e-4��3/4095��ֱ���ú��߻����Ϊ��������
			Vin[i]=7.326e-4*SampleTable[i];

			DSP28x_usDelay(1000);  // Ϊ����۲죬�ڱ������öϵ�
		}
	}
}


// ����ADCģ���йز���
void SetAdcParm(void)
{
	// Specific ADC setup for this example:
	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;  // S/H ��������ʱ��,16��ADCCLK
	AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1  Cascaded mode
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;   // ����A0ͨ��
	//AdcRegs.ADCCHSELSEQ1.bit.CONV01  = 0x1;// ����A1ͨ��
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
	//AdcRegs.ADCCHSELSEQ4.bit.CONV15  = 0xF;  // ����B7ͨ��

	// ����SEQ1�����ͨ��������������16������ͨ��
	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0;  //ʵ��ֵ������ֵ��1

	AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       // Setup continuous run
}
//===========================================================================
// No more.
//===========================================================================

