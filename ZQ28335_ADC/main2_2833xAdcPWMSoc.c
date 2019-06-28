
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

// ADC start parameters,HSPCLK = 25.0 MHz
#if (CPU_FRQ_150MHZ)     // Default - 150 MHz SYSCLKOUT
  #define ADC_MODCLK 0x3 // HSPCLK = SYSCLKOUT/(2*ADC_MODCLK2) = 150/(2*3)   = 25.0 MHz
#endif
#if (CPU_FRQ_100MHZ)
  #define ADC_MODCLK 0x2 // HSPCLK = SYSCLKOUT/(2*ADC_MODCLK2) = 100/(2*2)   = 25.0 MHz
#endif

// Prototype statements for functions found within this file.
interrupt void adc_isr(void);

// Global variables used in this example:
Uint16 LoopCount;
Uint16 ConversionCount;
Uint16 Voltage1[10];
Uint16 Voltage2[10];


main()
{

   InitSysCtrl();

   EALLOW;
   SysCtrlRegs.HISPCP.all = ADC_MODCLK;
   EDIS;

   DINT;
   InitPieCtrl();
   IER = 0x0000;
   IFR = 0x0000;
   InitPieVectTable();

   EALLOW;
   PieVectTable.ADCINT = &adc_isr;
   EDIS;

   InitAdc(); // 使能ADC模块，并上电

   // Enable ADCINT in PIE
   PieCtrlRegs.PIEIER1.bit.INTx6 = 1;
   IER |= M_INT1;
   EINT;
   ERTM;

   LoopCount = 0;
   ConversionCount = 0;

   // Configure ADC
   AdcRegs.ADCMAXCONV.all = 0x0001;       // Setup 2 conv's on SEQ1
   AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x3; // Setup ADCINA3 as 1st SEQ1 conv.
   AdcRegs.ADCCHSELSEQ1.bit.CONV01 = 0x2; // Setup ADCINA2 as 2nd SEQ1 conv.
   AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;// Enable SOCA from ePWM to start SEQ1
   AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;  // Enable SEQ1 interrupt (every EOS)

   // Assumes ePWM1 clock is already enabled in InitSysCtrl();
   EPwm1Regs.ETSEL.bit.SOCAEN = 1;        // Enable SOC on A group
   EPwm1Regs.ETSEL.bit.SOCASEL = 4;       // Select SOC from from CPMA on upcount
   EPwm1Regs.ETPS.bit.SOCAPRD = 1;        // Generate pulse on 1st event
   EPwm1Regs.CMPA.half.CMPA = 0x0080;	  // Set compare A value
   EPwm1Regs.TBPRD = 0xFFFF;              // Set period for ePWM1
   EPwm1Regs.TBCTL.bit.CTRMODE = 0;		  // count up and start

// Wait for ADC interrupt
   for(;;)
   {
      LoopCount++;
   }

}


interrupt void  adc_isr(void)
{

  Voltage1[ConversionCount] = AdcRegs.ADCRESULT0 >>4;
  Voltage2[ConversionCount] = AdcRegs.ADCRESULT1 >>4;

  // If 40 conversions have been logged, start over
  if(ConversionCount == 9)
  {
     ConversionCount = 0;
  }
  else ConversionCount++;

  // Reinitialize for next ADC sequence
  AdcRegs.ADCTRL2.bit.RST_SEQ1 = 1;         // Reset SEQ1
  AdcRegs.ADCST.bit.INT_SEQ1_CLR = 1;       // Clear INT SEQ1 bit
  PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;   // Acknowledge interrupt to PIE

  return;
}



