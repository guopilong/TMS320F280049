#include "F28x_Project.h"


//接收一个字节的数据，并发送到串口中
//
// Defines
//
// Define AUTOBAUD to use the autobaud lock feature
//#define AUTOBAUD

//
// Globals
//
uint16_t loopCounter = 0;

//
// Function Prototypes
//
void initSCIAEchoback(void);
void transmitSCIAChar(uint16_t a);
void transmitSCIAMessage(unsigned char * msg);
void initSCIAFIFO(void);

//
// Main
//
void main(void)
{
    uint16_t ReceivedChar;//接收的字节
    unsigned char *msg;//数据格式

    //
    // Initialize device clock and peripherals
    //
    InitSysCtrl();

    //
    // Initialize GPIO
    //
    InitGpio();

    //
    // For this example, only init the pins for the SCI-A port.
    // GPIO_SetupPinMux() - Sets the GPxMUX1/2 and GPyMUX1/2 register bits
    // GPIO_SetupPinOptions() - Sets the direction and configuration of GPIOs
    // These functions are found in the F28X7x_Gpio.c file.
    //

    GPIO_SetupPinMux(28, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_PUSHPULL);
    GPIO_SetupPinMux(29, GPIO_MUX_CPU1, 1);
    GPIO_SetupPinOptions(29, GPIO_OUTPUT, GPIO_ASYNC);

    //
    // Disable CPU interrupts
    //
    DINT;

    //
    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flags
    // are cleared.
    //
    InitPieCtrl();

    //
    // Disable CPU interrupts and clear all CPU interrupt flags
    //
    IER = 0x0000;
    IFR = 0x0000;

    //
    // Initialize the PIE vector table with pointers to the shell Interrupt
    // Service Routines (ISR)
    //
    InitPieVectTable();

    loopCounter = 0;

    initSCIAFIFO();                         // Initialize the SCI FIFO
    initSCIAEchoback();                     // Initialize SCI for echoback

    msg = "\r\n\n\nHello World!\0";
    transmitSCIAMessage(msg); //发射数据

    msg = "\r\nYou will enter a character, and the DSP will echo it back! \n\0";
    transmitSCIAMessage(msg); //发射数据

    for(;;)
    {
        msg = "\r\nEnter a character: \0";
        transmitSCIAMessage(msg);

        //
        // Wait for character
        //
        while(SciaRegs.SCIFFRX.bit.RXFFST == 0)
        {

        } // wait for empty state

        //
        // Get character
        //
        ReceivedChar = SciaRegs.SCIRXBUF.all;

        //
        // Echo character back
        //
        msg = "  You sent: \0";
        transmitSCIAMessage(msg);
        transmitSCIAChar(ReceivedChar);

        loopCounter++;
    }
}

//
//  initSCIAEchoback - Initialize SCI-A for echoback
//
void initSCIAEchoback(void)
{
    //
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    //
    SciaRegs.SCICCR.all = 0x0007;           // 1 stop bit,  No loopback
                                            // No parity, 8 char bits,
                                            // async mode, idle-line protocol
    SciaRegs.SCICTL1.all = 0x0003;          // enable TX, RX, internal SCICLK,
                                            // Disable RX ERR, SLEEP, TXWAKE
    SciaRegs.SCICTL2.all = 0x0003;
    SciaRegs.SCICTL2.bit.TXINTENA = 1;
    SciaRegs.SCICTL2.bit.RXBKINTENA = 1;

    //
    // SCIA at 9600 baud
    // @LSPCLK = 25 MHz (100 MHz SYSCLK) HBAUD = 0x01  and LBAUD = 0x44.
    //
    SciaRegs.SCIHBAUD.all = 0x0001;
    SciaRegs.SCILBAUD.all = 0x0044;

    SciaRegs.SCICTL1.all = 0x0023;          // Relinquish SCI from Reset
}

//
// transmitSCIAChar - Transmit a character from the SCI
//
void transmitSCIAChar(uint16_t a)
{
    while (SciaRegs.SCIFFTX.bit.TXFFST != 0)
    {

    }
    SciaRegs.SCITXBUF.all = a;
}

//
// transmitSCIAMessage - Transmit message via SCIA
//
void transmitSCIAMessage(unsigned char * msg)
{
    int i;
    i = 0;
    while(msg[i] != '\0')
    {
        transmitSCIAChar(msg[i]);
        i++;
    }
}

//
//按照先进先出的原则，缓冲器的设置
//
void initSCIAFIFO(void)
{
    SciaRegs.SCIFFTX.all = 0xE040;
    SciaRegs.SCIFFRX.all = 0x2044;
    SciaRegs.SCIFFCT.all = 0x0;
}

//
// End of file
//
