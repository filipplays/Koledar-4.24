/**
 * @file Koledar_4.24_firmware.ino
 * @author Filip Zevnik
 * @brief The main firmware code file
 * @version 4.24
 * @date 2024-07-26
 *
 * This is the main firmware file for the Koledar 4.24 project, the includes the
 * setup and loop code.
 */

#include <UTFT.h>
#include <TinyGPS++.h>

#include "BD.h"
#include "LCD.h"
#include "chars.h"
#include "dates.h"
#include "timer.h"
#include "EEPROM_utils.h"
#include "communication.h"

/** \addtogroup display_dims Display dimensions
 * @brief Display dimensions
 * @{
 */
#define SCREEN_W 800
#define SCREEN_H 480
/** @} */

/** \addtogroup display_defines Display pin defines
 * @brief The defines for the pins used to communicate with the screen.
 * @{
 */
#define RS 53
#define WR 52
#define CS 51
#define RST 50
#define BR_PIN 5
#define IRQ_PIN 19
/** @} */

#define TEST_MODE 0
#define SEC_IN_MIN 60
#define ENABLE_NIGHT_MODE 1
uint8_t bUse_timer = 0;

extern uint8_t SevenSegNumFont[];

/** \addtogroup dnt_vars Date and time variables
 * @brief The variables used for the current date and time.
 * The p infront of some variables denotes that the variable is storing the previous date or time values
 * and is used to dictate when to update the display.
 * @{
 */
uint8_t g_byDay = 0;
uint8_t g_byMonth = 0;
uint16_t g_wYear = 0;
uint8_t g_byHour = 0;
uint8_t g_byMinute = 0;
uint8_t g_byP_day = 0;
uint8_t g_byP_month = 0;
uint16_t g_wP_year = 0;
uint8_t g_byP_hour = 0;
uint8_t g_byP_minute = 0;
/** @} */

/** \addtogroup msc_vars Misc variables
 * @brief Miscellaneous variables.
 * @{
 */
uint8_t g_byText_size = 15;
uint8_t g_byBrightness = 255;
volatile uint8_t g_byTimer_cnt = 0;
volatile uint8_t g_byTimer_mins = 0;
uint8_t g_bySaved_BDs = 0;
uint8_t g_bGPS_state = 0;
int32_t g_liDisplay_ON_millis = -1;
uint8_t g_bSystem_sleep = 0;
uint8_t g_bConnected_to_GUI = 0;
/** @} */

/** \addtogroup modules Peripheral modules.
 * @brief Objects used for interacting with the GPS and LCD modules.
 *  @{
 */
UTFT myGLCD(SSD1963_800, RS, WR, CS, RST);
TinyGPSPlus gps;
/** @} */

/** \addtogroup color_vars Background and text color variables
 * @brief The variables dictating the color of the background and text.
 * @{
 */
uint8_t g_byBG_red = 100;
uint8_t g_byBG_green = 150;
uint8_t g_byBG_blue = 200;
uint8_t g_byText_red = 0;
uint8_t g_byText_green = 0;
uint8_t g_byText_blue = 0;
/** @} */

/**
 * @brief The Arduino setup function.
 * Here all of the needed values and peripherals are initialized and the "waiting for signal" screen
 * is displayed.
 */
void setup()
{
    Serial.begin(9600);
    Serial3.begin(9600);
    while (!Serial || !Serial3)
        ;

    // Load the saved style values
    vInit_bytes();

    myGLCD.InitLCD();
    myGLCD.setFont(SevenSegNumFont);

    pinMode(BR_PIN, OUTPUT);
    analogWrite(BR_PIN, g_byBrightness);

    // Display the initial screen
    myGLCD.fillScr(g_byBG_red, g_byBG_green, g_byBG_blue);

    /*
        g_byText_size = 5;
        vDisplay_string("0123456789", 10, 10, 50);
        vDisplay_string("abc\1defghi", 10, 10, 130);
        vDisplay_string("jklmnopqrs", 10, 10, 210);
        vDisplay_string("\2tuvwxyz\3", 9, 10, 290);
        while (1)
            ;
    */

    vWaiting_for_GPS_screen();

    // Setup for interrupts
    pinMode(IRQ_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IRQ_PIN), vDisplay_ON_IRQ, LOW);

    vInit_timer();

#if TEST_MODE == 0
    uint8_t byGPS_data;
    do
    {
        vHandle_serial();
        while (Serial3.available() > 0)
        {
            byGPS_data = Serial3.read();
            if (g_bConnected_to_GUI == 0)
                gps.encode(byGPS_data);
        }
    } while (!gps.date.isValid() || gps.date.year() < 2023);
#elif TEST_MODE == 1
    g_byDay = 20;
    g_byMonth = 11;
    g_wYear = 2024;
    g_byHour = 11;
    g_byMinute = 30;
    g_byP_day = g_byDay;
    g_byP_month = g_byMonth;
    g_wP_year = g_wYear;
    g_byP_hour = g_byHour;
    g_byP_minute = g_byMinute;
    g_byTimer_mins = 0;
    vPick_screen(ALL);
#endif
}

/**
 * @brief The Arduino loop function.
 * It is the main loop of the program.
 * The display is updated based on the GPS or timer data, and the system listens for any data being written to its
 * USB serial port.
 */
void loop()
{
    while (1)
    {
        vHandle_serial();
        // Take care of turning the screen back off at night after IRQ
        if ((g_bSystem_sleep == 1) && (g_liDisplay_ON_millis != -1))
            if (g_liDisplay_ON_millis < millis())
            {
                g_liDisplay_ON_millis = -1;
                vON_OFF_fade(0);
            }

        // Update display from GPS settings
        while (Serial3.available() && !TEST_MODE && g_bConnected_to_GUI == 0)
            gps.encode(Serial3.read());
        if (gps.date.isValid() && gps.date.year() > 2023 && bUse_timer == 0)
        {
            vReset_timer();

            g_byTimer_cnt = gps.time.second();
            g_byMinute = gps.time.minute();
            g_byHour = gps.time.hour() + 1;

            g_byDay = gps.date.day();
            g_byMonth = gps.date.month();
            g_wYear = gps.date.year();

            if (bSummer_time(g_wYear, g_byMonth, g_byDay) == 1)
                g_byHour++;

            if (g_byHour > 23)
            {
                g_byHour %= 24;
                vAdd_day();
            }

            if ((g_byMinute > g_byP_minute) || (g_byHour > g_byP_hour) || (g_byDay > g_byP_day) || (g_byMonth > g_byP_month) || (g_wYear > g_wP_year))
            {
                Update_digit eUpdate_num;

                eUpdate_num = (Update_digit)(byUpdate_time(&g_byMinute, &g_byP_minute));
                eUpdate_num = (Update_digit)((int)eUpdate_num + byUpdate_time(&g_byHour, &g_byP_hour));
                if (byUpdate_time(&g_byDay, &g_byP_day) > 0)
                    eUpdate_num = ALL;
                g_byP_month = g_byMonth;
                g_wP_year = g_wYear;
                vDisplay_night_mode(g_byHour);
                if (g_bGPS_state == 0)
                {
                    g_bGPS_state = 1;
                    vPick_screen(ALL);
                }
                else
                {
                    vPick_screen(eUpdate_num);
                }
            }
        }
        else // Case when the GPS has no signal
        {
            if (g_byTimer_mins > 0 && g_bConnected_to_GUI == 0)
            {
                Update_digit eUpdate_num;
                eUpdate_num = eAdd_minutes(g_byTimer_mins);
                g_byTimer_mins = 0;
                vDisplay_night_mode(g_byHour);
                if (g_bGPS_state == 1)
                {
                    g_bGPS_state = 0;
                    vPick_screen(ALL);
                }
                else
                {
                    vPick_screen(eUpdate_num);
                }
            }
        }
#if TEST_MODE == 1
        if (g_byTimer_mins > 0)
        {
            Update_digit eUpdate_num;
            g_bGPS_state = 0;
            eUpdate_num = eAdd_minutes(g_byTimer_mins);
            g_byTimer_mins = 0;
            vDisplay_night_mode(g_byHour);
            vPick_screen(eUpdate_num);
        }
#endif
        /*
        if (Serial.available() > 0)
        {
            char cInput = Serial.read();
            if (cInput == '0')
            {
                bUse_timer = 0;
            }
            else if (cInput == '1')
            {
                bUse_timer = 1;
            }
        }
        */
    }
}
