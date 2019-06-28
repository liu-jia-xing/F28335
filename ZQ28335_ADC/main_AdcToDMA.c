
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// ����ͨ����ţ�
// �ڼ���ģʽ�£�16��ģ������ͨ��ADCINA0~A7��ADCINB0~B7�ı�ŷֱ�Ϊ0~15

// ���ݴ������:
// AdcMirror.ADCRESULT0--->DMABuf1[160]--->ADC_Result[160]
// ADC����ֵ-->DMA���䣨��10�Σ�--->ADC_Result[160]���޷�������--->(��ɺ󣬳���ת����ʽ�浽)ADC_Result[160]����������

// ADC�����йس�������
// ADC start parameters(ADC�����йغ궨��)
// ���ø�������ʱ�Ӳ�����HSPCLK = 25.0 MHz
#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
  #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/(2*ADC_MODCLK) = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_100MHZ)
  #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/(2*ADC_MODCLK)  = 100/(2*2)   = 25.0 MHz
#endif

// ����ADC����ʱ�Ӳ���= 12.5MHz���ܵ�ADC����Ƶ�ʣ�12.5MHz/16=781.25KHz��������16·��ÿ·����Ƶ��48.8KHz��
// ADC module clock = 12.5MHz = 16 ADC clocks
#define ADC_CKPS   0x1   // ADC module clock = HSPCLK/��2*ADC_CKPS��   = 25.0MHz/(1*2) = 12.5MHz

// �ɼ�����С����������= 16 ADC clocks��
#define ADC_SHCLK  0xf   // S/H width in ADC module periods���ɼ�����С��                                            = 16 ADC clocks

#define AVG        1000  // Average sample limit��δʹ��
#define ZOFFSET    0x00  // Average Zero offset��δʹ��

// ������������С=160����Ԫ��
#define BUF_SIZE   160   // Sample buffer size

// �йر�������
// Global variable for this example
Uint16 j = 0;

Uint16 ADC_END = 0; // ADC finish flag

// ADCֱ�ӵĲ�������Ƿ���ADC����Ĵ���ADCRESULT0~15�У��޷�������0~4095��
// ͨ�������жϺ�DMA���䵽DMABuf1[160]�У��ڴ������֮ǰ��CPU���ò���ADC����
// DMABuf1�������� RAM L4 �洢��
#pragma DATA_SECTION(DMABuf1,"DMARAML4");
volatile Uint16 DMABuf1[160];    // ���DMA���������ADC�������ݡ�

// DMA����������Ҫ����Դ��ַ��Ŀ�ĵ�ַ
volatile Uint16 *DMASource;
volatile Uint16 *DMADest;


// ���Ĳ����������ADC_Result�����У�16��ͨ����ÿ��ͨ��10��Ԫ����ADCINA0~ADCINB7�����δ�ţ�
// ���ݸ�ʽΪ����������ʵ�ʵ�ѹֵ0~3Vһ�¡�
// ADC_Result��DMABuf1������ͨ������Ĺ�ʽת���ɸ�������
//  ADC_Result[i]=(float)DMABuf1[i] * 3.0 / 4096.0

// ����Ĵ��룬������ADC_Result[160]��������RAM L4�洢����
// DMARAML4������ʲô�ط����ɲ�α�p22ͼ2-3�����Ķ�28335_RAM_lnk.CMD�ļ�������ΪCMD�еĴ��롣
// MEMORY:   RAML4      : origin = 0x00C000, length = 0x001000
// SECTIONS: DMARAML4         : > RAML4,     PAGE = 1
#pragma DATA_SECTION(ADC_Result,"DMARAML4");
volatile float ADC_Result[160];  // ��Ų�����ĸ���������0~3V�����ѹ��ͬ��



void SetAdcParm(void);// ����ADCģ���йز���
interrupt void local_DINTCH1_ISR(void);// DMA CH1 �жϷ�������ISR��

void main(void)
{
   Uint16 i;

   InitSysCtrl();

   // ����HSPCLKʱ��
   EALLOW;
   SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/ADC_MODCLK
   EDIS;

   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   // ʹ��DINTCH1��DMA ͨ��1���жϣ��жϳ���Ϊlocal_DINTCH1_ISR������
   EALLOW;	// Allow access to EALLOW protected registers
   PieVectTable.DINTCH1= &local_DINTCH1_ISR;
   EDIS;   // Disable access to EALLOW protected registers
      
   // ʹ��INT7�жϣ�DMA Ch1 �ж���INT7�飬bit1λ��
   IER = M_INT7 ;	   //Enable INT7 (7.1 DMA Ch1)
   // ʹ��PIE��ENPIE���жϺ�ȫ����Ӧ��PIEACK��λ
   EnableInterrupts();
   
   // ʹ��ADCģ�飬���ϵ�
   InitAdc();  // For this example, init the ADC

   SetAdcParm();// ����ADCģ���йز���


   // ����ΪDMA CH1 �й�����
	DMAInitialize(); // DMAӲ����λ

	// Clear Table
   for (i=0; i<BUF_SIZE; i++)
   {
     DMABuf1[i] = 0xffff;
   }

	
   // Configure DMA Channel
   // DMA ͨ��1 ����
   // ����DMA����ͨ����Ŀ��Ĵ�����ַ
    DMADest   = &DMABuf1[0];              //Point DMA destination to the beginning of the array
	// ����DMA����ͨ����Դ�Ĵ�����ַ��ȡADCRESULT0
    DMASource = &AdcMirror.ADCRESULT0;    //Point DMA source to ADC result register base
	// ����DMA CH1��Դ�Ĵ�����Ŀ��Ĵ�������ʼ��ַ�͵�ǰʹ�õ�ַ
    DMACH1AddrConfig(DMADest,DMASource);

    // ����CH1��ʱ϶��Burst��
    // DMACH1BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep)
    // ÿ��burstƬ��16�֣�Դburst��ַ����Ϊ1��Ŀ��burst��ַ����Ϊ10��
    // ADCRESULT0~ADCRESULT15�Ĳ���ֵ����һ������ʱ϶��burst����ÿ��ʱ϶16�֣�Դ��ַ����Ϊ1��Ŀ���ַ����Ϊ10��
    // Ŀ���ַ����Ϊ10��ʾ��DMABuf1[]�У�DMABuf1[0~9]��ԪΪADCIN0ͨ����10���������ݣ�DMABuf1[10~19]��ԪΪADCIN1ͨ����10����������

    DMACH1BurstConfig(15,1,10);

    // DMA CH1�Ĵ�������
    // tsize��ÿ�δ����ʱ϶��burst������tsize+1
    // srctstep��ÿ�����һ��ʱ϶��Դ��ַ�������з���������ѭ��Ѱַʱ���Ըò���
    // deststep��ÿ�����һ��ʱ϶��Ŀ���ַ�������з���������ѭ��Ѱַʱ���Ըò���
    // ÿ�δ���10��ʱ϶��Դ��ַ������-15��Ŀ���ַ������(-150 + 1)
    // 9��ʾ������10��ʱ϶��-15��ʾÿ��ʱ϶����ʱ(��ADCRESULT15)���ص�ADCRESULT0����ַ��15��
    // Ŀ���ַ������(-150 + 1)��ʾĿ���ַ�ص�DMABuf1[0~9]�ж�Ӧ��λ��
    DMACH1TransferConfig(9,-15,(-150 + 1));

    // N��ʱ϶���ѭ��Ѱַ����
    // srcwsize��Դѭ��Ѱַ��С
    // srcwstep��Դѭ��Ѱַ����
    // deswsize��Ŀ��ѭ��Ѱַ��С
    // deswstep��Ŀ��ѭ��Ѱַ����
    // ��ʵ�鲻ʹ��ѭ��Ѱַ
    DMACH1WrapConfig(100,100,100,100);	  //Don't use wrap function

    //  DMA CH1 ����ģʽ����
    // void DMACH1ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, ...
    // ... Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte)
    // persel�������ж�Դѡ��λ��0~31��������ѡ��DMA_SEQ1INT�ж�
    // PERINT_ENABLE�������жϴ���DMA����ʹ��λ��������ʹ�������жϴ���DMA����
    // ONESHOT_DISABLE=0��һ���¼�����һ��ʱ϶���䣬
    // CONT_DISABLE=0:ÿ���һ��ʱ϶���䣬DMA��ֹͣ��RUNSTS bit to 0.
    // SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,������ͬ�����⼰����ж�
    // SIXTEEN_BIT�����ݴ���16λģʽ
    // CHINT_END����ÿ���������ʱ�����жϸ�CPU
    // CHINT_ENABLE:ʹ�ܸ���ͨ����CPU���жϣ�ͨ��PIE��
    DMACH1ModeConfig(DMA_SEQ1INT, PERINT_ENABLE, ONESHOT_DISABLE, CONT_DISABLE,SYNC_DISABLE,SYNC_SRC,
	                 OVRFLOW_DISABLE,SIXTEEN_BIT,CHINT_END,CHINT_ENABLE); 

	// DMA CH1 ��ʼ����
    StartDMACH1();

   // Start SEQ1
   AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0x1;    // ����SEQ1������

   // ÿ�δ�����Ͼ�������һ�δ��䣬��10�Ρ�
   for(i=0;i<10;i++)
   {
    	for(j=0;j<1000;j++)   // �ȴ�
    	{    }
        AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;    //Normally ADC will be tied to ePWM, or timed routine
   }	                                     //For this example will re-start manually                   

  // Waiting ADC finish
   while(!ADC_END);    // ADC_END=0

   // Translate DMABuf to ADC_Result
   // ��DMABuf1������ת��Ϊ���������浽ADC_Result.
   for (i=0; i<BUF_SIZE; i++)
   {
     ADC_Result[i] = (float)DMABuf1[i] * 3.0 / 4096.0;   // ���������ת��Ϊ������
   }
   
   asm ("      ESTOP0");

   for(;;);
}

// INT7.1
interrupt void local_DINTCH1_ISR(void)     // DMA Channel 1
{
  
  // To receive more interrupts from this PIE group, acknowledge this interrupt 
	// ����Ӧ���ж�λд1������ʹ��λ���㣬ͬʱ���ж����е��ж�û��ִ��ʱ��ʹ��PIE�����жϽ���CPU�ж�
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
   ADC_END = 1;
}

// ����ADCģ���йز���
void SetAdcParm(void)
{
	   // ����ADCģ�����
	   // ������ɼ�����С: ACQ_PS=0xF:���ֵΪ15������ADC_SHCLK=0xF��Ϊ16��ADCLK����
	   AdcRegs.ADCTRL1.bit.ACQ_PS       = ADC_SHCLK;
	   // ����ADCLKʱ��: f_ADCCLK=HSPCLK/(2*ADCCLKPS)/(ADCTRL1[7]+1)��������ADCTRL1[7]=0��Ϊ12.5MHz
	   AdcRegs.ADCTRL3.bit.ADCCLKPS     = ADC_CKPS;    // ADC_CKPS=1
	   // ��������������ģʽ:SEQ_CASC������ģʽ.1=��������ģʽ��SEQ��һ��16״̬������
	   AdcRegs.ADCTRL1.bit.SEQ_CASC     = 1;          // 0 Non-Cascaded Mode; 1 Cascaded Mode������ģʽ��
	   // ʹ��INT_SEQ1�ж����� INT_ENA_SEQ1=1��ʹ��INT_SEQ1�ж�����
	   // ��16��ͨ���������ʱ��������
	   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;
	   // ��λ��������RST_SEQ1=1����������λ��CONV00״̬��
	   AdcRegs.ADCTRL2.bit.RST_SEQ1     = 0x1;

	   // �����������Ĵ�����Ӧ��AD����ͨ��:
	   // CONV00--->ADC����ͨ��A0��ADCINA0��42pin����.......
	   // CONV15--->ADC����ͨ��B7��ADCINB7��53pin��;
	   AdcRegs.ADCCHSELSEQ1.bit.CONV00  = 0x0;
	   AdcRegs.ADCCHSELSEQ1.bit.CONV01  = 0x1;
	   AdcRegs.ADCCHSELSEQ1.bit.CONV02  = 0x2;
	   AdcRegs.ADCCHSELSEQ1.bit.CONV03  = 0x3;
	   AdcRegs.ADCCHSELSEQ2.bit.CONV04  = 0x4;
	   AdcRegs.ADCCHSELSEQ2.bit.CONV05  = 0x5;
	   AdcRegs.ADCCHSELSEQ2.bit.CONV06  = 0x6;
	   AdcRegs.ADCCHSELSEQ2.bit.CONV07  = 0x7;
	   AdcRegs.ADCCHSELSEQ3.bit.CONV08  = 0x8;
	   AdcRegs.ADCCHSELSEQ3.bit.CONV09  = 0x9;
	   AdcRegs.ADCCHSELSEQ3.bit.CONV10  = 0xA;
	   AdcRegs.ADCCHSELSEQ3.bit.CONV11  = 0xB;
	   AdcRegs.ADCCHSELSEQ4.bit.CONV12  = 0xC;
	   AdcRegs.ADCCHSELSEQ4.bit.CONV13  = 0xD;
	   AdcRegs.ADCCHSELSEQ4.bit.CONV14  = 0xE;
	   AdcRegs.ADCCHSELSEQ4.bit.CONV15  = 0xF;

	   // ����SEQ1�����ͨ��������16������ͨ��
	   AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 15;   // Set up ADC to perform 4 conversions for every SOC

}
