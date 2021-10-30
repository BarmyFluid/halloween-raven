#ifndef COMMON_H
#define	COMMON_H

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL  // 8MHz default internal osc.

// Fake shadow port structures to avoid read-modify-write effect
// Diddle with the shadow (s in front of port name or portbits name)
// then use updPorts() to make the changes take effect
#define sPORTAbits (*(PORTAbits_t * volatile)&sPORTA)
#define sPORTCbits (*(PORTCbits_t * volatile)&sPORTC)
#define updPorts() (PORTA=sPORTA, PORTC=sPORTC)
volatile near uint8_t sPORTA = 0, sPORTC = 0;

// Makes the next statement or block an atomic operation (by disabling interrupts)
#define ATOMIC for(INTCONbits.GIE=0;!INTCONbits.GIE;INTCONbits.GIE=1)

// Helpers for converting times to cycles based on crystal frequency 
#define usToTcy(x) ((unsigned long)((x)*(_XTAL_FREQ/4000000.0)))
#define msToTcy(x) ((unsigned long)((x)*(_XTAL_FREQ/4000.0)))

#endif	/* COMMON_H */