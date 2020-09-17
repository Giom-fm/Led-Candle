/* 
 * File:   led.h
 * Author: Guillaume Fournier-Mayer
 *
 * This module is used to controll the RGB LED by setting the color and brightness.
 * Created on 3. August 2020, 15:28
 */

#ifndef LED_H
#define	LED_H

#include <stdint.h>
#include "types.h"

void led_init();

/**
 * Turns the Led with the passed brightness and color on.
 * 
 * @param brightness The brightness of the LED.
 * @param color The Color of the LED.
*/
void led_on(uint8_t brightness, Color color);

/**
 * Turns off the led by disbaling the PWM Timer.
 */
void led_off();

/**
 * Starts blinking the LED. Note that you have to turn the led on first.
 */
void led_blink_start();

/**
 * Stops the blinking of the LED.
 */
void led_blink_stop();


#endif	/* LED_H */

