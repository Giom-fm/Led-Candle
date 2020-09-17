/* 
 * File:   config.h
 * Author: giom
 *
 * Created on 20. August 2020, 16:55
 */

#ifndef CONFIG_H
#define	CONFIG_H

// ## BLINK ####################################################################

#define LED_BLINK 244

// ## DELAY COLORS #############################################################

#define DELAY_BRIGHTNESS 50

// Red
#define DELAY_1_COLOR_RED 100
#define DELAY_1_COLOR_GREEN 0
#define DELAY_1_COLOR_BLUE 0

// Green
#define DELAY_2_COLOR_RED 0
#define DELAY_2_COLOR_GREEN 100
#define DELAY_2_COLOR_BLUE 0

// Blue
#define DELAY_3_COLOR_RED 0
#define DELAY_3_COLOR_GREEN 0
#define DELAY_3_COLOR_BLUE 100

// ## BRIGHTNESS ###############################################################

#define BRIGHTNESS_MAX 100
#define BRIGHTNESS_DEFAULT 50
#define BRIGHTNESS_STEP 25

// ## COLOR ####################################################################

//Orange
#define COLOR_1_RED 100
#define COLOR_1_GREEN 50
#define COLOR_1_BLUE 0

//Warm White
#define COLOR_2_RED 100
#define COLOR_2_GREEN 85
#define COLOR_2_BLUE 50

//Purple
#define COLOR_3_RED 100
#define COLOR_3_GREEN 0
#define COLOR_3_BLUE 100

// ## DELAY ####################################################################

// Walk delays
#define DELAY_WALK_1_MINUTES 1
#define DELAY_WALK_1_HOURS 0

#define DELAY_WALK_2_MINUTES 3
#define DELAY_WALK_2_HOURS 0

#define DELAY_WALK_3_MINUTES 7
#define DELAY_WALK_3_HOURS 0

// Deco delays
#define DELAY_DECO_1_MINUTES 30
#define DELAY_DECO_1_HOURS 0

#define DELAY_DECO_2_MINUTES 0
#define DELAY_DECO_2_HOURS 1

#define DELAY_DECO_3_MINUTES 0
#define DELAY_DECO_3_HOURS 3


// ## LOCK RESET ###############################################################

#define X_THRESHOLD 0.2
#define Y_THRESHOLD 0.2
#define Z_UP_THRESHOLD 1.2
#define Z_DOWN_THRESHOLD 0.8

// ## MOVEMENT #################################################################

#define LEFT_TRESHOLD -0.5
#define RIGHT_TRESHOLD 0.5
#define FORWARD_TRESHOLD -0.5
#define BACKWARD_TRESHOLD 0.5

// ## SETUP ####################################################################

#define SETUP_MAX_INDEX  2
#define SETUP_DEFAULT_INDEX 1
#define CONFIG_INDEX_DEFAULT 0

#endif	/* CONFIG_H */

