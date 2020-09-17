/* 
 * File:   timer.h
 * Author: Guillaume Fournier-Mayer
 *
 * This module is used to controll the switch-off timer by starting, stoping or resting the Timer.
 * Created on 28. Juli 2020, 11:18
 */

#ifndef TIMER_H
#define	TIMER_H

#include <stdint.h>

volatile uint16_t timer_milliseconds;
volatile uint8_t timer_seconds;
volatile uint8_t timer_minutes;
volatile uint8_t timer_hours;


/**
 * Starts the clock
 */
void timer_clock_start();

/**
 * Stops the clock
 */
void timer_clock_stop();

/**
 * Resets the clock value to 00:00:00
 */
void timer_clock_reset();

#endif	/* TIMER_H */

