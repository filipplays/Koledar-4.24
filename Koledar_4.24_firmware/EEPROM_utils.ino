#include "EEPROM.h"

#include "EEPROM_utils.h"

void vSave_byte(uint8_t bySave, uint8_t byIndex)
{
    if (byIndex < 8)
        EEPROM.update(byIndex, bySave);
}

uint8_t vRead_byte(uint8_t byIndex)
{
    if (byIndex < 8)
        return EEPROM.read(byIndex);
    return 0x69;
}

void vInit_bytes()
{
    g_bySaved_BDs = vRead_byte(0);

    g_byBG_red = vRead_byte(1);
    g_byBG_green = vRead_byte(2);
    g_byBG_blue = vRead_byte(3);

    g_byText_red = vRead_byte(4);
    g_byText_green = vRead_byte(5);
    g_byText_blue = vRead_byte(6);

    g_byBrightness = vRead_byte(7);
}