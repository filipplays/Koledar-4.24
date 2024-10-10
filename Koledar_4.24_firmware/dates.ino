#include "dates.h"

uint8_t bLeap_year(uint16_t wYear)
{
  if ((fmod(wYear, 4) == 0 && fmod(wYear, 100) != 0) || (fmod(wYear, 400) == 0))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

uint8_t byDay_of_week(uint8_t byMonth, uint8_t byDay, uint16_t wYear)
{
  uint8_t bLeap = bLeap_year(wYear);
  uint16_t yyyy = wYear;
  uint8_t m = byMonth;
  uint8_t d = byDay;
  uint8_t yy;          // Last two digits of the year
  uint8_t c;           // The century
  uint8_t mTable;      // The length of the month from abyMonth_days_len
  uint16_t SummedDate; //
  uint8_t DoW;         // Day of the week (0 - 6)
  uint8_t acTable;
  // The results are limited to 1900 - 2299
  while (yyyy > 2299)
  {
    yyyy = yyyy - 400;
  }
  while (yyyy < 1900)
  {
    yyyy = yyyy + 400;
  }

  // Calculating the century
  c = yyyy / 100;

  // The last two digits of the year
  yy = fmod(yyyy, 100);

  // The century table
  if (c == 19)
  {
    acTable = 1;
  }
  if (c == 20)
  {
    acTable = 0;
  }
  if (c == 21)
  {
    acTable = 5;
  }
  if (c == 22)
  {
    acTable = 3;
  }

  // Acounting for leap years
  if (m == 1)
  {
    if (bLeap)
    {
      mTable = 6;
    }
    else
    {
      mTable = 0;
    }
  }
  if (m == 2)
  {
    if (bLeap)
    {
      mTable = 2;
    }
    else
    {
      mTable = 3;
    }
  }
  // The other months
  if (m == 10)
  {
    mTable = 0;
  }
  if (m == 8)
  {
    mTable = 2;
  }
  if (m == 3 || m == 11)
  {
    mTable = 3;
  }
  if (m == 4 || m == 7)
  {
    mTable = 6;
  }
  if (m == 5)
  {
    mTable = 1;
  }
  if (m == 6)
  {
    mTable = 4;
  }
  if (m == 9 || m == 12)
  {
    mTable = 5;
  }

  // Insert into formula
  SummedDate = d + mTable + yy + (yy / 4) + acTable;

  // Find the day of the week
  DoW = fmod(SummedDate, 7);

  return DoW;
}

uint8_t bSummer_time(uint16_t wYear, uint8_t byMonth, uint8_t byDay)
{
  if (byMonth > 3 && byMonth < 10)
    return 1; // Time time is summer time
  if (byMonth == 3)
  { // If we are in March
    if (byDay < 25)
      return 0; // The first 25 days arent in the summer time
    else
      byDay++; // So we don't check the previous day
    while (byDay < 32)
    { // If we are in the last seven days we check if there was already a Sunday
      if (byDay_of_week(byMonth, byDay, wYear) == 1)
        return 0; // If there wasnt a Sunday
      byDay++;
    }
    return 1; // If there was a Sunday
  }
  if (byMonth == 10)
  {
    if (byDay < 25)
      return 1;
    else
      byDay++;
    while (byDay < 32)
    {
      if (byDay_of_week(byMonth, byDay, wYear) == 1)
        return 1;
      byDay++;
    }
    return 0;
  }
  return 0;
}

uint16_t wDay_diff(uint8_t byBD_day, uint8_t byBD_month, uint8_t byDay, uint8_t byMonth, uint16_t wYear)
{
  uint8_t byTmp_month = byMonth;
  uint16_t wCnt1 = 0;
  while (1)
  {
    if (byTmp_month == byBD_month)
      break;
    wCnt1 = wCnt1 + abyMonth_days_len[byTmp_month - 1];
    if (byTmp_month == 2 and bLeap_year(wYear) == 1)
      wCnt1 = wCnt1 + 1;
    byTmp_month = byTmp_month + 1;
    if (byTmp_month > 12)
      byTmp_month = 1;
  }
  int8_t cCnt2 = byBD_day - byDay;
  int16_t nDays = wCnt1 + cCnt2;
  if (nDays < 0)
    return bLeap_year(wYear) ? 366 + nDays : 365 + nDays;
  return nDays;
}

Update_digit eAdd_minutes(uint8_t byMinutes)
{
  uint8_t byUpdate_digits = 0;
  g_byMinute += byMinutes;

  if (g_byMinute % 10 == 0)
    byUpdate_digits++;

  if (g_byMinute > 59)
  {
    g_byMinute = 0;
    g_byHour++;
    byUpdate_digits = 2;

    if (g_byHour % 10 == 0)
      byUpdate_digits++;

    if (g_byHour > 23)
    {
      g_byHour = 0;
      byUpdate_digits = 4;
      vAdd_day();
    }
  }

  return (Update_digit)byUpdate_digits;
}

uint8_t byUpdate_time(const uint8_t *byTime, uint8_t *byP_time)
{
  uint8_t byUpdate_num = 0;
  if (*byTime != *byP_time)
  {
    byUpdate_num++;
    if (*byTime % 10 == 0)
      byUpdate_num++;

    *byP_time = *byTime;
  }
  return byUpdate_num;
}

void vAdd_day()
{
  g_byDay++;
  if ((bLeap_year(g_wYear) && g_byMonth == 3 && g_byDay > abyMonth_days_len[g_byMonth] + 1) ||
      (!bLeap_year(g_wYear) && g_byMonth == 3 && g_byDay > abyMonth_days_len[g_byMonth]) ||
      (g_byMonth != 3 && g_byDay > abyMonth_days_len[g_byMonth]))
  {
    g_byDay = 1;
    g_byMonth++;

    if (g_byMonth > 11)
    {
      g_byMonth = 1;
      g_wYear++;
    }
  }
}