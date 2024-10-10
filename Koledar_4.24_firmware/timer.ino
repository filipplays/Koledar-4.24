/**
 * @file timer.ino
 * @author Filip Zevnik
 * @brief
 * @version 4.24
 * @date 2024-07-26
 *
 *
 */

#include "timer.h"
#include "LCD.h"
#include "dates.h"

ISR(TIMER5_COMPA_vect)
{
    if (++g_byTimer_cnt > SEC_IN_MIN - 1)
    {
        g_byTimer_cnt = 0;
        g_byTimer_mins++;
    }
}

void vReset_timer()
{
    TCNT1 = 0x00;
    g_byTimer_cnt = 0;
    g_byTimer_mins = 0;
}

void vInit_timer()
{
    noInterrupts();
    TCCR5A = 0x00;
    TCCR5B = 0x00;
    TCNT5 = 0x00;

    OCR5A = 0x3D08;

    TCCR5B |= (1 << WGM52);
    TCCR5B |= (1 << CS50);
    TCCR5B |= (1 << CS52);

    TIFR5 = 0x00;
    TIMSK5 |= (1 << OCIE5A);

    interrupts();
}