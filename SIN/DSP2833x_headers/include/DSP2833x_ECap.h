// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:52:07 $
//###########################################################################
//
// FILE:   DSP2833x_ECap.h
//
// TITLE:  DSP2833x Enhanced Capture Module Register Bit Definitions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#ifndef DSP2833x_ECAP_H
#define DSP2833x_ECAP_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
// Capture control register 1 bit definitions */                                    
struct ECCTL1_BITS {          // bits   description
	// =0:在上升沿（re）触发CAPTURE事件1
	// =1:在下降沿（fe）触发CAPTURE事件1
	Uint16 CAP1POL:1;          // 0      Capture Event 1 Polarity select
	Uint16 CTRRST1:1;          // 1      Counter Reset on Capture Event 1（=1时）

	Uint16 CAP2POL:1;          // 2      Capture Event 2 Polarity select
	Uint16 CTRRST2:1;          // 3      Counter Reset on Capture Event 2

	Uint16 CAP3POL:1;          // 4      Capture Event 3 Polarity select
	Uint16 CTRRST3:1;          // 5      Counter Reset on Capture Event 3

	Uint16 CAP4POL:1;          // 6      Capture Event 4 Polarity select
	Uint16 CTRRST4:1;          // 7      Counter Reset on Capture Event 4

	// =1: 在捕获事件时间使能CAP1~4的装载
	Uint16 CAPLDEN:1;          // 8      Enable Loading CAP1-4 regs on a Cap Event

	// =0~31，事件滤除预定表选择，分别除以（2*PRESCALE），PRESCALE=0时不分频；
	Uint16 PRESCALE:5;         // 13:9   Event Filter prescale select
	Uint16 FREE_SOFT:2;        // 15:14  Emulation mode
};

union ECCTL1_REG {
   Uint16              all;
   struct ECCTL1_BITS  bit;
};


// In V1.1 the STOPVALUE bit field was changed to 
// STOP_WRAP.  This correlated to a silicon change from
// F2833x Rev 0 to Rev A. 
//----------------------------------------------------
// Capture control register 2 bit definitions */                                    
struct ECCTL2_BITS {          // bits   description

	// =0: 运行于连续模式；
	// =1: 运行于单次模式；
	Uint16 CONT_ONESHT:1;      // 0      Continuous or one-shot

	// =00: (单次模式下），cap1之后停止，(连续模式下），cap1之后返回；
	// =01: (单次模式下），cap2之后停止，(连续模式下），cap2之后返回；
	// =10: (单次模式下），cap3之后停止，(连续模式下），cap3之后返回；
	// =11: (单次模式下），cap4之后停止，(连续模式下），cap4之后返回；
	Uint16 STOP_WRAP:2;        // 2:1    Stop value for one-shot, Wrap for continuous

	// =1: 单次序列强制：将MOD4计数器复位为0，启动MOD4计数器，使能捕获寄存器装载；
	Uint16 REARM:1;            // 3      One-shot re-arm

	// =0: TSCTR停止
	// =1: TSCTR运行
	Uint16 TSCTRSTOP:1;        // 4      TSCNT counter stop

	// =0: 禁止内部同步选择
	// =1: 通过SYNCI或sw强制事件使CTRPHS寄存器装载计数器；
	Uint16 SYNCI_EN:1;         // 5      Counter sync-in select

	// =00: 选择内部同步信号为外部同步信号
	// =01: 选择CTR=PRD事件作为外部同步信号
	// =10: 禁止外部同步信号
	// =11: 禁止外部同步信号
	Uint16 SYNCO_SEL:2;        // 7:6    Sync-out mode（外部同步选择）

	// =1: 写1时强制当前EPWM模块的TSCTR映射装载；
	Uint16 SWSYNC:1;           // 8      SW forced counter sync

	// =0: CAP功能，CAPx/APWMx引脚作捕获输入
	// =1: APWM功能，CAPx/APWMx引脚作捕获输出
	Uint16 CAP_APWM:1;         // 9      CAP/APWM operating mode select

	// =0: APWM输出为H电平有效
	// =1: APWM输出为L电平有效
	Uint16 APWMPOL:1;          // 10     APWM output polarity select

	Uint16 rsvd1:5;            // 15:11
};


union ECCTL2_REG {
   Uint16              all;
   struct ECCTL2_BITS  bit;
};


//----------------------------------------------------
// ECAP interrupt enable register bit definitions */                                    
struct ECEINT_BITS {          // bits   description
	Uint16 rsvd1:1;            // 0      reserved

	// =1: 允许捕获事件1作为中断源
	Uint16 CEVT1:1;            // 1      Capture Event 1 Interrupt Enable

	// =1: 允许捕获事件2作为中断源
	Uint16 CEVT2:1;            // 2      Capture Event 2 Interrupt Enable

	// =1: 允许捕获事件3作为中断源
	Uint16 CEVT3:1;            // 3      Capture Event 3 Interrupt Enable

	// =1: 允许捕获事件4作为中断源
	Uint16 CEVT4:1;            // 4      Capture Event 4 Interrupt Enable

	// =1: 允许计数器溢出作为中断源
	Uint16 CTROVF:1;           // 5      Counter Overflow Interrupt Enable

	// =1: 允许周期相等作为中断源
	Uint16 CTR_EQ_PRD:1;       // 6      Period Equal Interrupt Enable

	// =1: 允许比较器相等作为中断源
	Uint16 CTR_EQ_CMP:1;       // 7      Compare Equal Interrupt Enable

	Uint16 rsvd2:8;            // 15:8   reserved
};


union ECEINT_REG {
   Uint16              all;
   struct ECEINT_BITS  bit;
};

//----------------------------------------------------
// ECAP interrupt flag register bit definitions */                                    
struct ECFLG_BITS {           // bits   description
   Uint16 INT:1;              // 0      Global Flag
   Uint16 CEVT1:1;            // 1      Capture Event 1 Interrupt Flag
   Uint16 CEVT2:1;            // 2      Capture Event 2 Interrupt Flag
   Uint16 CEVT3:1;            // 3      Capture Event 3 Interrupt Flag
   Uint16 CEVT4:1;            // 4      Capture Event 4 Interrupt Flag         
   Uint16 CTROVF:1;           // 5      Counter Overflow Interrupt Flag
   Uint16 CTR_EQ_PRD:1;       // 6      Period Equal Interrupt Flag
   Uint16 CTR_EQ_CMP:1;       // 7      Compare Equal Interrupt Flag
   Uint16 rsvd2:8;            // 15:8   reserved
};


union ECFLG_REG {
   Uint16              all;
   struct ECFLG_BITS   bit;
};


//----------------------------------------------------

struct ECAP_REGS {
   Uint32              TSCTR;    // Time stamp counter(时间标签计数器)
   Uint32              CTRPHS;   // Counter phase(计数器相位寄存器，用于设置相位超前或滞后)
   Uint32              CAP1;     // Capture 1 
   Uint32              CAP2;     // Capture 2    
   Uint32              CAP3;     // Capture 3 
   Uint32              CAP4;     // Capture 4   
   Uint16              rsvd1[8]; // reserved
   union   ECCTL1_REG  ECCTL1;   // Capture Control Reg 1
   union   ECCTL2_REG  ECCTL2;   // Capture Control Reg 2
   union   ECEINT_REG  ECEINT;   // ECAP interrupt enable
   union   ECFLG_REG   ECFLG;    // ECAP interrupt flags
   union   ECFLG_REG   ECCLR;    // ECAP interrupt clear
   union   ECEINT_REG  ECFRC;    // ECAP interrupt force
   Uint16              rsvd2[6]; // reserved   
};

    
 

//---------------------------------------------------------------------------
// GPI/O External References & Function Declarations:
//
extern volatile struct ECAP_REGS ECap1Regs;
extern volatile struct ECAP_REGS ECap2Regs;
extern volatile struct ECAP_REGS ECap3Regs;
extern volatile struct ECAP_REGS ECap4Regs;
extern volatile struct ECAP_REGS ECap5Regs;
extern volatile struct ECAP_REGS ECap6Regs;


#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_ECAP_H definition

//===========================================================================
// End of file.
//===========================================================================
