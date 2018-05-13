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
    OCR5AH = (char) (0x80>>8);
    OCR5AL = (char) (0x80);
    // activate all interrupts
	sei();
    loopCnt = 0;
}

// timer interrupt
ISR( TIMER5_COMPA_vect ) {
    loopCnt++;
    analogInterrupt();
    switchInterrupt();
    rampInterrupt();
    // encoderInterrupt();
    switch(loopCnt) {
#ifdef BLACK_RECOGNITION
    case 10:
        lightPrepare();
        break;
    case 15:
        lightMeasure();
        break;
#endif
    case 30:
        loopCnt = 0;
        break;
    }
}
