/*
 * config.hpp
 * created by Brendan Berg on 03.02.2018
 * 
 * Copyright (c) 2018 SiemBots
 */


/*
 * DIRECTIONS
 */

#define FRONT           0
#define RIGHT           1
#define BACK            2
#define LEFT            3

#define FRONT_RIGHT     0
#define BACK_RIGHT      1
#define BACK_LEFT       2
#define FRONT_LEFT      3

#define NORTH           0
#define EAST            1
#define WEST            2
#define SOUTH           3


/*
 * SHARP
 */

#define SHARP_SAMPLES   10
#define SHAPR_FILTER    100

#define WALLLIMIT_RECOG 200
#define WALLLIMIT_ALIGN 150


/*
 * ACCELEROMETER
 */

#define ACCEL_SAMPLES   5

#define TILT_UP_LIMIT   345
#define TILT_DOWN_LIMIT 320


/*
 * RAMP
 */

#define RAMP_MAX_MAYBE  200
#define RAMP_DECREMENT  4
#define RAMP_UP_MIN     60
#define RAMP_DOWN_MIN   30


/*
 * TOGGLESWITCH
 */

#define TOGGLE_RIGHT    0
#define TOGGLE_CENTER   1
#define TOGGLE_LEFT     2


/*
 * BUTTON
 */

#define DEBOUNCING      50

#define BUTTON_CENTER   0
#define BUTTON_UP       1
#define BUTTON_RIGHT    2
#define BUTTON_DOWN     3
#define BUTTON_LEFT     4