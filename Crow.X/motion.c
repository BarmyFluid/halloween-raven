#include "motion.h"
#include "common.h"

#define NeckServo sPORTAbits.RA0
#define MouthServo sPORTAbits.RA1
#define MouthOpened msToTcy(1.15)
#define MouthClosed msToTcy(0.55)
#define PERIOD msToTcy(20) // 20ms = 50Hz

volatile uint16_t pulseWidthNeck = NeckCentre;
volatile uint16_t pulseWidthMouth = MouthClosed;
volatile uint16_t pulsePeriod = PERIOD;
volatile uint8_t servo = 1;

static void interrupt ISR(void) {
    // To ensure correct timing we will only act when an overflow occurs
    if ((PIR1bits.CCP1IF & PIE1bits.CCP1IE) == 0) return;

    // This routine is based on the K8LH Crazy-8 PWM found here:
    // https://www.microchip.com/forums/m/tm.aspx?m=194482&p=2
    //    
    // Essentially we have a full period for the servo PWM in order to turn
    // pins on and off to create the correct pulse-widths to signal the servos
    // to go to the position we want. We can use a single timer and CCP and
    // just set one servo after the other keeping track of how much of the
    // period we have "consumed" padding the final timeout to achieve the full
    // PWM period. EG:
    //
    //           <----> 1ms
    //            ____
    // Servo 1 __|    |__________________________ _ _ _ __________
    //
    //                <----------> 2ms
    //                 __________
    // Servo 2 _______|          |_______________ _ _ _ __________
    //
    //
    // Period  <-------------------------------------------------> 20ms

    if (servo == 1) {
        CCPR1 += pulseWidthNeck;
        pulsePeriod -= pulseWidthNeck;
        NeckServo = 1;
        MouthServo = 0;
        servo++;
    } else if (servo == 2) {
        CCPR1 += pulseWidthMouth;
        pulsePeriod -= pulseWidthMouth;
        NeckServo = 0;
        MouthServo = 1;
        servo++;
    } else {
        CCPR1 += pulsePeriod;
        pulsePeriod = PERIOD;
        NeckServo = 0;
        MouthServo = 0;
        servo = 1;
    }

    PIR1bits.CCP1IF = 0;
    updPorts();
}

static void SetCCP() {
    // Configure Timer 1
    T1CONbits.T1CKPS = 0; // 1:1
    T1CONbits.TMR1CS = 0; // Fosc/4

    // Set the timer so it will rollover soon and we'll get an early interrupt
    TMR1 = 0xFFF0;

    // Compare mode, set output on match
    CCPR1 = 0;
    CCP1CONbits.CCP1M = 0b1000;

    // Enable CCP1 Interrupt
    PIR1bits.CCP1IF = 0;
    PIE1bits.CCP1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    // Start Timer 1
    T1CONbits.TMR1ON = 1;
}

void motionBegin() {
    // Configure IO pins
    TRISAbits.TRISA0 = 0; // Neck Servo (OUT)
    TRISAbits.TRISA1 = 0; // Mouth Servo (OUT)

    // Using the Capture, Compare & PWM to maintain an ISR timed to work with
    // the expected pulse-width of an SG90 servo
    SetCCP();
}

void motionOpenMouth() {
    ATOMIC pulseWidthMouth = MouthOpened;
}

void motionCloseMouth() {
    ATOMIC pulseWidthMouth = MouthClosed;
}

void motionTurnHead(const unsigned long from, const unsigned long to) {
    if (from > to) {
        for (uint16_t pw = from; pw >= to; pw -= usToTcy(10)) {
            __delay_ms(40);
            ATOMIC pulseWidthNeck = pw;
        }
    } else {
        for (uint16_t pw = from; pw <= to; pw += usToTcy(10)) {
            __delay_ms(40);
            ATOMIC pulseWidthNeck = pw;
        }
    }
}