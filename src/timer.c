#include "timer.h"


void timerInit() {
    cli();
    TCCR5A = 0;
    TCCR5B = 0;
    TCCR5C = 0;
    TIMSK5 = 0;
    TCCR5B |= (1<<WGM52);
    TCCR5B |= (1<<CS50);
    TCCR5B |= (1<<CS51);
    TIMSK5 |= (1<<OCIE5A);
    OCR5AH = (char) (100>>8);
    OCR5AL = (char) (100);
	sei();
}

// Timer interrupt
ISR( TIMER5_COMPA_vect ) {
    
}
