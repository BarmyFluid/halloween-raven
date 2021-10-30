#include "eyes.h"
#include "common.h"

#define EyeLed1 sPORTCbits.RC1
#define EyeLed2 sPORTCbits.RC2

void eyesBegin() {
    // Configure IO pins
    TRISCbits.TRISC1 = 0; // Eye LED 1 (OUT)
    TRISCbits.TRISC2 = 0; // Eye LED 2 (OUT)
}

void eyesSet(eye_state_t left, eye_state_t right) {
    EyeLed1 = left;
    EyeLed2 = right;
    updPorts();
}

void eyesBlink() {
    eyesSet(EYE_OFF, EYE_OFF);
    updPorts();
    __delay_ms(150);
    eyesSet(EYE_ON, EYE_ON);
    updPorts();
}

void eyesWink(eye_state_t left, eye_state_t right) {
    eyesSet(left, right);
    __delay_ms(1000);
    eyesSet(EYE_ON, EYE_ON);
}