
#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

// CPU时钟频率
#if (CPU_FRQ_150MHZ)
  	  #define CPU_CLK   150e6
#endif
#if (CPU_FRQ_100MHZ)
  	  #define CPU_CLK   100e6
#endif

// PWM脉冲频率，单位Hz, HSPCLKDIV和CLKDIV未分频
float PWM1_Freq=200;
Uint16 TBPrd1=200;
//#define TBPRD1        	CPU_CLK/PWM1_Freq-1      //增 或 减计数模式，
////#define TBPRD1        	CPU_CLK/(2*PWM1_Freq)  //增减计数模式


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
	EPwm1Regs.CMPB=(Uint16)(TBPrd1/8);
	EPwm1Regs.CMPCTL.bit.SHDWAMODE=CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.SHDWBMODE=CC_SHADOW;
	EPwm1Regs.CMPCTL.bit.LOADAMODE=CC_CTR_ZERO;
	EPwm1Regs.CMPCTL.bit.LOADBMODE=CC_CTR_ZERO;

	// LED驱动，L有效
	EPwm1Regs.AQCTLA.bit.ZRO=AQ_CLEAR;
	EPwm1Regs.AQCTLA.bit.CAU=AQ_SET;
	EPwm1Regs.AQCTLB.bit.ZRO=AQ_CLEAR;
	EPwm1Regs.AQCTLB.bit.CBU=AQ_SET;
}


