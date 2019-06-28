
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"// DSP2833x Examples Include File
#include "PWM1Out.h"
// CPU时钟频率
#if (CPU_FRQ_150MHZ)
  	  #define CPU_CLK   150e6
#endif
#if (CPU_FRQ_100MHZ)
  	  #define CPU_CLK   100e6
#endif
/******************************************************/


/***************************************************/





// PWM脉冲频率，单位Hz, HSPCLKDIV和CLKDIV未分频
#define ADC_CKPS   0x1   // ADC module clock = HSPCLK/2*ADC_CKPS   = 25.0MHz/(1*2) = 12.5MHz
#define ADC_SHCLK  0xf   // S/H width in ADC module periods                        = 16 ADC clocks

void SetAdcParm(void)
{
	// Specific ADC setup for this example:
	AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;  // S/H 采样保持时间,16个ADCCLK
	AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;
	AdcRegs.ADCTRL1.bit.SEQ_CASC = 1;        // 1  Cascaded mode
	AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x0;   // 采样A0通道
	AdcRegs.ADCMAXCONV.bit.MAX_CONV1 = 0;  //实际值比设置值大1

	AdcRegs.ADCTRL1.bit.CONT_RUN = 1;       // Setup continuous run
}


void SetupEPwm1Parm()
{
	TBPrd1=(Uint16)(150e6/16/(PWM1_Freq)-1); //增 或 减计数模式,=46875<65535 OK

	EPwm1Regs.TBCTL.bit.HSPCLKDIV=TB_DIV4;  // TBCLK
	EPwm1Regs.TBCTL.bit.CLKDIV=TB_DIV4;
	EPwm1Regs.TBPRD=TBPrd1;          //PBPRD
	EPwm1Regs.TBCTL.bit.PRDLD=TB_SHADOW;

	EPwm1Regs.TBPHS.half.TBPHS = 0;      // PHASE
	EPwm1Regs.TBCTL.bit.PHSEN=TB_DISABLE;//禁止相位装载
	EPwm1Regs.TBCTL.bit.SYNCOSEL=TB_SYNC_DISABLE;

	EPwm1Regs.TBCTL.bit.CTRMODE=TB_COUNT_UP;  // 增计数
	EPwm1Regs.TBCTR=0;                    //TBCTR清零

	EPwm1Regs.CMPA.half.CMPA=(Uint16)(TBPrd1/2);
	//EPwm1Regs.CMPB=(Uint16)((TBPrd1*VinAvg)/8);
	EPwm1Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
	//EPwm1Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;
	//EPwm1Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;

	// LED驱动，L有效
	EPwm1Regs.AQCTLA.bit.ZRO=AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.CAU=AQ_SET;
	EPwm1Regs.AQCTLB.bit.ZRO=AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.CBU=AQ_SET;
}

interrupt void cpu_timer0_isr(void)
{

	CpuTimer0.InterruptCount++;
	EPwm1Regs.CMPA.half.CMPA=duty[CpuTimer0.InterruptCount-1];
	if(CpuTimer0.InterruptCount==64)
	    CpuTimer0.InterruptCount=0;
	//CpuTimer0.InterruptCount %= BUF_SIZE;

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}





