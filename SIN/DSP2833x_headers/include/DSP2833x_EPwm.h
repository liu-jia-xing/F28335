// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:52:10 $
//###########################################################################
//
// FILE:   DSP2833x_EPwm.h
//
// TITLE:  DSP2833x Enhanced PWM Module Register Bit Definitions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#ifndef DSP2833x_EPWM_H
#define DSP2833x_EPWM_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
// Time base control register bit definitions */                                    
struct TBCTL_BITS {          // bits   description

	// CTRMODE:=0，增计数，=1，减计数，=2，增减计数，=3，无效
	Uint16 CTRMODE:2;         // 1:0    Counter Mode

	// PHSEN:=0，不装载TBPHS; =1,同步时装载TBPHS的值
	Uint16 PHSEN:1;           // 2      Phase load enable

	// PRDLD:=0,从映射寄存器转载TBPRD,=1，从CPU直接装载TBPRD;
	Uint16 PRDLD:1;           // 3      Active period load

	// =0，EPWMxSYNC;
	// =1，CTR = zero: Time-base counter equal to zero (TBCTR = 0x0000)
	// =2，CTR = CMPB : Time-base counter equal to counter-compare B (TBCTR = CMPB)
	// =3，Disable EPWMxSYNCO signal
	Uint16 SYNCOSEL:2;        // 5:4    Sync output select,These bits select the source of the EPWMxSYNCO signal.

	// 写0无效，写1：强迫输出一个同步脉冲，仅SYNCOSEL = 00时有效。
	Uint16 SWFSYNC:1;         // 6      Software force sync pulse

	// pwm时钟来自SYSCLK，经HSPCLKDIV分频，再经CLKDIV分频，得到TBCLK
	// TBCLK = SYSCLKOUT / (HSPCLKDIV × CLKDIV)
	//  HSPCLKDIV:=0，不分频， =1~7: 系数2倍的分频；
	//  CLKDIV:=0，不分频，       =1~7: 系数的2次幂倍的分频；如=3，为8分频，=7，为128分频。
	Uint16 HSPCLKDIV:3;       // 9:7    High speed time pre-scale
	Uint16 CLKDIV:3;          // 12:10  Timebase clock pre-scale

	// =0，同步后减计数，=1，同步后增计数，在增减计数下无效。
	Uint16 PHSDIR:1;          // 13     Phase Direction

	Uint16 FREE_SOFT:2;       // 15:14  Emulation mode
};

union TBCTL_REG {
   Uint16              all;
   struct TBCTL_BITS   bit;
};

//----------------------------------------------------
// Time base status register bit definitions */                                    
struct TBSTS_BITS {          // bits   description
	// =0:Time-Base Counter is currently counting down.
	// =1:Time-Base Counter is currently counting up.
	Uint16 CTRDIR:1;          // 0      Counter direction status

	// Reading a 1 on this bit indicates that an external synchronization event has occurred
	// (EPWMxSYNCI). Writing a 1 to this bit will clear the latched event.
	Uint16 SYNCI:1;           // 1      External input sync status

	// Reading a 1 on this bit indicates that the time-base counter reached the max value 0xFFFF. Writing
	// a 1 to this bit will clear the latched event.
	Uint16 CTRMAX:1;          // 2      Counter max latched status

	Uint16 rsvd1:13;          // 15:3   reserved
};

union TBSTS_REG {
   Uint16              all;
   struct TBSTS_BITS   bit;
};

//----------------------------------------------------
// Compare control register bit definitions */                                    
struct CMPCTL_BITS {          // bits   description

	// =0,TBCTR=0时，装载CMPA;      =1,CTR=PRD时，装载CMPA;
	// =2,TBCTR=0和PRD时，装载CMPA;  =3，无效
	Uint16 LOADAMODE:2;        // 0:1    Active compare A

	// 与LOADAMODE类似，只是装载到CMPB
	Uint16 LOADBMODE:2;        // 3:2    Active compare B

	// =0，装载映射模式，=1：直接模式
	Uint16 SHDWAMODE:1;        // 4      Compare A block operating mode

	Uint16 rsvd1:1;            // 5      reserved

	Uint16 SHDWBMODE:1;        // 6      Compare B block operating mode

	Uint16 rsvd2:1;            // 7      reserved

	// =1:表示CMPA映射FIFO已满，CPU写入将覆盖当前映射值。
	Uint16 SHDWAFULL:1;        // 8      Compare A Shadow registers full Status

	Uint16 SHDWBFULL:1;        // 9      Compare B Shadow registers full Status

	Uint16 rsvd3:6;            // 15:10  reserved
};


union CMPCTL_REG {
   Uint16                all;
   struct CMPCTL_BITS    bit;
};

//----------------------------------------------------
// Action qualifier register bit definitions */
// Action qualifier(动作限定)分三种，A和B的动作模式相同，下面为A的模式：
// 0：无动作；                            1：清除，使EPWMxA输出L;
// 2：置位，使EPWMxA输出H;    3：切换EPWMxA输出，
struct AQCTL_BITS {           // bits   description

	// TBCLK=0,产生动作
	Uint16 ZRO:2;              // 1:0    Action Counter = Zero

	// TBCLK=PRD,产生动作
	Uint16 PRD:2;              // 3:2    Action Counter = Period

	// TBCLK=CMPA,且为UP计数时，产生动作(1: 输出L;   2：输出H，3：翻转)
	Uint16 CAU:2;              // 5:4    Action Counter = Compare A up

	// TBCLK=CMPA,且为DOWN计数时，产生动作
	Uint16 CAD:2;              // 7:6    Action Counter = Compare A down

	Uint16 CBU:2;              // 9:8    Action Counter = Compare B up

	Uint16 CBD:2;              // 11:10  Action Counter = Compare B down

	Uint16 rsvd:4;             // 15:12  reserved
};

union AQCTL_REG {
   Uint16                all;
   struct AQCTL_BITS     bit;
};

//----------------------------------------------------
// Action qualifier SW force register bit definitions */


struct AQSFRC_BITS {           // bits   description

	// Action When One-Time Software Force A Is Invoked
	// =00: Does nothing (action disabled)
	// =01: Clear (low)
	// =10: Set (high)
	// =11: Toggle (Low → High, High → Low)
	// Note: This action is not qualified by counter direction (CNT_dir)
	Uint16 ACTSFA:2;            // 1:0    Action when One-time SW Force A invoked

	// =1:Initiates a single software forced event
	Uint16 OTSFA:1;             // 2      One-time SW Force A output

	// 同 ACTSFA
	Uint16 ACTSFB:2;            // 4:3    Action when One-time SW Force B invoked

	// 同OTSFA
	Uint16 OTSFB:1;             // 5      One-time SW Force A output

	// AQCSFRC Active Register Reload From Shadow Options
	// =00: Load on event counter equals zero
	// =01: Load on event counter equals period
	// =10: Load on event counter equals zero or counter equals period
	// =11: Load immediately (the active register is directly accessed by the CPU and is not loaded from the
	// shadow register).
	Uint16 RLDCSF:2;            // 7:6    Reload from Shadow options

	Uint16 rsvd1:8;             // 15:8   reserved
};

union AQSFRC_REG {
   Uint16                 all;
   struct AQSFRC_BITS     bit;
};

//----------------------------------------------------
// Action qualifier continuous SW force register bit definitions */                                    
struct AQCSFRC_BITS {          // bits   description

	// Continuous Software Force on Output A
	// In immediate mode, a continuous force takes effect on the next TBCLK edge.
	// In shadow mode, a continuous force takes effect on the next TBCLK edge after a shadow load into
	// the active register.
	// 00 Forcing disabled, i.e., has no effect
	// 01 Forces a continuous low on output A
	// 10 Forces a continuous high on output A
	// 11 Software forcing is disabled and has no effect
	Uint16 CSFA:2;              // 1:0    Continuous Software Force on output A

	// as CSFA
	Uint16 CSFB:2;              // 3:2    Continuous Software Force on output B

	Uint16 rsvd1:12;            // 15:4   reserved
};

union AQCSFRC_REG {
   Uint16                  all;
   struct AQCSFRC_BITS     bit;
};


// As of version 1.1
// Changed the MODE bit-field to OUT_MODE
// Added the bit-field IN_MODE
// This corresponds to changes in silicon as of F2833x devices
// Rev A silicon.
//----------------------------------------------------
// Dead-band generator control register bit definitions                                    
struct DBCTL_BITS {          // bits   description

	// OUT_MODE:  Dead Band Output Mode Control
	// BIT1控制S1开关(ePWMxA信号)，BIT0控制S0开关(ePWMxB信号)
	// =00B: 对ePWMxA和ePWMxB，直通，无死区控制
	// =01B: ePWMxA直通,ePWMxB延迟，
	// =10B: ePWMxB直通,ePWMxA延迟，
	// =11B: ePWMxA和ePWMxB延迟，
	// 上升沿延迟时间由DBRED(10位计数器)的值决定，下降沿延迟时间由DBFED(10位计数器)的值决定
	Uint16 OUT_MODE:2;      	 // 1:0    Dead Band Output Mode Control

	// BIT3控制S3开关,BIT2控制S2开关;
	// =00B: 主高模式(AH),ePWMxA和ePWMxB均不反相；
	// =01B: 主低互补模式(ALC),ePWMxA反相；
	// =10B: 主高互补模式(AHC),ePWMxB反相；
	// =11B: 主低模式(AL),ePWMxA和ePWMxB均反相；
	Uint16 POLSEL:2;          // 3:2    Polarity Select Control

	// IN_MODE:  Dead Band Input Select Mode Control
	// BIT5控制S5开关,BIT4控制S4开关;
	// =00B: 上升沿延时模块和下降沿延迟模块的输入均来自ePWMxA输入
	// =01B: 上升沿延时模块的输入均来自ePWMxA输入,下降沿延迟模块的输入来自ePWMxB输入
	// =10B: 与01相反。
	// =11B:  上升沿延时模块和下降沿延迟模块的输入均来自ePWMxB输入
	Uint16 IN_MODE:2;         // 5:4    Dead Band Input Select Mode Control

	Uint16 rsvd1:10;          // 15:4   reserved
};

union DBCTL_REG {
   Uint16                  all;
   struct DBCTL_BITS       bit;
};


//----------------------------------------------------
// Trip zone(TZ) select register bit definitions
struct TZSEL_BITS {           // bits   description

	// =1: Enable TZ1 as a CBC trip source for this ePWM module
	Uint16  CBC1:1;            // 0      TZ1 CBC select
	Uint16  CBC2:1;            // 1      TZ2 CBC select
	Uint16  CBC3:1;            // 2      TZ3 CBC select
	Uint16  CBC4:1;            // 3      TZ4 CBC select
	Uint16  CBC5:1;            // 4      TZ5 CBC select
	Uint16  CBC6:1;            // 5      TZ6 CBC select

	Uint16  rsvd1:2;           // 7:6    reserved

	// =1: Enable TZ1 as a one-shot trip source for this ePWM module
	Uint16  OSHT1:1;           // 8      One-shot(一次性) TZ1 select
	Uint16  OSHT2:1;           // 9      One-shot TZ2 select
	Uint16  OSHT3:1;           // 10     One-shot TZ3 select
	Uint16  OSHT4:1;           // 11     One-shot TZ4 select
	Uint16  OSHT5:1;           // 12     One-shot TZ5 select
	Uint16  OSHT6:1;           // 13     One-shot TZ6 select

	Uint16  rsvd2:2;           // 15:14  reserved
};

union TZSEL_REG {
   Uint16                  all;
   struct TZSEL_BITS       bit;
};


//----------------------------------------------------
// Trip zone control register bit definitions */                                    
struct TZCTL_BITS {         // bits   description

	// When a trip event occurs the following action is taken on output EPWMxA. Which trip-zone pins
	// can cause an event is defined in the TZSEL register.
	// 00: High impedance (EPWMxA = High-impedance state)
	// 01: Force EPWMxA to a high state
	// 10: Force EPWMxA to a low state
	// 11: Do nothing, no action is taken on EPWMxA.
	Uint16 TZA:2;            // 1:0    TZ1 to TZ6 Trip Action On EPWMxA

	// as TZB
	Uint16 TZB:2;            // 3:2    TZ1 to TZ6 Trip Action On EPWMxB
	Uint16 rsvd:12;          // 15:4   reserved
};

union TZCTL_REG {
   Uint16                  all;
   struct TZCTL_BITS       bit;
};


//----------------------------------------------------
// Trip zone control register bit definitions */                                    
struct TZEINT_BITS {         // bits   description
   Uint16  rsvd1:1;          // 0      reserved

   // =1: Enable interrupt generation; a cycle-by-cycle trip event will cause an EPWMx_TZINT PIE interrupt.
   Uint16  CBC:1;            // 1      Trip Zones Cycle By Cycle Int Enable

   // =1: Enable Interrupt generation; a one-shot trip event will cause a EPWMx_TZINT PIE interrupt.
   Uint16  OST:1;            // 2      Trip Zones One Shot Int Enable

   Uint16  rsvd2:13;         // 15:3   reserved
};   


union TZEINT_REG {
   Uint16                  all;
   struct TZEINT_BITS      bit;
};


//----------------------------------------------------
// Trip zone flag register bit definitions */                                    
struct TZFLG_BITS {         // bits   description

	// =1: Indicates an EPWMx_TZINT PIE interrupt was generated because of a trip condition.
	Uint16  INT:1;           // 0      Global status

	// flag bit
	Uint16  CBC:1;           // 1      Trip Zones Cycle By Cycle Int

	// flag bit
	Uint16  OST:1;           // 2      Trip Zones One Shot Int

	// flag bit
	Uint16  rsvd2:13;        // 15:3   reserved
};

union TZFLG_REG {
   Uint16                  all;
   struct TZFLG_BITS       bit;
};

//----------------------------------------------------
// Trip zone flag clear register bit definitions */                                    
struct TZCLR_BITS {         // bits   description

	// flag clear bit
	Uint16  INT:1;           // 0      Global status

	// flag clear bit
	Uint16  CBC:1;           // 1      Trip Zones Cycle By Cycle Int

	// flag clear bit
	Uint16  OST:1;           // 2      Trip Zones One Shot Int

	Uint16  rsvd2:13;        // 15:3   reserved
};

union TZCLR_REG {
   Uint16                  all;
   struct TZCLR_BITS       bit;
};

//----------------------------------------------------
// Trip zone flag force register bit definitions */                                    
struct TZFRC_BITS {         // bits   description

	Uint16  rsvd1:1;         // 0      reserved

	// flag force bit
	Uint16  CBC:1;           // 1      Trip Zones Cycle By Cycle Int

	// flag force bit
	Uint16  OST:1;           // 2      Trip Zones One Shot Int

	Uint16  rsvd2:13;        // 15:3   reserved
};

union TZFRC_REG {
   Uint16                  all;
   struct TZFRC_BITS       bit;
};

//----------------------------------------------------
// Event trigger select register bit definitions */                                    
struct ETSEL_BITS {         // bits   description

	// ePWMx_INT生成选项，同SOCASEL和SOCBSEL。
	Uint16  INTSEL:3;        // 2:0    EPWMxINTn Select

	// =1：启用ePWMx_INT生成；=0 禁止
	Uint16  INTEN:1;         // 3      EPWMxINTn Enable

	Uint16  rsvd1:4;         // 7:4    reserved

	// 决定ePWMxSOCA脉冲何时被生成
	// 001:TBCTR=0时;
	// 010:TBCTR=TBPRD时;

	// 100:TBCTR=CMPA,且UP计数时;
	// 101:TBCTR=CMPA,且DOWN计数时;

	// 110:TBCTR=CMPB,且UP计数时;
	// 111:TBCTR=CMPB,且DOWN计数时;  其他，保留
	Uint16  SOCASEL:3;       // 10:8   Start of conversion A Select

	// =1：使能ePWMxSOCA脉冲（ADC开始转换A脉冲）;  =0，禁止。
	Uint16  SOCAEN:1;        // 11     Start of conversion A Enable

	// 决定ePWMxSOCB脉冲何时被生成
	// 001:TBCTR=0时;
	// 010:TBCTR=TBPRD时;

	// 100:TBCTR=CMPA,且UP计数时;
	// 101:TBCTR=CMPA,且DOWN计数时;

	// 110:TBCTR=CMPB,且UP计数时;
	// 111:TBCTR=CMPB,且DOWN计数时;  其他，保留
	Uint16  SOCBSEL:3;       // 14:12  Start of conversion B Select

	// =1：使能ePWMxSOCB脉冲（ADC开始转换B脉冲）;  =0，禁止。
	Uint16  SOCBEN:1;        // 15     Start of conversion B Enable
 };

union ETSEL_REG {
   Uint16                  all;
   struct ETSEL_BITS       bit;
};


//----------------------------------------------------
// Event trigger pre-scale register bit definitions */                                    
struct ETPS_BITS {         // bits   description

	// 在ETPS[INTCNT]=01时，发生中断（第一个事件），中断后ETPS[INTCNT]自动清零
	// 在ETPS[INTCNT]=10时，发生中断（第二个事件）
	// 在ETPS[INTCNT]=11时，发生中断（第三个事件）
	Uint16  INTPRD:2;       // 1:0    EPWMxINTn Period Select
	Uint16  INTCNT:2;       // 3:2    EPWMxINTn Counter Register

	Uint16  rsvd1:4;        // 7:4    reserved

	// =0;禁止
	// =1;在第一个事件上生成ePWMxSOCA脉冲，ETPS[SOCACNT]=01,一旦生成，ETPS[SOCACNT]自动清零
	// =2;在第二个事件上生成ePWMxSOCA脉冲，ETPS[SOCACNT]=10
	// =3;在第三个事件上生成ePWMxSOCA脉冲，ETPS[SOCACNT]=11
	Uint16  SOCAPRD:2;      // 9:8    EPWMxSOCA Period Select
	Uint16  SOCACNT:2;      // 11:10  EPWMxSOCA Counter Register

	// 同A
	Uint16  SOCBPRD:2;      // 13:12  EPWMxSOCB Period Select
	Uint16  SOCBCNT:2;      // 15:14  EPWMxSOCB Counter Register

};

union ETPS_REG {
   Uint16                  all;
   struct ETPS_BITS        bit;
};

//----------------------------------------------------
// Event trigger Flag register bit definitions */                                    
struct ETFLG_BITS {         // bits   description

	// Flag bit
	Uint16  INT:1;           // 0	EPWMxINTn Flag

	// Flag bit
	Uint16  rsvd1:1;         // 1	reserved

	// Flag bit
	Uint16  SOCA:1;          // 2	EPWMxSOCA Flag

	// Flag bit
	Uint16  SOCB:1;          // 3	EPWMxSOCB Flag

	Uint16  rsvd2:12;        // 15:4	reserved
};

union ETFLG_REG {
   Uint16                   all;
   struct ETFLG_BITS        bit;
};


//----------------------------------------------------
// Event trigger Clear register bit definitions */                                    
struct ETCLR_BITS {         // bits   description

	// clear flag bit
	Uint16  INT:1;           // 0	EPWMxINTn Clear

	// clear flag bit
	Uint16  rsvd1:1;         // 1	reserved

	// clear flag bit
	Uint16  SOCA:1;          // 2	EPWMxSOCA Clear

	// clear flag bit
	Uint16  SOCB:1;          // 3	EPWMxSOCB Clear

	Uint16  rsvd2:12;        // 15:4	reserved
};

union ETCLR_REG {
   Uint16                   all;
   struct ETCLR_BITS        bit;
};

//----------------------------------------------------
// Event trigger Force register bit definitions */                                    
struct ETFRC_BITS {         // bits   description

	// Force bit
	Uint16  INT:1;           // 0	EPWMxINTn Force

	// Force bit
	Uint16  rsvd1:1;         // 1	reserved

	// Force bit
	Uint16  SOCA:1;          // 2	EPWMxSOCA Force

	// Force bit
	Uint16  SOCB:1;          // 3	EPWMxSOCB Force

	Uint16  rsvd2:12;        // 15:4	reserved
};

union ETFRC_REG {
   Uint16                  all;
   struct ETFRC_BITS        bit;
};
//----------------------------------------------------
// PWM chopper （ 斩波器）control register bit definitions */
struct PCCTL_BITS {         // bits   description

	// =1: Enable chopping function(斩波功能)
	Uint16  CHPEN:1;         // 0      PWM chopping enable

	//0000~1111: (1~16) x SYSCLKOUT / 8 wide
	Uint16  OSHTWTH:4;       // 4:1    One-shot pulse width

	// 000~111:Divide by (1~8)  (=111: 1.56 MHz at 100 MHz SYSCLKOUT)
	Uint16  CHPFREQ:3;       // 7:5    Chopping clock frequency

	// 000~110: Duty = (1-7)/8 * 100%, 111:unused
	Uint16  CHPDUTY:3;       // 10:8   Chopping clock Duty cycle

	Uint16  rsvd1:5;         // 15:11  reserved
};


union PCCTL_REG {
   Uint16                  all;
   struct PCCTL_BITS       bit;
};

struct HRCNFG_BITS {       	// bits   description

	// Edge Mode Bits: Selects the edge of the PWM that is controlled by the micro-edge position (MEP) logic:
	// 00: HRPWM capability is disabled (default on reset)
	// 01: MEP control of rising edge
	// 10: MEP control of falling edge
	// 11: MEP control of both edges
	Uint16  EDGMODE:2;     	// 1:0    Edge Mode select Bits

	// Control Mode Bits: Selects the register (CMP or TBPHS) that controls the MEP:
	// 0: CMPAHR(8) Register controls the edge position ( i.e., this is duty control mode). (default on reset)
	// 1: TBPHSHR(8) Register controls the edge position ( i.e., this is phase control mode).
	Uint16  CTLMODE:1;     	// 2      Control mode Select Bit

	// Shadow mode bit: Selects the time event that loads the CMPAHR shadow value into the active register:
	// 0: CTR = zero (counter equal zero)
	// 1: CTR=PRD (counter equal period)
	// Note: Load mode selection is valid only if CTLMODE=0
	Uint16  HRLOAD:1;      	// 3      Shadow mode Select Bit

	Uint16  rsvd1:12;      	// 15:4   reserved
};

union HRCNFG_REG {
   Uint16                  	all;
   struct HRCNFG_BITS       bit;
};


struct TBPHS_HRPWM_REG {   	// bits   description

	// Time base phase high resolution bits(高8位有效)
	Uint16  TBPHSHR;     	// 15:0   Extension register for HRPWM Phase (8 bits)

	// Time-Base Phase Register(16位有效)
	Uint16  TBPHS;           // 31:16  Phase offset register
};

union TBPHS_HRPWM_GROUP {
   Uint32                  all;
   struct TBPHS_HRPWM_REG  half;
};

struct CMPA_HRPWM_REG {   	// bits   description

	// Compare A High Resolution Register (CMPAHR),高8位有效
	Uint16  CMPAHR;     	    // 15:0   Extension register for HRPWM compare (8 bits)

	// 比较器A,16位有效
	Uint16  CMPA;            // 31:16  Compare A reg
};

union CMPA_HRPWM_GROUP {
   Uint32                 all;
   struct CMPA_HRPWM_REG  half;
};


struct EPWM_REGS {
   union  TBCTL_REG           TBCTL;   // 
   union  TBSTS_REG           TBSTS;   // 
   union  TBPHS_HRPWM_GROUP   TBPHS;   // Union of TBPHS:TBPHSHR
   Uint16                     TBCTR;   // Counter
   Uint16                     TBPRD;   // Period register set 
   Uint16                     rsvd1;   // 
   union  CMPCTL_REG          CMPCTL;  // Compare control
   union  CMPA_HRPWM_GROUP    CMPA;    // Union of CMPA:CMPAHR
   Uint16                     CMPB;    // Compare B reg
   union  AQCTL_REG           AQCTLA;  // Action qual output A
   union  AQCTL_REG           AQCTLB;  // Action qual output B
   union  AQSFRC_REG          AQSFRC;  // Action qual SW force
   union  AQCSFRC_REG         AQCSFRC; // Action qualifier continuous SW force 
   union  DBCTL_REG           DBCTL;   // Dead-band control
   Uint16                     			DBRED;   // Dead-band rising edge delay
   Uint16                     			DBFED;   // Dead-band falling edge delay
   union  TZSEL_REG           TZSEL;   // Trip zone select
   Uint16                     rsvd2;   
   union  TZCTL_REG           TZCTL;   // Trip zone control
   union  TZEINT_REG          TZEINT;  // Trip zone interrupt enable
   union  TZFLG_REG           TZFLG;   // Trip zone interrupt flags
   union  TZCLR_REG           TZCLR;   // Trip zone clear   
   union  TZFRC_REG    	      TZFRC;   // Trip zone force interrupt
   union  ETSEL_REG           ETSEL;   // Event trigger selection
   union  ETPS_REG            ETPS;    // Event trigger pre-scaler
   union  ETFLG_REG           ETFLG;   // Event trigger flags
   union  ETCLR_REG           ETCLR;   // Event trigger clear   
   union  ETFRC_REG           ETFRC;   // Event trigger force
   union  PCCTL_REG           PCCTL;   // PWM chopper control
   Uint16                     		rsvd3;   //
   union  HRCNFG_REG          HRCNFG;  // HRPWM Config Reg
};
    
 

//---------------------------------------------------------------------------
// External References & Function Declarations:
//
extern volatile struct EPWM_REGS EPwm1Regs;
extern volatile struct EPWM_REGS EPwm2Regs;
extern volatile struct EPWM_REGS EPwm3Regs;
extern volatile struct EPWM_REGS EPwm4Regs;
extern volatile struct EPWM_REGS EPwm5Regs;
extern volatile struct EPWM_REGS EPwm6Regs;

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_EPWM_H definition

//===========================================================================
// End of file.
//===========================================================================
