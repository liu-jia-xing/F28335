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
 * ADCʱ�ӣ�SYSCLK--->HISPCP��Ƶ��ϵ����1~14��--->ADCCLKPS[3~0]��Ƶ--->CPS��Ƶ��0 or 2��
 */
//---------------------------------------------------------------------------
// ADC Individual Register Bit Definitions:

struct ADCTRL1_BITS {     // bits  description

	Uint16  rsvd1:4;      // 3:0   reserved

    // =0��˫����ģʽ��SEQ1��SEQ2����������8״̬������ʹ��
    // =1����������ģʽ��SEQ������һ��16״̬������ʹ��
    Uint16  SEQ_CASC:1;   // 4     Cascaded sequencer mode

    // =0����ֹѭ�����ǣ�
    // =1������ѭ�����ǣ�������MAX_CONVn���õ����ͨ������������ģ��ͨ����ѭ��ת������
    Uint16  SEQ_OVRD:1;   // 5     Sequencer override

    // =0������ֹͣģʽ
    // =1����������ģʽ
    Uint16  CONT_RUN:1;   // 6     Continuous run

    // =0��ADCʱ�Ӳ���Ƶ
    // =1����HSPCLKԤ��Ƶ[ADCCLKPS[3~0]��ʱ��2��Ƶ
    Uint16  CPS:1;        // 7     ADC core clock pre-scalar

    // =0~15�� 1~16��ADCCLK����
    Uint16  ACQ_PS:4;     // 11:8  Acquisition window size

    // �������λ=soft free
    Uint16  SUSMOD:2;     // 13:12 Emulation suspend mode

    // =0:��Ч��
    // =1:��λ����ADCģ�顣
    Uint16  RESET:1;      // 14    ADC software reset

    Uint16  rsvd2:1;      // 15    reserved
};


union ADCTRL1_REG {
   Uint16                all;
   struct ADCTRL1_BITS   bit;
};


struct ADCTRL2_BITS {         // bits  description

    // =0:��ֹ
    // =1:SEQ2������ePWMx��SOCB�����ź�������
	Uint16  EPWM_SOCB_SEQ2:1; // 0     EPWM compare B SOC mask for SEQ2

	Uint16  rsvd1:1;          // 1     reserved

    // =0: ��ÿһ��SEQ2���н���ʱ����INT_SEQ2
    // =1: ��ÿ��һ��SEQ2���н���ʱ����INT_SEQ2
	Uint16  INT_MOD_SEQ2:1;   // 2     SEQ2 Interrupt mode

    // =0:
    // =1: ʹ��INT_SEQ2���ж�����
    Uint16  INT_ENA_SEQ2:1;   // 3     SEQ2 Interrupt enable

    Uint16  rsvd2:1;          // 4     reserved

    // =0:
    // =1: �ӵ�ǰֹͣλ����SEQ2������RET_SEQ2����ͬʱ��Ϊ1
    Uint16  SOC_SEQ2:1;       // 5     Start of conversion for SEQ2

    // =0:
    // =1:SEQ2��λ��CONV08
    Uint16  RST_SEQ2:1;       // 6     Reset SEQ2

    // =0:
    // =1: ͨ��GPIO�˿ڵ�A����������ADC��һ���Զ�ת�����У������ű�����ΪXINT2.
    Uint16  EXT_SOC_SEQ1:1;   // 7     External start of conversion for SEQ1

    // =0:
    // =1: SEQ1������ePWMx��SOCA�����ź�������
    Uint16  EPWM_SOCA_SEQ1:1; // 8     EPWM compare B SOC mask for SEQ1

    Uint16  rsvd3:1;          // 9     reserved

    // =0: ��ÿһ��SEQ1���н���ʱ����INT_SEQ1
    // =1: ��ÿ��һ��SEQ1���н���ʱ����INT_SEQ1
    Uint16  INT_MOD_SEQ1:1;   // 10    SEQ1 Interrupt mode

    // =0:
    // =1: ʹ��INT_SEQ1���ж�����
    Uint16  INT_ENA_SEQ1:1;   // 11    SEQ1 Interrupt enable

    Uint16  rsvd4:1;          // 12    reserved

    // SEQ1 ��SEQ ����ת��ת�������ź�λ��
    // =0:
    // =1: �ӵ�ǰֹͣλ����SEQ1������RET_SEQ1����ͬʱ��Ϊ1
    Uint16  SOC_SEQ1:1;       // 13    Start of conversion trigger for SEQ1

    // =0:
    // =1:SEQ1��λ��CONV00
    Uint16  RST_SEQ1:1;       // 14    Restart sequencer 1

    // =0:
    // =1:��������������EPWM SOCB�źſ�������ת������λ����������Ч
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

    // CONV1�����ͨ����
	Uint16  MAX_CONV1:4;      // 3:0   Max number of conversions

	// CONV2�����ͨ����
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

    // =0:˳�����
    // =1:ͬ������
	Uint16   SMODE_SEL:1;     // 0      Sampling mode select

    // ADCCLK=SYSCLK/(2*HISPCP)/(2*ADCCLKPS)/(2*CPS)=150/6/2/1=12.5MHz
	Uint16   ADCCLKPS:4;      // 4:1    ADC core clock divider

    // =0:ADCģ����磨��ѹ�Ͳο���·���⣩
    // =1:ADCģ���ϵ磨��ѹ�Ͳο���·���⣩
	Uint16   ADCPWDN:1;       // 5      ADC powerdown

    // =00:��ѹ�Ͳο���·
    // =11:��ѹ�Ͳο���·
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
extern volatile struct ADC_REGS AdcRegs;    // �����AdcRegsFile����
extern volatile struct ADC_RESULT_MIRROR_REGS AdcMirror;    // �����AdcMirrorFile����


#ifdef __cplusplus
}
#endif /* extern "C" */


#endif  // end of DSP2833x_ADC_H definition

//===========================================================================
// End of file.
//===========================================================================
