/* 
 * File:   types.h
 * Author: Guillaume Fournier-Mayer
 * This module defines some data structures that are used in the whole programm.
 * Created on 20. August 2020, 16:53
 */

#ifndef TYPES_H
#define	TYPES_H

/**
 * Holds all the states of the statemachine.
 */
typedef enum {
    LED_OFF,
    LED_ON_LANTERN,
    LED_ON_DECORATION,
    SETUP_DELAY,
    SETUP_COLOR,
    SETUP_BRIGHTNESS
} State;

/**
 * Represents the delay by storing minutes and hours.
 */
typedef struct {
    uint8_t minutes;
    uint8_t hours;
} Delay;

/**
 * Represents a RGB color.
 */
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

/**
 * Holds all needed config data.
 */
typedef struct {
    uint8_t brightness;
    Color color;
    Delay lantern_delay;
    Delay deco_delay;
} Config;

#endif	/* TYPES_H */

