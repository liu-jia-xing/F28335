// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:51:50 $
//###########################################################################
//
// FILE:   DSP2833x_Adc.h
//
// TITLE:  DSP2833x Device ADC Register Definitions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#ifndef DSP2833x_ADC_H
#define DSP2833x_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/*******************************
 * ADC时钟：SYSCLK--->HISPCP分频（系数：1~14）--->ADCCLKPS[3~0]分频--->CPS分频（0 or 2）
 */
//---------------------------------------------------------------------------
// ADC Individual Register Bit Definitions:

struct ADCTRL1_BITS {     // bits  description

	Uint16  rsvd1:4;      // 3:0   reserved

    // =0：双排序模式，SEQ1和SEQ2是用作两个8状态排序器使用
    // =1：级联运行模式，SEQ是用作一个16状态排序器使用
    Uint16  SEQ_CASC:1;   // 4     Cascaded sequencer mode

    // =0：禁止循环覆盖；
    // =1：允许循环覆盖（覆盖由MAX_CONVn设置的最大通道数，在整个模块通道内循环转换）；
    Uint16  SEQ_OVRD:1;   // 5     Sequencer override

    // =0：启动停止模式
    // =1：连续运行模式
    Uint16  CONT_RUN:1;   // 6     Continuous run

    // =0：ADC时钟不分频
    // =1：对HSPCLK预分频[ADCCLKPS[3~0]的时钟2分频
    Uint16  CPS:1;        // 7     ADC core clock pre-scalar

    // =0~15： 1~16个ADCCLK周期
    Uint16  ACQ_PS:4;     // 11:8  Acquisition window size

    // 仿真控制位=soft free
    Uint16  SUSMOD:2;     // 13:12 Emulation suspend mode

    // =0:无效；
    // =1:复位整个ADC模块。
    Uint16  RESET:1;      // 14    ADC software reset

    Uint16  rsvd2:1;      // 15    reserved
};


union ADCTRL1_REG {
   Uint16                all;
   struct ADCTRL1_BITS   bit;
};


struct ADCTRL2_BITS {         // bits  description

    // =0:禁止
    // =1:SEQ2可以由ePWMx的SOCB触发信号启动。
	Uint16  EPWM_SOCB_SEQ2:1; // 0     EPWM compare B SOC mask for SEQ2

	Uint16  rsvd1:1;          // 1     reserved

    // =0: 在每一个SEQ2序列结束时设置INT_SEQ2
    // =1: 在每隔一个SEQ2序列结束时设置INT_SEQ2
	Uint16  INT_MOD_SEQ2:1;   // 2     SEQ2 Interrupt mode

    // =0:
    // =1: 使能INT_SEQ2的中断请求
    Uint16  INT_ENA_SEQ2:1;   // 3     SEQ2 Interrupt enable

    Uint16  rsvd2:1;          // 4     reserved

    // =0:
    // =1: 从当前停止位启动SEQ2，它与RET_SEQ2不能同时设为1
    Uint16  SOC_SEQ2:1;       // 5     Start of conversion for SEQ2

    // =0:
    // =1:SEQ2复位到CONV08
    Uint16  RST_SEQ2:1;       // 6     Reset SEQ2

    // =0:
    // =1: 通过GPIO端口的A组引脚启动ADC的一个自动转换序列，该引脚被配置为XINT2.
    Uint16  EXT_SOC_SEQ1:1;   // 7     External start of conversion for SEQ1

    // =0:
    // =1: SEQ1可以由ePWMx的SOCA触发信号启动。
    Uint16  EPWM_SOCA_SEQ1:1; // 8     EPWM compare B SOC mask for SEQ1

    Uint16  rsvd3:1;          // 9     reserved

    // =0: 在每一个SEQ1序列结束时设置INT_SEQ1
    // =1: 在每隔一个SEQ1序列结束时设置INT_SEQ1
    Uint16  INT_MOD_SEQ1:1;   // 10    SEQ1 Interrupt mode

    // =0:
    // =1: 使能INT_SEQ1的中断请求
    Uint16  INT_ENA_SEQ1:1;   // 11    SEQ1 Interrupt enable

    Uint16  rsvd4:1;          // 12    reserved

    // SEQ1 或SEQ 启动转换转换触发信号位。
    // =0:
    // =1: 从当前停止位启动SEQ1，它与RET_SEQ1不能同时设为1
    Uint16  SOC_SEQ1:1;       // 13    Start of conversion trigger for SEQ1

    // =0:
    // =1:SEQ1复位到CONV00
    Uint16  RST_SEQ1:1;       // 14    Restart sequencer 1

    // =0:
    // =1:允许级联排序器由EPWM SOCB信号控制启动转换，该位仅级联下有效
    Uint16  EPWM_SOCB_SEQ:1;  // 15    EPWM compare B SOC enable

};


union ADCTRL2_REG {
   Uint16                all;
   struct ADCTRL2_BITS   bit;
};


struct ADCASEQSR_BITS {       // bits   description
    Uint16  SEQ1_STATE:4;     // 3:0    SEQ1 state
    Uint16  SEQ2_STATE:3;     // 6:4    SEQ2 state
    Uint16  rsvd1:1;          // 7      reserved
    Uint16  SEQ_CNTR:4;       // 11:8   Sequencing counter status 
    Uint16  rsvd2:4;          // 15:12  reserved  
};

union ADCASEQSR_REG {
   Uint16                 all;
   struct ADCASEQSR_BITS  bit;
};


struct ADCMAXCONV_BITS {      // bits  description

    // CONV1的最大通道数
	Uint16  MAX_CONV1:4;      // 3:0   Max number of conversions

	// CONV2的最大通道数
    Uint16  MAX_CONV2:3;      // 6:4   Max number of conversions

    Uint16  rsvd1:9;          // 15:7  reserved 
};

union ADCMAXCONV_REG {
   Uint16                  all;
   struct ADCMAXCONV_BITS  bit;
};


struct ADCCHSELSEQ1_BITS {    // bits   description
    Uint16  CONV00:4;         // 3:0    Conversion selection 00
    Uint16  CONV01:4;         // 7:4    Conversion selection 01
    Uint16  CONV02:4;         // 11:8   Conversion selection 02
    Uint16  CONV03:4;         // 15:12  Conversion selection 03
};

union  ADCCHSELSEQ1_REG{
   Uint16                    all;
   struct ADCCHSELSEQ1_BITS  bit;
};

struct ADCCHSELSEQ2_BITS {    // bits   description
    Uint16  CONV04:4;         // 3:0    Conversion selection 04
    Uint16  CONV05:4;         // 7:4    Conversion selection 05
    Uint16  CONV06:4;         // 11:8   Conversion selection 06
    Uint16  CONV07:4;         // 15:12  Conversion selection 07
};

union  ADCCHSELSEQ2_REG{
   Uint16                    all;
   struct ADCCHSELSEQ2_BITS  bit;
};

struct ADCCHSELSEQ3_BITS {    // bits   description
    Uint16  CONV08:4;         // 3:0    Conversion selection 08
    Uint16  CONV09:4;         // 7:4    Conversion selection 09
    Uint16  CONV10:4;         // 11:8   Conversion selection 10
    Uint16  CONV11:4;         // 15:12  Conversion selection 11
};

union  ADCCHSELSEQ3_REG{
   Uint16                    all;
   struct ADCCHSELSEQ3_BITS  bit;
};

struct ADCCHSELSEQ4_BITS {    // bits   description
    Uint16  CONV12:4;         // 3:0    Conversion selection 12
    Uint16  CONV13:4;         // 7:4    Conversion selection 13
    Uint16  CONV14:4;         // 11:8   Conversion selection 14
    Uint16  CONV15:4;         // 15:12  Conversion selection 15
};

union  ADCCHSELSEQ4_REG {
   Uint16                    all;
   struct ADCCHSELSEQ4_BITS  bit;
};

struct ADCTRL3_BITS {         // bits   description

    // =0:顺序采样
    // =1:同步采样
	Uint16   SMODE_SEL:1;     // 0      Sampling mode select

    // ADCCLK=SYSCLK/(2*HISPCP)/(2*ADCCLKPS)/(2*CPS)=150/6/2/1=12.5MHz
	Uint16   ADCCLKPS:4;      // 4:1    ADC core clock divider

    // =0:ADC模块掉电（稳压和参考电路除外）
    // =1:ADC模块上电（稳压和参考电路除外）
	Uint16   ADCPWDN:1;       // 5      ADC powerdown

    // =00:稳压和参考电路
    // =11:稳压和参考电路
	Uint16   ADCBGRFDN:2;     // 7:6    ADC bandgap/ref power down

	Uint16   rsvd1:8;         // 15:8   reserved
}; 

union  ADCTRL3_REG {
   Uint16                all;
   struct ADCTRL3_BITS   bit;
};


struct ADCST_BITS {           // bits   description
    Uint16   INT_SEQ1:1;      // 0      SEQ1 Interrupt flag  
    Uint16   INT_SEQ2:1;      // 1      SEQ2 Interrupt flag
    Uint16   SEQ1_BSY:1;      // 2      SEQ1 busy status
    Uint16   SEQ2_BSY:1;      // 3      SEQ2 busy status
    Uint16   INT_SEQ1_CLR:1;  // 4      SEQ1 Interrupt clear
    Uint16   INT_SEQ2_CLR:1;  // 5      SEQ2 Interrupt clear
    Uint16   EOS_BUF1:1;      // 6      End of sequence buffer1
    Uint16   EOS_BUF2:1;      // 7      End of sequence buffer2
    Uint16   rsvd1:8;         // 15:8   reserved
};

                             
union  ADCST_REG {            
   Uint16             all;    
   struct ADCST_BITS  bit;    
};                           

struct ADCREFSEL_BITS {       // bits   description
	Uint16   rsvd1:14;        // 13:0   reserved  
	Uint16   REF_SEL:2;       // 15:14  Reference select
};

union ADCREFSEL_REG {
	Uint16		all;
	struct ADCREFSEL_BITS bit;
};

struct ADCOFFTRIM_BITS{       // bits   description
	int16	OFFSET_TRIM:9;    // 8:0    Offset Trim  
	Uint16	rsvd1:7;          // 15:9   reserved
};

union ADCOFFTRIM_REG{
	Uint16		all;
	struct ADCOFFTRIM_BITS bit;
};

struct ADC_REGS {
    union ADCTRL1_REG      ADCTRL1;       // ADC Control 1
    union ADCTRL2_REG      ADCTRL2;       // ADC Control 2
    union ADCMAXCONV_REG   ADCMAXCONV;    // Max conversions
    union ADCCHSELSEQ1_REG ADCCHSELSEQ1;  // Channel select sequencing control 1
    union ADCCHSELSEQ2_REG ADCCHSELSEQ2;  // Channel select sequencing control 2
    union ADCCHSELSEQ3_REG ADCCHSELSEQ3;  // Channel select sequencing control 3
    union ADCCHSELSEQ4_REG ADCCHSELSEQ4;  // Channel select sequencing control 4
    union ADCASEQSR_REG    ADCASEQSR;     // Autosequence status register
    Uint16                 ADCRESULT0;    // Conversion Result Buffer 0
    Uint16                 ADCRESULT1;    // Conversion Result Buffer 1
    Uint16                 ADCRESULT2;    // Conversion Result Buffer 2
    Uint16                 ADCRESULT3;    // Conversion Result Buffer 3
    Uint16                 ADCRESULT4;    // Conversion Result Buffer 4
    Uint16                 ADCRESULT5;    // Conversion Result Buffer 5
    Uint16                 ADCRESULT6;    // Conversion Result Buffer 6
    Uint16                 ADCRESULT7;    // Conversion Result Buffer 7
    Uint16                 ADCRESULT8;    // Conversion Result Buffer 8
    Uint16                 ADCRESULT9;    // Conversion Result Buffer 9
    Uint16                 ADCRESULT10;   // Conversion Result Buffer 10
    Uint16                 ADCRESULT11;   // Conversion Result Buffer 11
    Uint16                 ADCRESULT12;   // Conversion Result Buffer 12
    Uint16                 ADCRESULT13;   // Conversion Result Buffer 13
    Uint16                 ADCRESULT14;   // Conversion Result Buffer 14
    Uint16                 ADCRESULT15;   // Conversion Result Buffer 15
    union ADCTRL3_REG      ADCTRL3;       // ADC Control 3  
    union ADCST_REG        ADCST;         // ADC Status Register
    Uint16				   rsvd1;
    Uint16                 rsvd2;
    union ADCREFSEL_REG    ADCREFSEL;     // Reference Select Register
    union ADCOFFTRIM_REG   ADCOFFTRIM;    // Offset Trim Register
};


struct ADC_RESULT_MIRROR_REGS
{
    Uint16                 ADCRESULT0;    // Conversion Result Buffer 0
    Uint16                 ADCRESULT1;    // Conversion Result Buffer 1
    Uint16                 ADCRESULT2;    // Conversion Result Buffer 2
    Uint16                 ADCRESULT3;    // Conversion Result Buffer 3
    Uint16                 ADCRESULT4;    // Conversion Result Buffer 4
    Uint16                 ADCRESULT5;    // Conversion Result Buffer 5
    Uint16                 ADCRESULT6;    // Conversion Result Buffer 6
    Uint16                 ADCRESULT7;    // Conversion Result Buffer 7
    Uint16                 ADCRESULT8;    // Conversion Result Buffer 8
    Uint16                 ADCRESULT9;    // Conversion Result Buffer 9
    Uint16                 ADCRESULT10;   // Conversion Result Buffer 10
    Uint16                 ADCRESULT11;   // Conversion Result Buffer 11
    Uint16                 ADCRESULT12;   // Conversion Result Buffer 12
    Uint16                 ADCRESULT13;   // Conversion Result Buffer 13
    Uint16                 ADCRESULT14;   // Conversion Result Buffer 14
    Uint16                 ADCRESULT15;   // Conversion Result Buffer 15
};

//---------------------------------------------------------------------------
// ADC External References & Function Declarations:
//
extern volatile struct ADC_REGS AdcRegs;    // 存放在AdcRegsFile段中
extern volatile struct ADC_RESULT_MIRROR_REGS AdcMirror;    // 存放在AdcMirrorFile段中


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of DSP2833x_ADC_H definition

//===========================================================================
// End of file.
//===========================================================================
