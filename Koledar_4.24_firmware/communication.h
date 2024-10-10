/**
 * @file communication.h
 * @author Filip Zevnik
 * @brief Used for communication with the KOLEDAR GUI 2.24 software.
 * @version 4.24
 * @date 2024-08-02
 *
 *
 */

#ifndef communication_h
#define communication_h

#define TMP_STR_LEN 50

enum Serial_state
{
    WAIT,
    WRITE_BD,
    TEST,
    SHOW_PROFILE
};
enum Serial_substate
{
    SUB_AMOUNT,
    SUB_NAME_SURNAME,
    SUB_DATE,
    SUB_COLOR,
    SUB_BRIGHTNESS
};

char sTmp_Name_surname[41];
uint8_t byBG_color[3];
uint8_t byText_color[3];

/**
 * @brief Handles all of the serial communication with the Koledar GUI desktop app
 *
 */
void vHandle_serial();

#endif