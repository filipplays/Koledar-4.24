#include "communication.h"
#include "LCD.h"
#include "EEPROM_utils.h"
#include "eeprom.h"

void vHandle_serial()
{

    static char sTmp[TMP_STR_LEN];
    static uint8_t bcTmp = 0;
    static uint8_t bycTmp_BD_cnt = 0;
    static uint8_t byDay, byMonth, byYear;
    static uint8_t byTmp_brightness;
    static Serial_state eState = WAIT;
    static Serial_substate eSubstate = SUB_AMOUNT;
    static uint8_t bOption_save_profile = 0;
    static uint8_t bOption_clear_BDs = 0;
    while (Serial.available() > 0)
    {
        bool bClear = false;
        sTmp[bcTmp] = Serial.read();

        if (sTmp[bcTmp] == '_')
        {
            bClear = true;
        }

        switch (eState)
        {
        case WAIT: // waiting for input
        {
            if (strcmp(sTmp, "test") == 0)
            {
                bClear = true;
                Serial.println("test");
                g_bConnected_to_GUI = 1;
                g_bGPS_state = 1;
                eState = WAIT;
            }
            else if (strcmp(sTmp, "reset") == 0)
            {
                bOption_clear_BDs = 1;
                bOption_save_profile = 1;
                eState = SHOW_PROFILE;
                eSubstate = SUB_COLOR;
                bClear = true;
            }
            else if (strcmp(sTmp, "writebd") == 0)
            {
                bClear = true;
                eState = WRITE_BD;
                eSubstate = SUB_AMOUNT;
            }
            else if (strcmp(sTmp, "read") == 0)
            {
                bClear = true;

                Serial.println(g_bySaved_BDs);

                char sTmp_out_string[60];
                BD BD_Tmp;
                for (uint8_t i = 0; i < g_bySaved_BDs; i++)
                {
                    READ_BD(BD_Tmp, i);
                    BD_Tmp.vFormat_alternative(sTmp_out_string);
                    Serial.println(sTmp_out_string);
                }
            }
            else if (strcmp(sTmp, "showprofile") == 0)
            {

                bClear = true;
                bOption_save_profile = 0;
                bOption_clear_BDs = 0;
                eState = SHOW_PROFILE;
                eSubstate = SUB_COLOR;
            }
            else if (strcmp(sTmp, "writeprofile") == 0)
            {
                bClear = true;
                bOption_save_profile = 1;
                bOption_clear_BDs = 0;
                eState = SHOW_PROFILE;
                eSubstate = SUB_COLOR;
            }
            break;
        }
        case WRITE_BD:
        {
            if (sTmp[bcTmp] == '.')
            {

                switch (eSubstate)
                {
                case SUB_AMOUNT:
                {
                    sTmp[bcTmp] = '\0';

                    g_bySaved_BDs = (uint8_t)atoi(sTmp);

                    if (g_bySaved_BDs == 0)
                    {
                        eState = WAIT;
                        bClear = true;
                        return;
                    }

                    bycTmp_BD_cnt = g_bySaved_BDs;

                    vSave_byte(g_bySaved_BDs, 0);

                    eSubstate = SUB_NAME_SURNAME;
                    bClear = true;
                    break;
                }
                case SUB_NAME_SURNAME:
                {
                    strncpy(sTmp_Name_surname, sTmp, 40);
                    sTmp_Name_surname[40] = '\0';
                    const char cKey[2] = "-";
                    char *pcTmp_pointer = strpbrk(sTmp_Name_surname, cKey);
                    while (pcTmp_pointer != NULL)
                    {
                        *pcTmp_pointer = '.';
                        pcTmp_pointer = strpbrk(sTmp_Name_surname, cKey);
                    }
                    eSubstate = SUB_DATE;
                    bClear = true;
                    break;
                }
                case SUB_DATE:
                {
                    char caTmp[5];
                    strncpy(caTmp, sTmp, 2);
                    caTmp[2] = '\0';
                    byDay = atoi(caTmp);

                    strncpy(caTmp, &sTmp[2], 2);
                    caTmp[2] = '\0';
                    byMonth = atoi(caTmp);

                    strncpy(caTmp, &sTmp[4], 3);
                    caTmp[3] = '\0';
                    byYear = atoi(caTmp);

                    BD BD_Tmp(sTmp_Name_surname, byDay, byMonth, byYear);
                    BD_Tmp.vSave_BD(g_bySaved_BDs - bycTmp_BD_cnt);

                    bycTmp_BD_cnt--;
                    if (bycTmp_BD_cnt == 0)
                    {
                        Serial.println("SUCCESS");
                        Serial.println("Rojstni dnevi so bili uspesno vpisani.");
                        eState = WAIT;
                        BD aBD_Tmp_BD_array[1];
                        aBD_Tmp_BD_array[0] = BD_Tmp;
                        g_wYear = 2045;
                        vON_OFF_fade(0);
                        vBD_screen(byDay, byMonth, 2045, 16, 30, aBD_Tmp_BD_array);
                        vON_OFF_fade(1);
                    }
                    else
                    {
                        eSubstate = SUB_NAME_SURNAME;
                    }

                    bClear = true;
                    break;
                }
                }
            }
        }
        break;

        case SHOW_PROFILE:
        {
            if (sTmp[bcTmp] == '.')
            {
                switch (eSubstate)
                {
                case SUB_COLOR:
                {
                    char sTmp_color[5];
                    for (uint8_t i = 0; i < 3; i++)
                    {
                        strncpy(sTmp_color, &sTmp[i * 3], 3);
                        sTmp_color[3] = '\0';
                        byBG_color[i] = (uint8_t)atoi(sTmp_color);
                    }
                    for (uint8_t i = 3; i < 6; i++)
                    {
                        strncpy(sTmp_color, &sTmp[i * 3], 3);
                        sTmp_color[3] = '\0';
                        byText_color[i - 3] = (uint8_t)atoi(sTmp_color);
                    }

                    bClear = true;
                    eSubstate = SUB_BRIGHTNESS;
                    break;
                }
                case SUB_BRIGHTNESS:
                {
                    char sTmp_brightness[5];
                    strncpy(sTmp_brightness, sTmp, 3);
                    sTmp_brightness[3] = '\0';
                    byTmp_brightness = atoi(sTmp_brightness);
                    bClear = true;
                    eState = WAIT;
                    eSubstate = SUB_AMOUNT;

                    g_byBG_red = byBG_color[0];
                    g_byBG_green = byBG_color[1];
                    g_byBG_blue = byBG_color[2];

                    g_byText_red = byText_color[0];
                    g_byText_green = byText_color[1];
                    g_byText_blue = byText_color[2];

                    g_byBrightness = byTmp_brightness;

                    Serial.println("SUCCESS");

                    if (bOption_save_profile == 0 && bOption_clear_BDs == 0)
                        Serial.println("Profil je prikazan.");

                    if (bOption_save_profile == 1)
                    {
                        vSave_byte(g_byBG_red, 1);
                        vSave_byte(g_byBG_green, 2);
                        vSave_byte(g_byBG_blue, 3);

                        vSave_byte(g_byText_red, 4);
                        vSave_byte(g_byText_green, 5);
                        vSave_byte(g_byText_blue, 6);

                        vSave_byte(g_byBrightness, 7);
                        if (bOption_clear_BDs == 0)
                            Serial.println("Profil je shranjen.");
                        else
                            Serial.println("Sistem je očiščen.");
                    }

                    if (bOption_clear_BDs == 1)
                    {
                        vSave_byte(0, 0);
                        g_bySaved_BDs = 0;
                    }

                    vDisplay_example_screen(6, 7, 2025, 16, 30);

                    break;
                }
                }
            }
            break;
        }
        }
        // END OF SWITCH
        if (bClear)
        {
            memset(sTmp, '\0', TMP_STR_LEN);
            bcTmp = 0;
        }
        else
            bcTmp++;

        if (bcTmp > TMP_STR_LEN - 1)
        {
            memset(sTmp, '\0', TMP_STR_LEN);
            bcTmp = 0;
            Serial.println("Temporary buffer overflow error!");
        }
    }
}