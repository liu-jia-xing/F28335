
//
// Included Files
//
#include "DSP28x_Project.h"     // Device Headerfile and Examples Include File

void scia_send(int a){
    //while(SciaRegs.SCIFFTX.bit.TXFFST!=0);

    SciaRegs.SCITXBUF=a;
}

void delay_us(Uint32 t)
{
    Uint32 i = t*30;
    DSP28x_usDelay(i);
}

interrupt void rxIsr(){

    GpioDataRegs.GPADAT.all=~SciaRegs.SCIRXBUF.all;
    GpioDataRegs.GPATOGGLE.bit.GPIO12=1;
    delay_us(1000000);
//    SciaRegs.SCIFFRX.bit.RXFFOVRCLR=1;      // Clear Overflow flag
//    SciaRegs.SCIFFRX.bit.RXFFINTCLR=1;      // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all|=0x100;          // Issue PIE ack

}

void main(){

    InitSysCtrl();

    InitSciaGpio();

    DINT;

    InitPieCtrl();

    IER=0x0000;

    IFR=0x0000;

    InitPieVectTable();

    SciaRegs.SCICCR.all=0x0007; //char len = 0x07+1=8-bits

    EALLOW;
    GpioCtrlRegs.GPADIR.all=0xffffffff;
    EDIS;

    SciaRegs.SCICTL1.bit.TXENA=1;
    SciaRegs.SCICTL1.bit.RXENA=1;
    //SciaRegs.SCICTL1.all=0x0003;

    //SciaRegs.SCICTL2.bit.TXINTENA=1;
    SciaRegs.SCICTL2.bit.RXBKINTENA=1;
    //SciaRegs.SCICTL2.all=0x0003;

    SciaRegs.SCICTL2.bit.TXINTENA=0;
    SciaRegs.SCICTL2.bit.RXBKINTENA=1;

    EALLOW;
    PieVectTable.SCIRXINTA=&rxIsr;
    EDIS;

    //
    // Enable interrupts required for this example
    //
    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER9.bit.INTx1=1;     // PIE Group 9, int1

    SciaRegs.SCIHBAUD=0x0001;
    SciaRegs.SCILBAUD=0x00E7;

    //SciaRegs.SCICTL1.bit.TXENA=1;
    //SciaRegs.SCICTL1.bit.RXENA=1;
    SciaRegs.SCICTL1.bit.SWRESET=1;
    //SciaRegs.SCICTL1.all=0x0023;

    IER=0x100;
    EINT;
    //ERTM;

    while(1){
        scia_send(0x73);
        delay_us(10000);
    }

}
