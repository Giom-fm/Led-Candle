#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "led.h"
#include "debug.h"

#define DEBUG 1

void timer_clock_start() {
    // Timer 0 Configuration

    // CTC Modus
    TCCR0A = (1 << WGM01);
    // Prescaler 8
    TCCR0B |= (1 << CS01);

    // Set compare value = 1ms
    OCR0A = 125 - 1;

    // Enable Compare Interrupt
    TIMSK0 |= (1 << OCIE0A);

    timer_clock_reset();
}

void timer_clock_stop() {
    TCCR0A = 0x0;
    TCCR0B = 0x0;
    TCNT0 = 0x0;
}

void timer_clock_reset() {
    timer_milliseconds = 0;
    timer_seconds = 0;
    timer_minutes = 0;
    timer_hours = 0;
}

ISR(TIMER0_COMPA_vect) {
    timer_milliseconds++;
    if (timer_milliseconds == 1000) {
        timer_milliseconds = 0;
        timer_seconds++;
        if (timer_seconds == 60) {
            timer_seconds = 0;
            timer_minutes++;
            if (timer_minutes == 60) {
                timer_hours++;
                if (timer_hours == 24) {
                    timer_hours = 0;
                }
            }
        }
    }
}
