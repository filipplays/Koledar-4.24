#include "LCD.h"
#include "chars.h"
#include "BD.h"
#include "dates.h"
#include "EEPROM_utils.h"
#include "EEPROM.h"

void vChar_stroke(const uint32_t adwChar[4], uint16_t xPos, uint16_t yPos)
{
  myGLCD.setColor(g_byText_red, g_byText_green, g_byText_blue);
  uint8_t byIndexX;
  uint8_t byIndexY;
  uint8_t byIndexT;
  for (uint8_t i = 0; i < CHAR_COLS * CHAR_ROWS; i++)
  {
    byIndexX = i % CHAR_COLS;
    byIndexY = (i - byIndexX) / CHAR_COLS;
    byIndexT = i / 30;
    if ((adwChar[byIndexT] & ((uint32_t)1 << (i % 30))) > 0)
      myGLCD.drawRect(xPos + byIndexX * g_byText_size, yPos + byIndexY * g_byText_size, xPos + byIndexX * g_byText_size + g_byText_size, yPos + byIndexY * g_byText_size + g_byText_size);
  }
}

void vChar_fill(const uint32_t adwChar[4], uint16_t xPos, uint16_t yPos)
{
  myGLCD.setColor(g_byText_red, g_byText_green, g_byText_blue);
  uint8_t byIndexX;
  uint8_t byIndexY;
  uint8_t byIndexT;
  for (uint8_t i = 0; i < CHAR_COLS * CHAR_ROWS; i++)
  {
    byIndexX = i % CHAR_COLS;
    byIndexY = (i - byIndexX) / CHAR_COLS;
    byIndexT = i / 30;
    if ((adwChar[byIndexT] & ((uint32_t)1 << (i % 30))) > 0)
      myGLCD.fillRect(xPos + byIndexX * g_byText_size, yPos + byIndexY * g_byText_size, xPos + byIndexX * g_byText_size + g_byText_size, yPos + byIndexY * g_byText_size + g_byText_size);
  }
}

void vDisplay_string(const char *ps_Str, uint8_t byStr_len, uint16_t xPos, uint16_t yPos, uint8_t byDelay_ms = 0)
{
  myGLCD.setColor(g_byBG_red, g_byBG_green, g_byBG_blue);
  myGLCD.fillRect(xPos, yPos, xPos + byStr_len * (CHAR_COLS + SPACEING) * g_byText_size, yPos + CHAR_ROWS * g_byText_size);

  delay(byDelay_ms);

  uint16_t xCurrPos = xPos;
  for (int i = 0; i < byStr_len; i++)
  {
    vDisplay_char(ps_Str[i], xCurrPos, yPos);
    xCurrPos += (CHAR_COLS + SPACEING) * g_byText_size;
  }
}

void vSet_string_dims(const char *ps_Str, uint8_t byStr_len, uint16_t *xStart, uint16_t *yStart, uint16_t xLimit, uint16_t yLimit, uint8_t byMax_text_size = 255)
{

  g_byText_size = xLimit / (byStr_len * (CHAR_COLS + SPACEING));
  if (g_byText_size * CHAR_ROWS > yLimit)
    g_byText_size = yLimit / (CHAR_ROWS);
  if (g_byText_size > byMax_text_size)
    g_byText_size = byMax_text_size;
  *yStart = *yStart + (yLimit - ((CHAR_ROWS)*g_byText_size)) / 2;
  *xStart = *xStart + (xLimit - (byStr_len * (CHAR_COLS + SPACEING) * g_byText_size - g_byText_size)) / 2;
}

void vDisplay_date(uint8_t byDay, uint8_t byMonth, uint16_t wYear)
{
  char sDate_str[14];
  uint8_t byStr_len = 8;

  if (byDay > 9)
    byStr_len++;
  if (byMonth > 9)
    byStr_len++;

  sprintf(sDate_str, "%d.%d.%d", byDay, byMonth, wYear);

  uint16_t xPos;
  uint16_t yPos = 405;

  if (g_bGPS_state == 0)
  {
    vDisplay_no_GPS();
    xPos = 150;
    vSet_string_dims(sDate_str, byStr_len, &xPos, &yPos, SCREEN_W - 150, 70);
  }
  else
  {
    xPos = 0;
    vSet_string_dims(sDate_str, byStr_len, &xPos, &yPos, SCREEN_W, 70);
  }

  vDisplay_string(sDate_str, byStr_len, xPos, yPos);
}

void vMain_screen(uint8_t byDay, uint8_t byMonth, uint16_t wYear, uint8_t byHour, uint8_t byMinute, Update_digit eUpdate_num = ALL)
{
  uint16_t xPos;
  uint16_t yPos;
  if (eUpdate_num == ALL)
  {
    myGLCD.setColor(g_byBG_red, g_byBG_green, g_byBG_blue);
    myGLCD.fillRect(0, 0, SCREEN_W, SCREEN_H);

    xPos = 0;
    yPos = 5;
    uint8_t byWeek_day = byDay_of_week(byMonth, byDay, wYear);

    vSet_string_dims(c_acWeek_days[byWeek_day], c_byWeek_days_len[byWeek_day], &xPos, &yPos, SCREEN_W, 200);

    vDisplay_string(c_acWeek_days[byWeek_day], c_byWeek_days_len[byWeek_day], xPos, yPos);

    xPos = 0;
    yPos += CHAR_ROWS * g_byText_size;

    char sTime_str[6];
    sprintf(sTime_str, "%02d:%02d", byHour, byMinute);

    uint8_t byTime_str_len = 5;

    vSet_string_dims(sTime_str, byTime_str_len, &xPos, &yPos, SCREEN_W, 400 - yPos, 20);

    vDisplay_string(sTime_str, byTime_str_len, xPos, yPos);

    vDisplay_date(byDay, byMonth, wYear);
  }
  else
  {
    xPos = 0;
    yPos = 5;
    uint8_t byWeek_day = byDay_of_week(byMonth, byDay, wYear);

    vSet_string_dims(c_acWeek_days[byWeek_day], c_byWeek_days_len[byWeek_day], &xPos, &yPos, SCREEN_W, 200);

    xPos = 0;
    yPos += CHAR_ROWS * g_byText_size;

    char sTime_str[8];
    sprintf(sTime_str, "%02d:%02d", byHour, byMinute);

    uint8_t byTime_str_len = 5;

    vSet_string_dims(sTime_str, byTime_str_len, &xPos, &yPos, SCREEN_W, 400 - yPos, 20);

    myGLCD.setColor(g_byBG_red, g_byBG_green, g_byBG_blue);

    if (eUpdate_num <= TWO_MINUTE)
      vDisplay_string(&sTime_str[byTime_str_len - 1 - (uint8_t)eUpdate_num], (uint8_t)eUpdate_num + 1, xPos + (4 - (uint8_t)eUpdate_num) * (CHAR_COLS + SPACEING) * g_byText_size, yPos, 150);
    else
      vDisplay_string(&sTime_str[byTime_str_len - 1 - (uint8_t)eUpdate_num - 1], (uint8_t)eUpdate_num + 1 + 1, xPos + (4 - (uint8_t)eUpdate_num - 1) * (CHAR_COLS + SPACEING) * g_byText_size, yPos, 150);
  }
}

void vBD_screen(uint8_t byDay, uint8_t byMonth, uint16_t wYear, uint8_t byHour, uint8_t byMinute, BD *aBD_BDs, uint8_t byBD_num = 1, Update_digit eUpdate_num = ALL)
{
  uint16_t xPos = 5;
  uint16_t yPos = 5;
  uint8_t byWeek_day = byDay_of_week(byMonth, byDay, wYear);

  char sTop_text[16];
  sprintf(sTop_text, "%s %02d:%02d", c_acWeek_days[byWeek_day], byHour, byMinute);

  uint8_t byTop_text_len = 5;
  byTop_text_len += c_byWeek_days_len[byWeek_day] + 1;

  vSet_string_dims(sTop_text, byTop_text_len, &xPos, &yPos, SCREEN_W, 120);

  if (eUpdate_num == ALL)
  {
    myGLCD.setColor(g_byBG_red, g_byBG_green, g_byBG_blue);
    myGLCD.fillRect(0, 0, SCREEN_W, SCREEN_H);

    vDisplay_string(sTop_text, byTop_text_len, xPos, yPos);

    if (byBD_num != 0)
    {
      uint16_t wBD_start = yPos + CHAR_ROWS * g_byText_size;
      uint16_t wBD_space = (400 - wBD_start) / byBD_num;
      uint8_t byBD_str_len;
      char sBD[60];

      uint16_t awData[3][3];
      uint16_t wSize_sum = 0;

      for (uint8_t i = 0; i < byBD_num; i++)
      {
        xPos = 0;
        yPos = wBD_start + wBD_space * i + 5;
        aBD_BDs[i].vFormat(sBD);
        for (byBD_str_len = 0; byBD_str_len < 60; byBD_str_len++)
        {
          if (sBD[byBD_str_len] == '\0')
            break;
        }
        vSet_string_dims(sBD, byBD_str_len, &xPos, &yPos, SCREEN_W, wBD_space);
        awData[i][0] = xPos;
        awData[i][1] = g_byText_size;
        awData[i][2] = byBD_str_len;
        wSize_sum += g_byText_size * CHAR_ROWS + 40;
      }

      yPos = wBD_start + (400 - wBD_start - wSize_sum) / 2;
      for (int i = 0; i < byBD_num; i++)
      {
        yPos += 20;
        aBD_BDs[i].vFormat(sBD);
        g_byText_size = awData[i][1];
        vDisplay_string(sBD, awData[i][2], awData[i][0], yPos);
        yPos += CHAR_ROWS * g_byText_size + 20;
      }
    }

    vDisplay_date(byDay, byMonth, wYear);
  }
  else
  {

    for (uint8_t i = 0; i < byTop_text_len - 5; i++)
      if (sTop_text[i] == 'k')
      {
        xPos -= g_byText_size;
      }

    myGLCD.setColor(g_byBG_red, g_byBG_green, g_byBG_blue);

    xPos += (CHAR_COLS + SPACEING) * g_byText_size * (byTop_text_len - 5);

    if (eUpdate_num <= TWO_MINUTE)
      vDisplay_string(&sTop_text[byTop_text_len - 1 - (uint8_t)eUpdate_num], (uint8_t)eUpdate_num + 1, xPos + (4 - (uint8_t)eUpdate_num) * (CHAR_COLS + SPACEING) * g_byText_size, yPos, 150);
    else
      vDisplay_string(&sTop_text[byTop_text_len - 1 - (uint8_t)eUpdate_num - 1], (uint8_t)eUpdate_num + 1 + 1, xPos + (4 - (uint8_t)eUpdate_num - 1) * (CHAR_COLS + SPACEING) * g_byText_size, yPos, 150);
  }
}

void vDisplay_char(char cChar, uint16_t xPos, uint16_t yPos)
{
  switch (cChar)
  {
  case '0':
    vChar_fill(zero, xPos, yPos);
    vChar_stroke(zero, xPos, yPos);
    break;
  case '1':

    vChar_fill(one, xPos, yPos);
    vChar_stroke(one, xPos, yPos);
    break;
  case '2':

    vChar_fill(two, xPos, yPos);
    vChar_stroke(two, xPos, yPos);
    break;
  case '3':

    vChar_fill(three, xPos, yPos);
    vChar_stroke(three, xPos, yPos);
    break;
  case '4':

    vChar_fill(four, xPos, yPos);
    vChar_stroke(four, xPos, yPos);
    break;
  case '5':

    vChar_fill(five, xPos, yPos);
    vChar_stroke(five, xPos, yPos);
    break;
  case '6':

    vChar_fill(six, xPos, yPos);
    vChar_stroke(six, xPos, yPos);
    break;
  case '7':

    vChar_fill(seven, xPos, yPos);
    vChar_stroke(seven, xPos, yPos);
    break;
  case '8':

    vChar_fill(eight, xPos, yPos);
    vChar_stroke(eight, xPos, yPos);
    break;
  case '9':

    vChar_fill(nine, xPos, yPos);
    vChar_stroke(nine, xPos, yPos);
    break;

  //---------------------
  case 'a':

    vChar_fill(a, xPos, yPos);
    vChar_stroke(a, xPos, yPos);
    break;
  case 'b':
    vChar_fill(b, xPos, yPos);
    vChar_stroke(b, xPos, yPos);
    break;
  case 'c':
    vChar_fill(c, xPos, yPos);
    vChar_stroke(c, xPos, yPos);
    break;
  case '\1':
    vChar_fill(ch, xPos, yPos);
    vChar_stroke(ch, xPos, yPos);
    break;
  case 'd':
    vChar_fill(d, xPos, yPos);
    vChar_stroke(d, xPos, yPos);
    break;
  case 'e':
    vChar_fill(e, xPos, yPos);
    vChar_stroke(e, xPos, yPos);
    break;
  case 'f':
    vChar_fill(f, xPos, yPos);
    vChar_stroke(f, xPos, yPos);
    break;
  case 'g':
    vChar_fill(g, xPos, yPos);
    vChar_stroke(g, xPos, yPos);
    break;
  case 'h':
    vChar_fill(h, xPos, yPos);
    vChar_stroke(h, xPos, yPos);
    break;
  case 'i':
    vChar_fill(_i, xPos, yPos);
    vChar_stroke(_i, xPos, yPos);
    break;
  case 'j':
    vChar_fill(_j, xPos, yPos);
    vChar_stroke(_j, xPos, yPos);
    break;
  case 'k':
    vChar_fill(k, xPos, yPos);
    vChar_stroke(k, xPos, yPos);
    break;
  case 'l':
    vChar_fill(l, xPos, yPos);
    vChar_stroke(l, xPos, yPos);
    break;
  case 'm':
    vChar_fill(m, xPos, yPos);
    vChar_stroke(m, xPos, yPos);
    break;
  case 'n':
    vChar_fill(n, xPos, yPos);
    vChar_stroke(n, xPos, yPos);
    break;
  case 'o':
    vChar_fill(o, xPos, yPos);
    vChar_stroke(o, xPos, yPos);
    break;
  case 'p':
    vChar_fill(p, xPos, yPos);
    vChar_stroke(p, xPos, yPos);
    break;
  case 'q':
    vChar_fill(q, xPos, yPos);
    vChar_stroke(q, xPos, yPos);
    break;
  case 'r':
    vChar_fill(r, xPos, yPos);
    vChar_stroke(r, xPos, yPos);
    break;
  case 's':
    vChar_fill(s, xPos, yPos);
    vChar_stroke(s, xPos, yPos);
    break;
  case '\2':
    vChar_fill(sh, xPos, yPos);
    vChar_stroke(sh, xPos, yPos);
    break;
  case 't':
    vChar_fill(t, xPos, yPos);
    vChar_stroke(t, xPos, yPos);
    break;
  case 'u':
    vChar_fill(u, xPos, yPos);
    vChar_stroke(u, xPos, yPos);
    break;
  case 'v':
    vChar_fill(v, xPos, yPos);
    vChar_stroke(v, xPos, yPos);
    break;
  case 'w':
    vChar_fill(w, xPos, yPos);
    vChar_stroke(w, xPos, yPos);
    break;
  case 'x':
    vChar_fill(x, xPos, yPos);
    vChar_stroke(x, xPos, yPos);
    break;
  case 'y':
    vChar_fill(y, xPos, yPos);
    vChar_stroke(y, xPos, yPos);
    break;
  case 'z':
    vChar_fill(z, xPos, yPos);
    vChar_stroke(z, xPos, yPos);
    break;
  case '\3':
    vChar_fill(zh, xPos, yPos);
    vChar_stroke(zh, xPos, yPos);
    break;
  case '-':
    vChar_fill(dash, xPos, yPos);
    vChar_stroke(dash, xPos, yPos);
    break;
  case ' ':
    vChar_fill(empty, xPos, yPos);
    vChar_stroke(empty, xPos, yPos);
    break;
  case '.':
    vChar_fill(stop, xPos, yPos);
    vChar_stroke(stop, xPos, yPos);
    break;
  case ':':
    vChar_fill(colon, xPos, yPos);
    vChar_stroke(colon, xPos, yPos);
    break;
  default:
    uint32_t err[4] = {0x3fffffff, 0x3fffffff, 0x3fffffff, 0x3fffffff};
    vChar_fill(err, xPos, yPos);
    vChar_stroke(err, xPos, yPos);
    break;
  }
}

void vWaiting_for_GPS_screen()
{
  uint16_t xPos = 0;
  uint16_t yPos = 0;
  const char sCakam_GPS[24] = "\1akam na gps signal ...";
  vSet_string_dims(sCakam_GPS, 23, &xPos, &yPos, SCREEN_W, SCREEN_H);
  vDisplay_string(sCakam_GPS, 23, xPos, yPos);
}

void vDisplay_night_mode(uint8_t byHour)
{
  if (ENABLE_NIGHT_MODE == 0)
  {
    g_bSystem_sleep = 0;
    return;
  }
  if ((byHour >= DISPLAY_OFF_HOUR) || (byHour < DISPLAY_ON_HOUR))
  {
    if (g_bSystem_sleep == 0)
      vON_OFF_fade(0);
    g_bSystem_sleep = 1;
  }
  else
  {
    if (g_bSystem_sleep == 1)
      vON_OFF_fade(1);
    g_bSystem_sleep = 0;
  }
}

void vPick_screen(Update_digit eUpdate_num)
{
  if (eUpdate_num == ALL && g_bSystem_sleep == 0)
    vON_OFF_fade(0);

  BD aBD_Temp_arr[145];
  BD BD_Temp;
  for (uint8_t i = 0; i < g_bySaved_BDs; i++)
  {
    READ_BD(aBD_Temp_arr[i], i);
  }

  if (g_bySaved_BDs > 0)
  {
    uint8_t bSwapped;
    uint8_t byValid_BDs = 0;
    for (uint8_t i = 0; i < g_bySaved_BDs - 1; i++)
    {
      bSwapped = 0;
      for (uint8_t j = 0; j < g_bySaved_BDs - i - 1; j++)
      {
        uint16_t wDays_till_BD_1 = aBD_Temp_arr[j].wDays_till_BD(g_byDay, g_byMonth, g_wYear);
        uint16_t wDays_till_BD_2 = aBD_Temp_arr[j + 1].wDays_till_BD(g_byDay, g_byMonth, g_wYear);

        if (wDays_till_BD_1 > wDays_till_BD_2)
        {
          BD_Temp = aBD_Temp_arr[j];
          aBD_Temp_arr[j] = aBD_Temp_arr[j + 1];
          aBD_Temp_arr[j + 1] = BD_Temp;
          bSwapped = 1;
        }
      }
      if (bSwapped == 0)
        break;
    }

    for (int16_t i = 0; (i < g_bySaved_BDs) && (i < 3); i++)
    {
      if (aBD_Temp_arr[i].wDays_till_BD(g_byDay, g_byMonth, g_wYear) <= MAX_DAYS_TILL_BD)
        byValid_BDs++;
    }

    if (byValid_BDs == 0)
    {
      vMain_screen(g_byDay, g_byMonth, g_wYear, g_byHour, g_byMinute, eUpdate_num);
    }
    else
    {
      if (byValid_BDs > 3)
        byValid_BDs = 3;

      vBD_screen(g_byDay, g_byMonth, g_wYear, g_byHour, g_byMinute, aBD_Temp_arr, byValid_BDs, eUpdate_num);
    }
  }
  else
    vMain_screen(g_byDay, g_byMonth, g_wYear, g_byHour, g_byMinute, eUpdate_num);

  if (eUpdate_num == ALL && g_bSystem_sleep == 0)
    vON_OFF_fade(1);
}

void vDisplay_no_GPS()
{
  const char sNo_GPS[9] = "brez gps";
  uint16_t xPos = 0;
  uint16_t yPos = 440;
  vSet_string_dims(sNo_GPS, 8, &xPos, &yPos, 120, 20, 1);
  myGLCD.setColor(g_byText_red, g_byText_green, g_byText_blue);
  vDisplay_string(sNo_GPS, 8, xPos, yPos);
}

void vDisplay_ON_IRQ()
{
  if ((g_bSystem_sleep == 1) && (g_liDisplay_ON_millis == -1))
  {
    vON_OFF_fade(1);
    g_liDisplay_ON_millis = millis() + DISPLAY_ON_TIME_MS;
  }
}

void vON_OFF_fade(uint8_t bDirection)
{
  uint8_t byBrightness_fraction = 1;
  uint8_t byTmp_brightness;
  if (bDirection == 0)
  {
    byTmp_brightness = g_byBrightness - byBrightness_fraction;
    for (uint8_t i = 0; i < g_byBrightness - 1; i++)
    {
      analogWrite(BR_PIN, byTmp_brightness);
      byTmp_brightness -= byBrightness_fraction;
      delay(1);
    }
    analogWrite(BR_PIN, 0);
  }
  else
  {
    byTmp_brightness = byBrightness_fraction;
    for (uint8_t i = 0; i < g_byBrightness - 1; i++)
    {
      analogWrite(BR_PIN, byTmp_brightness);
      byTmp_brightness += byBrightness_fraction;
      delay(1);
    }
    analogWrite(BR_PIN, g_byBrightness);
  }
}

void vDisplay_example_screen(uint8_t byDay, uint8_t byMonth, uint16_t wYear, uint8_t byHour, uint8_t byMinute)
{
  vON_OFF_fade(0);
  vMain_screen(byDay, byMonth, wYear, byHour, byMinute);
  vON_OFF_fade(1);
}