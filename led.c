
#include "led.h"
#include <avr/io.h>
#include <stdbool.h>

#include <avr/interrupt.h>
#include "debug.h"

#define DEBUG 0
#define PWM_PERIOD 10
#define COLOR_MAX 100

volatile uint8_t pulse_width_red;
volatile uint8_t pulse_width_green;
volatile uint8_t pulse_width_blue;
volatile bool led_blink_state = true;

void led_init() {
    DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
    led_off();
}

void led_blink_start(uint16_t compare_value) {
    // Prescaler 1024 & CTC Modus
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);

    // Set compare value
    OCR1A = compare_value - 1;

    // Enable Compare Interrupt
    TIMSK1 |= (1 << OCIE1A);
}

void led_blink_stop() {
    TCCR1A = 0x00;
    TCCR1B = 0x00;
    TCNT1 = 0x00;
    TIMSK1 = 0x00;
    led_blink_state = true;
}

/**
 * Toogles the blink state
 */
ISR(TIMER1_COMPA_vect) {
    led_blink_state = !led_blink_state;
}

void led_on(uint8_t brightness, Color color) {

    //Calculate the pulse with for all color channles
    pulse_width_red = (uint8_t) ((brightness * color.red) / (COLOR_MAX * PWM_PERIOD));
    pulse_width_green = (uint8_t) ((brightness * color.green) / (COLOR_MAX * PWM_PERIOD));
    pulse_width_blue = (uint8_t) ((brightness * color.blue) / (COLOR_MAX * PWM_PERIOD));

    // CTC Modus
    TCCR2A = (1 << WGM21);
    // 256 Prescaler 
    TCCR2B = (1 << CS22) | (1 << CS21);
    // Set compare value
    OCR2A = 1;
    // Enable Compare Interrupt
    TIMSK2 |= (1 << OCIE2A);
}

void led_off() {
    //Reset the PWM timer
    TCCR2A = 0;
    TCCR2B = 0;
    TIMSK2 &= ~(1 << OCIE2A);
    // Turn off LED
    PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2));
}

/**
 * Turns the color channels of the led on and off to emulate a PWM signal 
 */
ISR(TIMER2_COMPA_vect) {

    static uint8_t pwm_step = 0;

    // Turn the led off if blinkstate is false
    if (!led_blink_state) {
        PORTB &= ~((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2));
    } else {
        // Check if Green channel should be turned on or off
        if (pwm_step < pulse_width_green) {
            PORTB |= (1 << PORTB0);
        } else {
            PORTB &= ~(1 << PORTB0);
        }

        // Check if red channel should be turned on or off
        if (pwm_step < pulse_width_red) {
            PORTB |= (1 << PORTB1);
        } else {
            PORTB &= ~(1 << PORTB1);
        }

        // Check if blue channel should be turned on or off
        if (pwm_step < pulse_width_blue) {
            PORTB |= (1 << PORTB2);
        } else {
            PORTB &= ~(1 << PORTB2);
        }

        // Increment the PWM Step
        pwm_step++;

        // Reset the pwm counter if needed
        if (pwm_step >= PWM_PERIOD) {
            pwm_step = 0;
        }
    }
}



