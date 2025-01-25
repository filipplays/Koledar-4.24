/**
 * @file dates.h
 * @author Filip Zevnik
 * @brief This file contains all of the functions that are used to operate with the date.
 * @version 4.24
 * @date 2024-07-26
 *
 *
 */

#ifndef dates_h
#define dates_h

enum Update_digit
{
    MINUTE,
    TWO_MINUTE,
    HOUR,
    TWO_HOUR,
    ALL
};

const uint8_t abyMonth_days_len[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 * @brief Returns 1 if the given year is a leap year and 0 if it is not.
 *
 * @param wYear
 * @return uint8_t
 */
uint8_t bLeap_year(uint16_t wYear);

/**
 * @brief Returns the day of the week given a date.
 *
 * @param byMonth
 * @param byDay
 * @param wYear
 * @return uint8_t
 */
uint8_t byDay_of_week(uint8_t byMonth, uint8_t byDay, uint16_t wYear);

/**
 * @brief Returns a 1 if the given date is in the summer time and 0 if it is not.
 *
 * This functions is used when displaying the time the GPS provides.
 *
 * @param wYear
 * @param byMonth
 * @param byDay
 * @return int
 */
uint8_t bSummer_time(uint16_t wYear, uint8_t byMonth, uint8_t byDay);

/**
 * @brief Returns the number of days left to a birtday.
 *
 * @param byBD_day The birthday day.
 * @param byBD_month The birthday month.
 * @param byDay
 * @param byMonth
 * @param wYear
 * @return int
 */
uint16_t wDay_diff(uint8_t byBD_day, uint8_t byBD_month, uint8_t byDay, uint8_t byMonth, uint16_t wYear);

/**
 * @brief Used to add the minutes counted by the timer to the g_byMinute variable.
 *
 * This function also calculates the minute, hour, day, and month overflow.
 *
 * @param byMinutes
 * @return uint8_t The number of digits the need to be updated.
 */
Update_digit eAdd_minutes(uint8_t byMinutes);

/**
 * @brief Used to update the time variables when collecting data from the GPS and inform about the
 * amount of data that needs updating on the display.
 *
 * @param byTime
 * @param byP_time
 * @return uint8_t
 */
uint8_t byUpdate_time(const uint8_t *byTime, uint8_t *byP_time);

/**
 * @brief Used to add one day to the current date and calculate the month and year (uses the global day month and year values).
 *
 */
void vAdd_day();

#endif