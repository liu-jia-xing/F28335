
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// 输入通道编号：
// 在级联模式下，16个模拟输入通道ADCINA0~A7、ADCINB0~B7的编号分别为0~15

// 数据传输过程:
// AdcMirror.ADCRESULT0--->DMABuf1[160]--->ADC_Result[160]
// ADC采样值-->DMA传输（分10次）--->ADC_Result[160]（无符号数）--->(完成后，程序转换格式存到)ADC_Result[160]（浮点数）

// ADC设置有关常数定义
// ADC start parameters(ADC参数有关宏定义)
// 设置高速外设时钟参数：HSPCLK = 25.0 MHz
#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
  #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/(2*ADC_MODCLK) = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_100MHZ)
  #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/(2*ADC_MODCLK)  = 100/(2*2)   = 25.0 MHz
#endif

// 设置ADC采样时钟参数= 12.5MHz，总的ADC采样频率：12.5MHz/16=781.25KHz，共采样16路，每路采样频率48.8KHz。
// ADC module clock = 12.5MHz = 16 ADC clocks
#define ADC_CKPS   0x1   // ADC module clock = HSPCLK/（2*ADC_CKPS）   = 25.0MHz/(1*2) = 12.5MHz

// 采集窗大小（保持周期= 16 ADC clocks）
#define ADC_SHCLK  0xf   // S/H width in ADC module periods（采集窗大小）                                            = 16 ADC clocks

#define AVG        1000  // Average sample limit，未使用
#define ZOFFSET    0x00  // Average Zero offset，未使用

// 采样缓冲区大小=160个单元。
#define BUF_SIZE   160   // Sample buffer size

// 有关变量定义
// Global variable for this example
Uint16 j = 0;

Uint16 ADC_END = 0; // ADC finish flag

// ADC直接的采样结果是放在ADC结果寄存器ADCRESULT0~15中，无符号整数0~4095，
// 通过采样中断和DMA传输到DMABuf1[160]中，在传输完成之前，CPU不用插手ADC事务，
// DMABuf1，定义在 RAM L4 存储器
#pragma DATA_SECTION(DMABuf1,"DMARAML4");
volatile Uint16 DMABuf1[160];    // 存放DMA传输过来的ADC采样数据。

// DMA传输数据需要定义源地址和目的地址
volatile Uint16 *DMASource;
volatile Uint16 *DMADest;


// 最后的采样结果放在ADC_Result变量中，16个通道，每个通道10单元，从ADCINA0~ADCINB7，依次存放，
// 数据格式为浮点数，与实际电压值0~3V一致。
// ADC_Result是DMABuf1的数据通过下面的公式转换成浮点数的
//  ADC_Result[i]=(float)DMABuf1[i] * 3.0 / 4096.0

// 下面的代码，将变量ADC_Result[160]，定义在RAM L4存储器，
// DMARAML4具体在什么地方，可查课本p22图2-3，或阅读28335_RAM_lnk.CMD文件，下面为CMD中的代码。
// MEMORY:   RAML4      : origin = 0x00C000, length = 0x001000
// SECTIONS: DMARAML4         : > RAML4,     PAGE = 1
#pragma DATA_SECTION(ADC_Result,"DMARAML4");
volatile float ADC_Result[160];  // 存放采样后的浮点数（与0~3V输入电压相同）



void SetAdcParm(void);// 设置ADC模块有关参数
interrupt void local_DINTCH1_ISR(void);// DMA CH1 中断服务函数（ISR）

void main(void)
{
   Uint16 i;

   InitSysCtrl();

   // 设置HSPCLK时钟
   EALLOW;
   SysCtrlRegs.HISPCP.all = ADC_MODCLK;	// HSPCLK = SYSCLKOUT/ADC_MODCLK
   EDIS;

   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   // 使用DINTCH1（DMA 通道1）中断，中断程序为local_DINTCH1_ISR（）；
   EALLOW;	// Allow access to EALLOW protected registers
   PieVectTable.DINTCH1= &local_DINTCH1_ISR;
   EDIS;   // Disable access to EALLOW protected registers
      
   // 使能INT7中断（DMA Ch1 中断在INT7组，bit1位）
   IER = M_INT7 ;	   //Enable INT7 (7.1 DMA Ch1)
   // 使能PIE（ENPIE）中断和全部响应（PIEACK）位
   EnableInterrupts();
   
   // 使能ADC模块，并上电
   InitAdc();  // For this example, init the ADC

   SetAdcParm();// 设置ADC模块有关参数


   // 以下为DMA CH1 有关配置
	DMAInitialize(); // DMA硬件复位

	// Clear Table
   for (i=0; i<BUF_SIZE; i++)
   {
     DMABuf1[i] = 0xffff;
   }

	
   // Configure DMA Channel
   // DMA 通道1 配置
   // 配置DMA传输通道的目标寄存器地址
    DMADest   = &DMABuf1[0];              //Point DMA destination to the beginning of the array
	// 配置DMA传输通道的源寄存器地址，取ADCRESULT0
    DMASource = &AdcMirror.ADCRESULT0;    //Point DMA source to ADC result register base
	// 配置DMA CH1的源寄存器和目标寄存器的起始地址和当前使用地址
    DMACH1AddrConfig(DMADest,DMASource);

    // 配置CH1的时隙（Burst）
    // DMACH1BurstConfig(Uint16 bsize, int16 srcbstep, int16 desbstep)
    // 每个burst片：16字，源burst地址增量为1，目标burst地址增量为10。
    // ADCRESULT0~ADCRESULT15的采样值，作一个传输时隙（burst），每个时隙16字，源地址增量为1，目标地址增量为10；
    // 目标地址增量为10表示在DMABuf1[]中，DMABuf1[0~9]单元为ADCIN0通道的10个采样数据，DMABuf1[10~19]单元为ADCIN1通道的10个采样数据

    DMACH1BurstConfig(15,1,10);

    // DMA CH1的传输配置
    // tsize：每次传输的时隙（burst）数：tsize+1
    // srctstep：每次完成一个时隙的源地址步长（有符号数），循环寻址时忽略该参数
    // deststep：每次完成一个时隙的目标地址步长（有符号数），循环寻址时忽略该参数
    // 每次传输10个时隙，源地址增量：-15，目标地址增量：(-150 + 1)
    // 9表示共传输10个时隙，-15表示每个时隙结束时(在ADCRESULT15)，回到ADCRESULT0（地址减15）
    // 目标地址增量：(-150 + 1)表示目标地址回到DMABuf1[0~9]中对应的位置
    DMACH1TransferConfig(9,-15,(-150 + 1));

    // N个时隙后的循环寻址配置
    // srcwsize：源循环寻址大小
    // srcwstep：源循环寻址步长
    // deswsize：目标循环寻址大小
    // deswstep：目标循环寻址步长
    // 本实验不使用循环寻址
    DMACH1WrapConfig(100,100,100,100);	  //Don't use wrap function

    //  DMA CH1 工作模式配置
    // void DMACH1ModeConfig(Uint16 persel, Uint16 perinte, Uint16 oneshot, Uint16 cont, ...
    // ... Uint16 synce, Uint16 syncsel, Uint16 ovrinte, Uint16 datasize, Uint16 chintmode, Uint16 chinte)
    // persel：外设中断源选择位，0~31，本处：选择DMA_SEQ1INT中断
    // PERINT_ENABLE：外设中断触发DMA传输使能位：本处：使能外设中断触发DMA传输
    // ONESHOT_DISABLE=0：一个事件触发一个时隙传输，
    // CONT_DISABLE=0:每完成一个时隙传输，DMA就停止，RUNSTS bit to 0.
    // SYNC_DISABLE,SYNC_SRC,OVRFLOW_DISABLE,不考虑同步问题及溢出中断
    // SIXTEEN_BIT：数据传输16位模式
    // CHINT_END：在每个传输结束时产生中断给CPU
    // CHINT_ENABLE:使能各个通道给CPU的中断（通过PIE）
    DMACH1ModeConfig(DMA_SEQ1INT, PERINT_ENABLE, ONESHOT_DISABLE, CONT_DISABLE,SYNC_DISABLE,SYNC_SRC,
	                 OVRFLOW_DISABLE,SIXTEEN_BIT,CHINT_END,CHINT_ENABLE); 

	// DMA CH1 开始传输
    StartDMACH1();

   // Start SEQ1
   AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 0x1;    // 启动SEQ1排序器

   // 每次传输完毕就启动下一次传输，共10次。
   for(i=0;i<10;i++)
   {
    	for(j=0;j<1000;j++)   // 等待
    	{    }
        AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;    //Normally ADC will be tied to ePWM, or timed routine
   }	                                     //For this example will re-start manually                   

  // Waiting ADC finish
   while(!ADC_END);    // ADC_END=0

   // Translate DMABuf to ADC_Result
   // 将DMABuf1的数据转化为浮点数，存到ADC_Result.
   for (i=0; i<BUF_SIZE; i++)
   {
     ADC_Result[i] = (float)DMABuf1[i] * 3.0 / 4096.0;   // 将采样结果转化为浮点数
   }
   
   asm ("      ESTOP0");

   for(;;);
}

// INT7.1
interrupt void local_DINTCH1_ISR(void)     // DMA Channel 1
{
  
  // To receive more interrupts from this PIE group, acknowledge this interrupt 
	// 向相应的中断位写1，可以使该位清零，同时当中断组中的中断没有执行时，使能PIE脉冲中断进入CPU中断
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP7;
   ADC_END = 1;
}

// 设置ADC模块有关参数
void SetAdcParm(void)
{
	   // 设置ADC模块参数
	   // 设置设采集窗大小: ACQ_PS=0xF:最大值为15。本例ADC_SHCLK=0xF，为16个ADCLK周期
	   AdcRegs.ADCTRL1.bit.ACQ_PS       = ADC_SHCLK;
	   // 设置ADCLK时钟: f_ADCCLK=HSPCLK/(2*ADCCLKPS)/(ADCTRL1[7]+1)，本例：ADCTRL1[7]=0，为12.5MHz
	   AdcRegs.ADCTRL3.bit.ADCCLKPS     = ADC_CKPS;    // ADC_CKPS=1
	   // 设置排序器级联模式:SEQ_CASC：级联模式.1=级联运行模式，SEQ作一个16状态排序器
	   AdcRegs.ADCTRL1.bit.SEQ_CASC     = 1;          // 0 Non-Cascaded Mode; 1 Cascaded Mode（级联模式）
	   // 使能INT_SEQ1中断请求： INT_ENA_SEQ1=1（使能INT_SEQ1中断请求）
	   // 在16个通道采样完毕时发出请求，
	   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 0x1;
	   // 复位排序器：RST_SEQ1=1（排序器复位到CONV00状态）
	   AdcRegs.ADCTRL2.bit.RST_SEQ1     = 0x1;

	   // 设置排序器寄存器对应的AD输入通道:
	   // CONV00--->ADC输入通道A0（ADCINA0，42pin），.......
	   // CONV15--->ADC输入通道B7（ADCINB7，53pin）;
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

	   // 设置SEQ1的最大通道数，共16个输入通道
	   AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 15;   // Set up ADC to perform 4 conversions for every SOC

}
