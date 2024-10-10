/**
 * @file timer.h
 * @author Filip Zevnik
 * @brief This file contains all of the functions needed to operate the timer used for keeping track of time when
 * GPS is not available.
 * @version 4.24
 * @date 2024-07-26
 *
 *
 */

#ifndef timer_h
#define timer_h

/**
 * @brief Used to initialize the timer to count seconds and trigger an output compare interrupt.
 *
 */
void vInit_timer();

/**
 * @brief Used to reset the timer counter value and the g_byTimer_cnt variable every time the GPS updates the time.
 *
 */
void vReset_timer();

#endif