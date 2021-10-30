#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "common.h"
#include "dfplayermini.h"
#include "motion.h"
#include "eyes.h"

void init() {
    // Select 8 MHz internal osc (which due to PLLEN will be 32MHz)
    OSCCONbits.IRCF0 = 0;
    OSCCONbits.IRCF1 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF3 = 1;

    // Internal oscillator is used for system clock
    SCS1 = 1;

    // WDTCON: Watchdog timer period prescaler set to 1:65536
    WDTPS0 = 1;
    WDTPS1 = 1;
    WDTPS2 = 0;
    WDTPS3 = 1;

    // ADCON0: Disable adc module
    ADON = 0;

    // Disable all ADC pins - all pins are digial IO
    ANSELA = 0;
    ANSELC = 0;

    // Start with outputs off
    updPorts();

    // All inputs by default 
    TRISA = 255;
    TRISC = 255;

    // No pull-ups
    OPTION_REGbits.nWPUEN = 0;
    WPUA = 0;
}

void squawk() {
    dfpPlay(1, 1);
    motionOpenMouth();
    __delay_ms(700);
    motionCloseMouth();
}

void blink() {
    for (uint8_t d = 0; d < 3; d++) {
        eyesBlink();
        __delay_ms(3000);
    }
}

void main(void) {
    init();
    eyesBegin();
    dfpBegin();
    dfpSetVolume(30);
    motionBegin();

    eyesSet(EYE_ON, EYE_ON);
    motionTurnHead(NeckCentre, NeckLeft);

    while (1) {
        motionTurnHead(NeckLeft, NeckRight);
        blink();
        squawk();
        eyesWink(EYE_OFF, EYE_ON);
        motionTurnHead(NeckRight, NeckLeft);
        blink();
        squawk();
        eyesWink(EYE_ON, EYE_OFF);
    }
}