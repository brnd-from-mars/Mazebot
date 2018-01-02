#include "timer.h"


void timerInit() {
    //deactivate all interrupts
    cli();
    // reset timer settings
    TCCR5A = 0;
    TCCR5B = 0;
    TCCR5C = 0;
    // ctc mode
    TCCR5B |= (1<<WGM52);
    // set prescaler
    TCCR5B |= (1<<CS50);
    TCCR5B |= (1<<CS51);
    // set interrupt
    TIMSK5 = (1<<OCIE5A);
    // interval 1ms
    OCR5AH = (char) (100>>8);
    OCR5AL = (char) (100);
    // activate all interrupts
	sei();
}

// timer interrupt
ISR( TIMER5_COMPA_vect ) {
    Serial.println(millis())
}
