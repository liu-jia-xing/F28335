// TI File $Revision: /main/1 $
// Checkin $Date: August 18, 2006   13:52:13 $
//###########################################################################
//
// FILE:   DSP2833x_EQep.h
//
// TITLE:  DSP2833x Enhanced Quadrature Encoder Pulse Module 
//         Register Bit Definitions.
//
//###########################################################################
// $TI Release: DSP2833x/DSP2823x Header Files V1.20 $
// $Release Date: August 1, 2008 $
//###########################################################################

#ifndef DSP2833x_EQEP_H
#define DSP2833x_EQEP_H


#ifdef __cplusplus
extern "C" {
#endif

//----------------------------------------------------
// Capture decoder control register bit definitions */                                    
struct QDECCTL_BITS {         // bits   description
   Uint16 rsvd1:5;            // 4:0    reserved

   // =1：反相输入，=0，无变化
   Uint16 QSP:1;              // 5      QEPS input polarity

   // =1：反相输入，=0，无变化
   Uint16 QIP:1;              // 6      QEPI input polarity

   // =1：反相输入，=0，无变化
   Uint16 QBP:1;              // 7      QEPB input polarity

   // =1：反相输入，=0，无变化
   Uint16 QAP:1;              // 8      QEPA input polarity

   // =1: 选择索引脉冲门
   Uint16 IGATE:1;            // 9      Index pulse gating option

   // =1：交换正交时钟输入，=0：不交换
   Uint16 SWAP:1;             // 10     CLK/DIR signal source for Position Counter

   // =1：不分频，上升沿计数，=0,2分频、上升/下降沿计数
   Uint16 XCR:1;              // 11     External clock rate

   // =1: 被选择引脚，=0：索引引脚
   Uint16 SPSEL:1;            // 12     Sync output pin select

   // =1：使能同步输出
   Uint16 SOEN:1;             // 13     Enable position compare sync

   // QSRC=00: 正交计数模式（QCLK=iclk，QDIR=iDIR）
   // QSRC=01: 正交计数模式（QCLK=xclk，QDIR=xDIR）
   // QSRC=10: 频率测量的递增计数模式（QCLK=xclk，QDIR=1）
   // QSRC=11: 频率测量的递减计数模式（QCLK=xclk，QDIR=0）
   Uint16 QSRC:2;             // 15:14  Position counter source
};

union QDECCTL_REG {
   Uint16              all;
   struct QDECCTL_BITS   bit;
};


//----------------------------------------------------
// QEP control register bit definitions */                                    
struct QEPCTL_BITS {           // bits   description

	// =1：QEP看门狗使能
	Uint16 WDE:1;               // 0      QEP watchdog enable

	// =1：QEP单元定时器使能
	Uint16 UTE:1;               // 1      QEP unit timer enable

	// =1：锁存直到超时
	// =0: 当CPU读QPOSCNT时，捕捉定时器和捕捉周期值分别锁存到QCTMRCNT和QCPRDLAT
	Uint16 QCLM:1;              // 2      QEP capture latch（锁存） mode

	// =1：使能
	Uint16 QPEN:1;              // 3      Quadrature position counter enable

	// =00: 保留
	// =01: 索引信号上升沿锁存位置计数器
	// =10: 索引信号下降沿锁存位置计数器
	// =11: 软件索引标识
	Uint16 IEL:2;               // 5:4    Index event latch

	// =0：在QEPS选中的上升沿锁存位置计数器（QPOSSLAT=POSCCNT）
	Uint16 SEL:1;               // 6      Strobe event latch

	// =1：初始化
	Uint16 SWI:1;               // 7      Software init position counter

	// =00/01: 保留
	// =10:    在QEPI信号的上升沿初始化位置计数器（QPOSCNT=QPOSINIT）
	// =11:    在QEPI信号的下降沿初始化位置计数器（QPOSCNT=QPOSINIT）
	Uint16 IEI:2;               // 9:8    Index event init of position count

	// =00/01: 保留
	// =10:    在QEPS信号的上升沿初始化位置计数器（QPOSCNT=QPOSINIT）
	// =11:    在QEPS信号的下降沿初始化位置计数器（QPOSCNT=QPOSINIT）(.....)
	Uint16 SEI:2;               // 11:10  Strobe event init

	// =00:    索引事件时位置计数器复位
	// =01:    最大位置时位置计数器复位
	// =10:    第一个位置索引事件时计数器复位
	// =11:    单位时间事件时计数器复位
   Uint16 PCRM:2;              // 13:12  Position counter reset
   Uint16 FREE_SOFT:2;         // 15:14  Emulation mode
};

union QEPCTL_REG {
   Uint16               all;
   struct QEPCTL_BITS   bit;
};         


//----------------------------------------------------
// Quadrature capture control register bit definitions */                                    
struct QCAPCTL_BITS {          // bits   description

	// k=0~1011: UPEVNT = QCLK/2^k,max: QCLK/2048
	Uint16 UPPS:4;              // 3:0    Unit position pre-scale

	// k=0~7:  SYSCLKOUT/(2^k)
	Uint16 CCPS:3;              // 6:4    QEP capture timer pre-scale

   Uint16 rsvd1:8;             // 14:7   reserved

   // =1: eQEP capture unit is enabled
   Uint16 CEN:1;               // 15     Enable QEP capture
};


union QCAPCTL_REG {
   Uint16               all;
   struct QCAPCTL_BITS  bit;
}; 



//----------------------------------------------------
// Position compare control register bit definitions */                                    
struct QPOSCTL_BITS {          // bits   description

	// k=0~0xFFF:     K * 4 * SYSCLKOUT cycles
	Uint16 PCSPW:12;            // 11:0   Position compare sync pulse width

	// =1,EN
	Uint16 PCE:1;               // 12     Position compare enable/disable

	// =1: Active LOW pulse output,=0:HIGH
	Uint16 PCPOL:1;             // 13     Polarity of sync output

	// =1: Load when QPOSCNT = QPOSCMP,=0: when QPOSCNT = 0
	Uint16 PCLOAD:1;            // 14     Position compare of shadow load

	// =1:y
	Uint16 PCSHDW:1;            // 15     Position compare shadow enable
};

union QPOSCTL_REG {
   Uint16               all;
   struct QPOSCTL_BITS  bit;
};         

//----------------------------------------------------
// QEP interrupt control register bit definitions */                                    
struct QEINT_BITS {          // bits   description
   Uint16 rsvd1:1;           // 0      reserved

   Uint16 PCE:1;             // 1      Position counter error
   Uint16 QPE:1;             // 2      Quadrature phase error
   Uint16 QDC:1;             // 3      Quadrature dir change
   Uint16 WTO:1;             // 4      Watchdog timeout
   Uint16 PCU:1;             // 5      Position counter underflow
   Uint16 PCO:1;             // 6      Position counter overflow
   Uint16 PCR:1;             // 7      Position compare ready
   Uint16 PCM:1;             // 8      Position compare match
   Uint16 SEL:1;             // 9      Strobe event latch
   Uint16 IEL:1;             // 10     Event latch
   Uint16 UTO:1;             // 11     Unit timeout
   Uint16 rsvd2:4;           // 15:12  reserved
};


union QEINT_REG {
   Uint16               all;
   struct QEINT_BITS    bit;
};


//----------------------------------------------------
// QEP interrupt status register bit definitions */                                    
struct QFLG_BITS {          // bits   description
   Uint16 INT:1;            // 0      Global interrupt 
   Uint16 PCE:1;            // 1      Position counter error
   Uint16 PHE:1;            // 2      Quadrature phase error
   Uint16 QDC:1;            // 3      Quadrature dir change
   Uint16 WTO:1;            // 4      Watchdog timeout
   Uint16 PCU:1;            // 5      Position counter underflow
   Uint16 PCO:1;            // 6      Position counter overflow
   Uint16 PCR:1;            // 7      Position compare ready
   Uint16 PCM:1;            // 8      Position compare match
   Uint16 SEL:1;            // 9      Strobe event latch
   Uint16 IEL:1;            // 10     Event latch
   Uint16 UTO:1;            // 11     Unit timeout
   Uint16 rsvd2:4;          // 15:12  reserved
};


union QFLG_REG {
   Uint16               all;
   struct QFLG_BITS     bit;
};

//----------------------------------------------------
// QEP interrupt force register bit definitions */                                    
struct QFRC_BITS {          // bits   description
   Uint16 reserved:1;       // 0      Reserved
   Uint16 PCE:1;            // 1      Position counter error
   Uint16 PHE:1;            // 2      Quadrature phase error
   Uint16 QDC:1;            // 3      Quadrature dir change
   Uint16 WTO:1;            // 4      Watchdog timeout
   Uint16 PCU:1;            // 5      Position counter underflow
   Uint16 PCO:1;            // 6      Position counter overflow
   Uint16 PCR:1;            // 7      Position compare ready
   Uint16 PCM:1;            // 8      Position compare match
   Uint16 SEL:1;            // 9      Strobe event latch
   Uint16 IEL:1;            // 10     Event latch
   Uint16 UTO:1;            // 11     Unit timeout
   Uint16 rsvd2:4;          // 15:12  reserved
};


union QFRC_REG {
   Uint16               all;
   struct QFRC_BITS     bit;
};

// V1.1 Added UPEVNT (bit 7) This reflects changes 
// made as of F2833x Rev A devices
//----------------------------------------------------
// QEP status register bit definitions */                                    
struct QEPSTS_BITS {          // bits   description
   Uint16 PCEF:1;             // 0      Position counter error
   Uint16 FIMF:1;             // 1      First index marker
   Uint16 CDEF:1;             // 2      Capture direction error
   Uint16 COEF:1;             // 3      Capture overflow error
   Uint16 QDLF:1;             // 4      QEP direction latch
   Uint16 QDF:1;              // 5      Quadrature direction
   Uint16 FIDF:1;             // 6      Direction on first index marker
   Uint16 UPEVNT:1;           // 7      Unit position event flag
   Uint16 rsvd1:8;            // 15:8   reserved
};

union QEPSTS_REG {
   Uint16               all;
   struct QEPSTS_BITS   bit;
};

//----------------------------------------------------

struct EQEP_REGS {
   Uint32              QPOSCNT;   // Position counter 
   Uint32              QPOSINIT;  // Position counter init
   Uint32              QPOSMAX;   // Maximum position count
   Uint32              QPOSCMP;   // Position compare
   Uint32              QPOSILAT;  // Index position latch
   Uint32              QPOSSLAT;  // Strobe position latch
   Uint32              QPOSLAT;   // Position latch
   Uint32              QUTMR;     // Unit timer
   Uint32              QUPRD;     // Unit period
   Uint16              QWDTMR;    // QEP watchdog timer
   Uint16              QWDPRD;    // QEP watchdog period
   union  QDECCTL_REG  QDECCTL;   // Quadrature decoder control
   union  QEPCTL_REG   QEPCTL;    // QEP control 
   union  QCAPCTL_REG  QCAPCTL;   // Quadrature capture control     
   union  QPOSCTL_REG  QPOSCTL;   // Position compare control
   union  QEINT_REG    QEINT;     // QEP interrupt control        
   union  QFLG_REG     QFLG;      // QEP interrupt flag
   union  QFLG_REG     QCLR;      // QEP interrupt clear                   
   union  QFRC_REG     QFRC;      // QEP interrupt force                   
   union  QEPSTS_REG   QEPSTS;    // QEP status
   Uint16              QCTMR;     // QEP capture timer
   Uint16              QCPRD;     // QEP capture period
   Uint16              QCTMRLAT;  // QEP capture latch
   Uint16              QCPRDLAT;  // QEP capture period latch
   Uint16              rsvd1[30]; // reserved
};

    
 

//---------------------------------------------------------------------------
// GPI/O External References & Function Declarations:
//
extern volatile struct EQEP_REGS EQep1Regs;
extern volatile struct EQEP_REGS EQep2Regs;



#ifdef __cplusplus
}
#endif /* extern "C" */

#endif  // end of DSP2833x_EQEP_H definition

//===========================================================================
// End of file.
//===========================================================================
