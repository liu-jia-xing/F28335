#include "F28x_Project.h"

void delay_loop(void);
void spi_xmit(Uint16 a);
void spi_fifo_init(void);
void spi_init(void);
void error(void);
void high_spi_init();
void init_high_spi_port();
#define SPI_BRR        ((200E6 / 4) / 500E3) - 1
void main(void)
{
   Uint16 sdata;  // send data
   Uint16 rdata;  // received data

   InitSysCtrl();

//   InitSpiaGpio();
   init_high_spi_port();
   DINT;

   InitPieCtrl();

   IER = 0x0000;
   IFR = 0x0000;

   InitPieVectTable();

   spi_fifo_init();     // Initialize the SPI FIFO

   sdata = 0x0000;
   for(;;)
   {
        //
        // Transmit data
        //
        spi_xmit(0x4233);
        delay_loop();
   }
        //

}

//
// delay_loop - Loop for a brief delay
//
void delay_loop()
{
    long i;
    for (i = 0; i < 1000000; i++) {}
}

//
// error - Error function that halts the debugger
//
void error(void)
{
    asm("     ESTOP0");     // Test failed!! Stop!
    for (;;);
}

//
// spi_xmit - Transmit value via SPI
//
void spi_xmit(Uint16 a)
{
    SpiaRegs.SPITXBUF = a;
}

//
// spi_fifo_init - Initialize SPIA FIFO
//
void spi_fifo_init()
{
    //
    // Initialize SPI FIFO registers
    //
    SpiaRegs.SPIFFTX.all = 0xE040;
    SpiaRegs.SPIFFRX.all = 0x2044;
    SpiaRegs.SPIFFCT.all = 0x0;
    //nihapasdf
    //
    // Initialize core SPI registers
    //
    //InitSpi();
    high_spi_init();
}

void high_spi_init()
{
    ClkCfgRegs.LOSPCP.bit.LSPCLKDIV = 0;
    SpiaRegs.SPICCR.bit.HS_MODE = 0x1;
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = 9;  //ÉèÖÃ²¨ÌØÂÊ 50/£¨9+1£©=5M

    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
      SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
      //SpiaRegs.SPICCR.bit.SPICHAR = (16-1);
      SpiaRegs.SPICCR.bit.SPICHAR = 7;
      SpiaRegs.SPICCR.bit.SPILBK = 1;
      //saflk
      // Enable master (0 == slave, 1 == master)
      // Enable transmission (Talk)
      // Clock phase (0 == normal, 1 == delayed)
      // SPI interrupts are disabled
      SpiaRegs.SPICTL.bit.MASTER_SLAVE = 1;
      SpiaRegs.SPICTL.bit.TALK = 1;
      SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
      SpiaRegs.SPICTL.bit.SPIINTENA = 0;

      // Set the baud rate
    //  SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = SPI_BRR;

      // Set FREE bit
      // Halting on a breakpoint will not halt the SPI
      SpiaRegs.SPIPRI.bit.FREE = 1;

      // Release the SPI from reset
      SpiaRegs.SPICCR.bit.SPISWRESET = 1;
}

void init_high_spi_port()
{
    EALLOW;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO58=3;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO59=3;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO60=3;
    GpioCtrlRegs.GPBGMUX2.bit.GPIO61=3;
        //
        // Enable internal pull-up for the selected pins
        //
        // Pull-ups can be enabled or disabled by the user.
        // This will enable the pullups for the specified pins.
        // Comment out other unwanted lines.
        //
        GpioCtrlRegs.GPBPUD.bit.GPIO58 = 0;  // Enable pull-up on GPIO58 (SPISIMOA)
    //  GpioCtrlRegs.GPBPUD.bit.GPIO5 = 0;   // Enable pull-up on GPIO5 (SPISIMOA)
        GpioCtrlRegs.GPBPUD.bit.GPIO59 = 0;  // Enable pull-up on GPIO59 (SPISOMIA)
    //  GpioCtrlRegs.GPBPUD.bit.GPIO3 = 0;   // Enable pull-up on GPIO3 (SPISOMIA)
        GpioCtrlRegs.GPBPUD.bit.GPIO60 = 0;  // Enable pull-up on GPIO60 (SPICLKA)
        GpioCtrlRegs.GPBPUD.bit.GPIO61 = 0;  // Enable pull-up on GPIO61 (SPISTEA)

        //
        // Set qualification for selected pins to asynch only
        //
        // This will select asynch (no qualification) for the selected pins.
        // Comment out other unwanted lines.
        //
        GpioCtrlRegs.GPBQSEL2.bit.GPIO58 = 3; // Asynch input GPIO58 (SPISIMOA)
    //  GpioCtrlRegs.GPBQSEL1.bit.GPIO5 = 3;  // Asynch input GPIO5 (SPISIMOA)
        GpioCtrlRegs.GPBQSEL2.bit.GPIO59 = 3; // Asynch input GPIO59 (SPISOMIA)
    //  GpioCtrlRegs.GPBQSEL1.bit.GPIO3 = 3;  // Asynch input GPIO3 (SPISOMIA)
        GpioCtrlRegs.GPBQSEL2.bit.GPIO60 = 3; // Asynch input GPIO60 (SPICLKA)
        GpioCtrlRegs.GPBQSEL2.bit.GPIO61 = 3; // Asynch input GPIO61 (SPISTEA)

        //
        //Configure SPI-A pins using GPIO regs
        //
        // This specifies which of the possible GPIO pins will be SPI functional
        // pins.
        // Comment out other unwanted lines.
        //
        GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 3; // Configure GPIO58 as SPISIMOA
    //  GpioCtrlRegs.GPBMUX1.bit.GPIO5 = 2;  // Configure GPIO5 as SPISIMOA
        GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 3; // Configure GPIO59 as SPISOMIA
    //  GpioCtrlRegs.GPBMUX1.bit.GPIO3 = 2;  // Configure GPIO3 as SPISOMIA
        GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 3; // Configure GPIO60 as SPICLKA
        GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 3; // Configure GPIO61 as SPISTEA
        EDIS;
}
