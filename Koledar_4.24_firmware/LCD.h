/**
 * @file LCD.h
 * @author Filip Zevnik
 * @brief The functions used to manipulate the display.
 * @version 4.24
 * @date 2024-07-26
 *
 * All of the functions used to display characters or string to the display use the g_byText_size global variable
 * for font size.
 *
 */

#ifndef lcd_h
#define lcd_h

#include "BD.h"
#include "EEPROM_utils.h"
#include "dates.h"

#define CHAR_ROWS 12
#define CHAR_COLS 10
#define SPACEING 2

#define DISPLAY_ON_TIME_MS 5000
#define DISPLAY_OFF_HOUR 20
#define DISPLAY_ON_HOUR 8

const char c_acWeek_days[7][11] = {{"sobota"}, {"nedelja"}, {"ponedeljek"}, {"torek"}, {"sreda"}, {"\1etrtek"}, {"petek"}};
const uint8_t c_byWeek_days_len[7] = {6, 7, 10, 5, 5, 7, 5};

/**
 * @brief Used to display a character from the chars.h file by stroking its outline.
 *
 * The g_byText_size global variable is used to determine the size of the font.
 *
 * @param adwChar A variable from the chars.h file.
 * @param xPos  The x position of the top-left part of the char.
 * @param yPos The y position of the top-left part of the char.
 */
void vChar_stroke(const uint32_t adwChar[4], uint16_t xPos, uint16_t yPos);

/**
 * @brief Used to display a character from the chars.h file by using filled rectangles.
 *
 * The g_byText_size global variable is used to determine the size of the font.
 *
 * @param adwChar A variable from the chars.h file.
 * @param xPos  The x position of the top-left part of the char.
 * @param yPos The y position of the top-left part of the char.
 */
void vChar_fill(const uint32_t adwChar[4], uint16_t xPos, uint16_t yPos);

/**
 * @brief Used to display a single char.
 *
 * The g_byText_size global variable is used to determine the size of the font.
 *
 * @param cChar
 * @param xPos The x position of the top-left part of the char.
 * @param yPos The y position of the top-left part of the char.
 */
void vDisplay_char(char cChar, uint16_t xPos, uint16_t yPos);

/**
 * @brief Displays a string on the screen using the characters from the chars.h file.
 *
 * The g_byText_size global variable is used to determine the size of the font.
 *
 * @param ps_Str The string to be displayed.
 * @param byStr_len The length of the string NOT including the termination character.
 * @param xPos The x position of the top-left part of the string.
 * @param yPos The y position of the top-left part of the string.
 * @param byDelay_ms How many milliseconds to wait after clearing the are to then draw the text
 */
void vDisplay_string(const char *ps_Str, uint8_t byStr_len, uint16_t xPos, uint16_t yPos, uint8_t byDelay_ms = 0);

/**
 * @brief Used to determine the correct size and position of the text.
 *
 * This function is called before the vDisplay_string function to determine the maksimum value of the g_byText_size
 * variable and the currect x and y start position of the text so that it is centered.
 * The box determening the area where the text should be centered is determined by the initial value of the xStart
 * and yStart variables for the start of the box, and the xLimit and yLimit variables for the ends of the box.
 * The desired position is saved into the xStart and yStart variables and the font size stored in the global
 * g_byText_size variable.
 *
 * @param ps_Str The string you want to display. It is passed to the function so it can check for any special
 * characters that may take up more space like "Č".
 * @param byStr_len The length of the sting not including the termination char.
 * @param xStart Pointer to the position variable that will be used in the vDisplay_string. The value of the
 * variable when it is passed determines the lower limit of where the string should be placed. The value
 * is then changed so that the value if such that the string is centerd in the given area.
 * @param yStart Pointer to the position variable that will be used in the vDisplay_string. The value of the
 * variable when it is passed determines the lower limit of where the string should be placed. The value
 * is then changed so that the value if such that the string is centerd in the given area.
 * @param xLimit The maksimum x position on the display where the string can be displayed.
 * @param yLimit The maksimum y position on the display where the string can be displayed.
 * @param byMax_text_size The maximum text size value, used if you want to have an intentionally smaller font size.
 * Defaults to 255.
 */
void vSet_string_dims(const char *ps_Str, uint8_t byStr_len, uint16_t *xStart, uint16_t *yStart, uint16_t xLimit, uint16_t yLimit, uint8_t byMax_text_size = 255);

/**
 * @brief Used to display the desired date at the bottom of the screen.
 *
 * @param byDay
 * @param byMonth
 * @param wYear
 */
void vDisplay_date(uint8_t byDay, uint8_t byMonth, uint16_t wYear);

/**
 * @brief Used to display the main screen of the chalendar, without any birthdays.
 *
 * @param byDay
 * @param byMonth
 * @param wYear
 * @param byHour
 * @param byMinute
 * @param eUpdate_num
 */
void vMain_screen(uint8_t byDay, uint8_t byMonth, uint16_t wYear, uint8_t byHour, uint8_t byMinute, Update_digit eUpdate_num = ALL);

/**
 * @brief Used to display the birthday screen.
 *
 * The chalendar can display up to three birthdays that are spread evenly among the available space.
 * @param byDay
 * @param byMonth
 * @param wYear
 * @param byHour
 * @param byMinute
 * @param aBD_BDs An array of initialized BD class objects.
 * @param byBD_num The number of BD objects to be displayed that are stored in the aBD_BDs array.
 * @param eUpdate_num
 */
void vBD_screen(uint8_t byDay, uint8_t byMonth, uint16_t wYear, uint8_t byHour, uint8_t byMinute, BD *aBD_BDs, uint8_t byBD_num = 1, Update_digit eUpdate_num = ALL);

/**
 * @brief Used to display informative text stating that the device is waiting for GPS signal.
 *
 */
void vWaiting_for_GPS_screen();

/**
 * @brief Turns the display backlight off depending on the time of day.
 *
 * @param byHour
 */
void vDisplay_night_mode(uint8_t byHour);

/**
 * @brief Choses the correct screen and displays it based on the birthdays saved.
 *
 * @param eUpdate_num
 */
void vPick_screen(Update_digit eUpdate_num);

/**
 * @brief Displays a small text in the bottom left corner saying that the GPS is not connected
 * indicating that the systems is running of its own timer.
 *
 */
void vDisplay_no_GPS();

/**
 * @brief Used for turning on the display after it was turned on for a set amount of time.
 *
 */
void vDisplay_ON_IRQ();

/**
 * @brief Used to fadein and fade out the display.
 *
 * @param bDirection 1 means fade ON and 0 means fade OFF
 */
void vON_OFF_fade(uint8_t bDirection);

/**
 * @brief Display an example screen to show the new configuration.
 *
 * @param byDay
 * @param byMonth
 * @param wYear
 * @param byHour
 * @param byMinute
 */
void vDisplay_example_screen(uint8_t byDay, uint8_t byMonth, uint16_t wYear, uint8_t byHour, uint8_t byMinute);

#endif