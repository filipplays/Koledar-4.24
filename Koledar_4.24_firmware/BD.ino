#include "BD.h"
#include "EEPROM_utils.h"
#include "EEPROM.h"
#include "dates.h"

int8_t cElementIndex(char cEl)
{
  for (int8_t i = 0; i < CTABLE_LEN - 1; i++)
  {
    if (acTable[i] == cEl)
      return i;
  }
  Serial.println("INDEX EROR");
  return -1;
}

BD::BD()
{
  this->byDay = -1;
  this->byMonth = -1;
  this->byYear = -1;
  const uint8_t abyPlaceholder_arr[25] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                                          0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  memcpy(this->abyName_surname, abyPlaceholder_arr, 25);
}

BD::BD(const char *abyName_surname, uint8_t byDay, uint8_t byMonth, uint8_t byYear)
{
  this->byDay = byDay;
  this->byMonth = byMonth;
  this->byYear = byYear;

  uint8_t byPrev_bits = 0;
  uint8_t byBits;
  uint8_t cbOut = 0;
  uint8_t byCurr;
  uint8_t byStep;
  int8_t byIndex;

  for (uint8_t i = 0; i < 40; i += 2)
  {
    byBits = 0;
    byCurr = 0;
    byStep = 0;
    if (byPrev_bits != 0)
    {
      byIndex = cElementIndex(abyName_surname[i - 1]);
      byCurr += ((byIndex << byPrev_bits) & 0x1F) << 3;
      byBits += 5 - byPrev_bits;
    }
    if (byBits + 5 <= 8)
    {
      byIndex = cElementIndex(abyName_surname[i]);
      if (byPrev_bits != 0)
      {
        byCurr += byIndex << (3 - (5 - byPrev_bits));
      }
      else
      {
        byCurr += byIndex << 3;
      }
      byBits += 5;
      byPrev_bits = 8 - byBits;
      if (byPrev_bits != 0)
      {
        byIndex = cElementIndex(abyName_surname[i + 1]);
        byCurr += byIndex >> (5 - byPrev_bits);
      }
      else
      {
        byStep = -1;
      }
    }
    else
    {
      byStep = -1;
      byPrev_bits = 8 - byBits;
      byIndex = cElementIndex(abyName_surname[i]);
      byCurr += byIndex >> (5 - byPrev_bits);
    }
    this->abyName_surname[cbOut] = byCurr;
    cbOut++;
    i += byStep;
  }
}

void BD::vDecode_name_surname(char *sOut_string)
{
  uint8_t byPrev_bits = 0;
  uint8_t byPrev_value = 0;
  uint8_t byBits;
  uint8_t cbCnt = 0;
  uint8_t byPnt;

  for (uint8_t i = 0; i < 25; i++)
  {
    byBits = 0;

    uint8_t byCurr = this->abyName_surname[i];

    if (byPrev_bits != 0)
    {
      byBits = 5 - byPrev_bits;
      byPnt = byPrev_value + (byCurr >> (8 - byBits));
      if (byPnt != INVALID)
      {
        sOut_string[cbCnt] = acTable[byPnt];
        sOut_string[cbCnt + 1] = '\0';
      }
      cbCnt++;
    }

    byBits += 5;
    if (byBits > 8)
    {
      byPrev_bits = 5 - byBits % 8;
      byPrev_value = (((byCurr << (8 - byPrev_bits)) & 0xff) >> (3));
      continue;
    }

    byPrev_bits = byPrev_bits == 0 ? 0 : 5 - byPrev_bits;

    byPnt = ((byCurr << byPrev_bits) & 0xff) >> (8 - byBits + byPrev_bits);
    byPrev_bits = 8 - byBits;
    byPrev_value = ((byCurr << byBits) & 0xff) >> 3;
    if (byPnt != INVALID)
    {
      sOut_string[cbCnt] = acTable[byPnt];
      sOut_string[cbCnt + 1] = '\0';
    }
    cbCnt++;
  }
}

void BD::vFormat(char *sOut)
{

  uint8_t byAge = g_wYear - (this->byYear + FIRST_YEAR);
  char sTitle[41];

  this->vDecode_name_surname(sTitle);

  const char asYear_strings[4][5] = {"let", "leto", "leti", "leta"};
  uint8_t bYear_string_index = 0;

  if (byAge > 4 || byAge == 0)
  {
    bYear_string_index = 0;
  }else
  {
    if(byAge == 4) byAge = 3;
    bYear_string_index = byAge;
  }

  sprintf(sOut, "%s %d.%d. %d %s", sTitle, this->byDay, this->byMonth, byAge, asYear_strings[bYear_string_index]);
}

void BD::vFormat_alternative(char *sOut)
{
  char sTitle[41];

  this->vDecode_name_surname(sTitle);

  sprintf(sOut, "%s %d.%d.%d", sTitle, this->byDay, this->byMonth, this->byYear + FIRST_YEAR);
}

void BD::vSave_BD(uint8_t byIndex)
{
  if (byIndex > 145)
    return;

  int iAddress = TO_ADR(byIndex);
  for (uint8_t i = 0; i < 25; i++)
  {
    EEPROM.update(iAddress + i, this->abyName_surname[i]);
  }
  EEPROM.update(iAddress + 25 + 0, this->byDay);
  EEPROM.update(iAddress + 25 + 1, this->byMonth);
  EEPROM.update(iAddress + 25 + 2, this->byYear);
}

uint16_t BD::wDays_till_BD(uint8_t byDay, uint8_t byMonth, uint16_t wYear)
{
  return wDay_diff(this->byDay, this->byMonth, byDay, byMonth, wYear);
}

void BD::vCopy(uint8_t abyName_surname[25], uint8_t *byDay, uint8_t *byMonth, uint8_t *byYear)
{
  memcpy(abyName_surname, this->abyName_surname, 25);
  *byDay = this->byDay;
  *byMonth = this->byMonth;
  *byYear = this->byYear;
}

void BD::operator=(BD *BD_Add)
{
  BD_Add->vCopy(this->abyName_surname, &this->byDay, &this->byMonth, &this->byYear);
}