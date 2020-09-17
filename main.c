/* 
 * File:   main.c
 * Author: Guillaume Fournier-Mayer
 * 
 * The main module that holds all the logic that is needed to 
 * Created on 8. Juli 2020, 15:02
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "debug.h"
#include "mpu6050.h"
#include "timer.h"
#include "led.h"
#include "types.h"
#include "config.h"

#define DEBUG 0

// #############################################################################

float a_x = 0, a_y = 0, a_z = 1;
#define MOVEMENT (a_x >= X_THRESHOLD) || (a_x <= -X_THRESHOLD) || (a_y >= Y_THRESHOLD) || (a_y <= -Y_THRESHOLD) || (a_z >= Z_UP_THRESHOLD) || (a_z <= Z_DOWN_THRESHOLD)
#define UP (a_z >= Z_UP_THRESHOLD)
#define DOWN (a_z <= -Z_DOWN_THRESHOLD)

#define LEFT (a_y <= LEFT_TRESHOLD)
#define RIGHT (a_y >= RIGHT_TRESHOLD)
#define FORWARD (a_x <= FORWARD_TRESHOLD)
#define BACKWARD (a_x >= BACKWARD_TRESHOLD)

bool lock;
State state;
Config config;
uint8_t setup_index = SETUP_DEFAULT_INDEX;

/**
 * An Array that holds all the setup colors presets
 */
const Color setup_colors[SETUP_MAX_INDEX + 1] = {
    {.red = DELAY_1_COLOR_RED, .green = DELAY_1_COLOR_GREEN, .blue = DELAY_1_COLOR_BLUE},
    {.red = DELAY_2_COLOR_RED, .green = DELAY_2_COLOR_GREEN, .blue = DELAY_2_COLOR_BLUE},
    {.red = DELAY_3_COLOR_RED, .green = DELAY_3_COLOR_GREEN, .blue = DELAY_3_COLOR_BLUE},
};

/**
 * An Array that holds all the lantern switch-off delays
 */
const Delay lantern_delays[SETUP_MAX_INDEX + 1] = {
    {.minutes = DELAY_WALK_1_MINUTES, .hours = DELAY_WALK_1_HOURS},
    {.minutes = DELAY_WALK_2_MINUTES, .hours = DELAY_WALK_2_HOURS},
    {.minutes = DELAY_WALK_3_MINUTES, .hours = DELAY_WALK_3_HOURS},
};

/**
 * An Array that holds all the decoration switch-off delays
 */
const Delay deco_delays[SETUP_MAX_INDEX + 1] = {
    {.minutes = DELAY_DECO_1_MINUTES, .hours = DELAY_DECO_1_HOURS},
    {.minutes = DELAY_DECO_2_MINUTES, .hours = DELAY_DECO_2_HOURS},
    {.minutes = DELAY_DECO_3_MINUTES, .hours = DELAY_DECO_3_HOURS},
};

/**
 * An Array that holds all the led color presets
 */
const Color led_colors[SETUP_MAX_INDEX + 1] = {
    {.red = COLOR_1_RED, .green = COLOR_1_GREEN, .blue = COLOR_1_BLUE},
    {.red = COLOR_2_RED, .green = COLOR_2_GREEN, .blue = COLOR_2_BLUE},
    {.red = COLOR_3_RED, .green = COLOR_3_GREEN, .blue = COLOR_3_BLUE},
};


// #############################################################################

/*
 * Initialize Setup with default values
 */
void init_config() {

    config.brightness = BRIGHTNESS_DEFAULT;
    config.color = led_colors[CONFIG_INDEX_DEFAULT];
    config.lantern_delay = lantern_delays[CONFIG_INDEX_DEFAULT];
    config.deco_delay = deco_delays[CONFIG_INDEX_DEFAULT];
}

/*
 * Initialize the sleep mode and actiavte external interrupt INT0
 */
void init_sleep_mode() {
    set_sleep_mode(SLEEP_MODE_STANDBY);
    // interrupt on rising edge of INT0
    EICRA |= (1 << ISC01) | (1 << ISC00);
    EIMSK |= (1 << INT0);
}

/**
 * Sets the Cpu to sleep while eliminating any race conditions
 */
void sleep() {
    cli();
    EIMSK |= (1 << INT0);
    sleep_enable();
    sei();
    sleep_cpu();
    sleep_disable();
}

/*
 * Initialize all needed modules
 */
void setup() {
#if DEBUG
    usart_init();
#endif
    mpu6050_init();
    led_init();
    init_sleep_mode();
    lock = false;
    state = LED_OFF;
    init_config();
    sei();
}

/**
 * The external interrupt wakeup ISR
 */
ISR(INT0_vect) {
    //Deactivate INT0 external interrupt
    EIMSK &= ~(1 << INT0);
}

/*
 * The main function
 */
int main(int argc, char** argv) {


    setup();

    while (true) {
        // Get new acceldata
        mpu6050_get_accel_data(&a_x, &a_y, &a_z);
        debug_print("Lock: %dtState: %d\r\n", lock, state);

        //Check if Lock can be reseted
        if (lock && a_x <= X_THRESHOLD && a_x >= -X_THRESHOLD && a_y <= Y_THRESHOLD && a_y >= -Y_THRESHOLD && a_z <= Z_UP_THRESHOLD && a_z >= Z_DOWN_THRESHOLD) {
            lock = false;
            led_blink_stop();
        } else {
            //Reactivate external interrupt INT0
            EIMSK |= (1 << INT0);
        }

        if (!lock) {
            switch (state) {
                case LED_OFF:
                {

                    if (FORWARD) { // Enter dealy setup menu
                        lock = true;
                        led_on(BRIGHTNESS_DEFAULT, setup_colors[SETUP_DEFAULT_INDEX]);
                        led_blink_start(LED_BLINK);
                        config.lantern_delay = lantern_delays[SETUP_DEFAULT_INDEX];
                        config.deco_delay = deco_delays[SETUP_DEFAULT_INDEX];
                        state = SETUP_DELAY;
                    } else if (UP) { // Enter Lantern Mode
                        lock = true;
                        timer_clock_start();
                        led_on(config.brightness, config.color);
                        state = LED_ON_LANTERN;
                    } else if (BACKWARD) { // Enter Decoration mode
                        lock = true;
                        timer_clock_start();
                        led_on(config.brightness, config.color);
                        state = LED_ON_DECORATION;
                    }
                    sleep();
                    break;
                }
                case LED_ON_LANTERN:
                {

                    if (MOVEMENT) { //Reset Timer if there is any movement
                        timer_clock_reset();
                    }

                    //Switch-off if times up or if switch-off movement has benen triggeredtime decay
                    if (DOWN || (timer_minutes >= config.lantern_delay.minutes && timer_hours >= config.lantern_delay.hours)) {
                        lock = true;
                        timer_clock_stop();
                        timer_clock_reset();
                        led_off();
                        state = LED_OFF;
                    }
                    debug_print("Clock: %d:%d:%d\r\n", timer_hours, timer_minutes, timer_seconds);
                    break;

                }
                case LED_ON_DECORATION:
                {


                    //Switch-off if times up or if switch-off movement has benen triggeredtime decay
                    if (DOWN || (timer_minutes >= config.deco_delay.minutes && timer_hours >= config.deco_delay.hours)) {
                        lock = true;
                        timer_clock_stop();
                        timer_clock_reset();
                        led_off();
                        state = LED_OFF;
                    }
                    debug_print("Clock: %d:%d:%d\r\n", timer_hours, timer_minutes, timer_seconds);
                    break;
                }
                case SETUP_DELAY:
                {
                    if (FORWARD) { // Enter color setup menu
                        lock = true;
                        setup_index = SETUP_DEFAULT_INDEX;
                        led_on(config.brightness, led_colors[setup_index]);
                        led_blink_start(LED_BLINK);
                        state = SETUP_COLOR;
                    } else if (RIGHT && setup_index < SETUP_MAX_INDEX) {
                        // Get a smaller delay value by decrementing the index
                        lock = true;
                        setup_index++;
                        config.lantern_delay = lantern_delays[setup_index];
                        config.deco_delay = deco_delays[setup_index];
                        led_on(DELAY_BRIGHTNESS, setup_colors[setup_index]);
                    } else if (LEFT && setup_index > 0) {
                        // Get a bigger delay value by incrementing the index
                        lock = true;
                        setup_index--;
                        config.lantern_delay = lantern_delays[setup_index];
                        config.deco_delay = deco_delays[setup_index];
                        led_on(DELAY_BRIGHTNESS, setup_colors[setup_index]);
                    }
                    break;
                }

                case SETUP_COLOR:
                {
                    if (FORWARD) { // Enter brightness setup menu
                        lock = true;
                        config.color = led_colors[setup_index];
                        led_blink_start(LED_BLINK);
                        setup_index = SETUP_DEFAULT_INDEX;
                        state = SETUP_BRIGHTNESS;
                    } else if (RIGHT && setup_index < SETUP_MAX_INDEX) {
                        // Get the next color by incrementing the index
                        lock = true;
                        setup_index++;
                        led_on(config.brightness, led_colors[setup_index]);
                    } else if (LEFT && setup_index > 0) {
                        // Get the previous color by decrementing the index
                        lock = true;
                        setup_index--;
                        led_on(config.brightness, led_colors[setup_index]);
                    }

                    break;
                }

                case SETUP_BRIGHTNESS:
                {

                    if (FORWARD) { // Setup is completed. Swtich-off!
                        lock = true;
                        timer_clock_stop();
                        timer_clock_reset();
                        led_off();
                        state = LED_OFF;

                    } else if (RIGHT && config.brightness < BRIGHTNESS_MAX) {
                        // Get a higher brightness value by incrementing the index
                        lock = true;
                        config.brightness += BRIGHTNESS_STEP;
                        led_on(config.brightness, config.color);

                    } else if (LEFT && config.brightness > BRIGHTNESS_STEP) {
                        // Get a deeper brightness value by decrementing the index
                        lock = true;
                        config.brightness -= BRIGHTNESS_STEP;
                        led_on(config.brightness, config.color);
                    }

                    break;
                }
            }
        }
    }



    return (EXIT_SUCCESS);
}

