#include "dfplayermini.h"

// UART Baud rate of DFPlayer Mini
#define BAUD 9600 

// Command packet values
#define START_BYTE      0x7E  
#define VERSION_BYTE    0xFF  
#define COMMAND_LENGTH  0x06  
#define ACKNOWLEDGE     0x00   
#define END_BYTE        0xEF

// Commands
#define CMD_INIT        0x3F
#define CMD_PLAY        0x0F
#define CMD_SETVOLUME   0x06

static void initUART(long int baudrate) {
    // Calculate SPBRG value for a low baud rate.
    unsigned int spbrg = (_XTAL_FREQ - baudrate * 64) / (baudrate * 64);

    if (spbrg > 255) {
        // We are in a high baud rate range so recalculate and set high baud rate flag
        spbrg = (_XTAL_FREQ - baudrate * 16) / (baudrate * 16);
        BRGH = 1;
    }

    if (spbrg < 256) {
        // Set-up the port into UART mode with appropriate baud and enable
        SPBRG = spbrg;
        SYNC = 0;
        SPEN = 1;

        APFCON0bits.RXDTSEL = 0; // Rx on RC5
        APFCON0bits.TXCKSEL = 0; // Tx on RC4

        CREN = 1; // Enable continuous reception
        TXEN = 1; // Enable transmission
    }
}

static void writeToUART(char data) {
    while (!TRMT);
    TXREG = data;
}

static void executeCommand(uint8_t command, uint8_t param1, uint8_t param2) {
    uint16_t checksum = -(VERSION_BYTE + COMMAND_LENGTH + command + ACKNOWLEDGE + param1 + param2);
    uint8_t checksumHighByte = checksum >> 8;
    uint8_t checksumLowByte = checksum & 0xff;

    writeToUART(START_BYTE);
    writeToUART(VERSION_BYTE);
    writeToUART(COMMAND_LENGTH);
    writeToUART(command);
    writeToUART(ACKNOWLEDGE);
    writeToUART(param1);
    writeToUART(param2);
    writeToUART(checksumHighByte);
    writeToUART(checksumLowByte);
    writeToUART(END_BYTE);
}

void dfpSetVolume(uint8_t volume) {
    if (volume > 30) volume = 30;
    executeCommand(CMD_SETVOLUME, 0x00, volume);
}

void dfpPlay(uint8_t folder, uint8_t file) {
    executeCommand(CMD_PLAY, folder, file);
}

void dfpBegin() {
    // Configure IO pins
    TRISAbits.TRISA2 = 1; // Audio Busy (IN)
    TRISCbits.TRISC4 = 0; // Tx (OUT)
    TRISCbits.TRISC5 = 1; // Rx (IN)

    // DF Player Mini uses UART for communication so we'll get that initialised
    initUART(BAUD);

    // The dataset suggests waiting a few seconds are power-up before sending commands
    __delay_ms(3000);
    executeCommand(CMD_INIT, 0x00, 0x00);

    // Following initialisation we'll give the player a little more time to
    // settle before setting the volume (to the minimum)
    __delay_ms(2000);
    dfpSetVolume(0);
}