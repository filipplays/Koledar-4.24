/**
 * @file BD.h
 * @author Filip Zevnik
 * @brief The BD class and its member functions.
 * @version 4.24
 * @date 2024-07-26
 *
 */

#ifndef birthday_h
#define birthday_h

#define MAX_DAYS_TILL_BD 10

#define CTABLE_LEN 32
#define INVALID CTABLE_LEN - 2

#define FIRST_YEAR 1900

char acTable[CTABLE_LEN] = "abc\1defghijklmnopqrs\2tuvwxyz\3 .";

/**
 * @brief This is the class that stores one persons information for the purpose of displaying their birthday.
 *
 * The name and surname are stored in a 25 B long array, because each letter only uses 5 bits of data. This possible
 * because I need to encode less than 32 characters. The data is sent to the calendar by the Koledar GUI in the
 * encoded form and is decoded as needed by the vFormat member function.
 * The age is an 8 bit integer and is converted to the correct year of birth by adding it to the "FIRST_YEAR" macro
 * which means that the lowest a year of birth could be is FIRST_YEAR and the highest is FIRST_YEAR + 255.
 *
 */
class BD
{
  // 5 bits per character for 40 characters = 200 bits or 25 bytes
  uint8_t abyName_surname[25];
  uint8_t byDay, byMonth, byYear;

public:
  /**
   * @brief Construct a new BD object using the encoded name and surname byte array.
   *
   * @param abyName_surname
   * @param byDay
   * @param byMonth
   * @param byYear
   */
  BD(const char *abyName_surname, uint8_t byDay, uint8_t byMonth, uint8_t byYear);
  /**
   * @brief Construct a new BD object with default values.
   *
   */
  BD();
  /**
   * @brief Used to copy one BD object to another.
   *
   * @param BD_Add
   * @return BD
   */
  BD operator=(BD *BD_Add);
  /**
   * @brief Copies the BD objects values into the given vars.
   *
   * @param abyName_surname
   * @param byDay
   * @param byMonth
   * @param byYear
   */
  void vCopy(uint8_t abyName_surname[25], uint8_t byDay, uint8_t byMonth, uint8_t byYear);
  /**
   * @brief Formats the encoded name and surname along with the date information and calculated age and saves
   * it to the sOut paramater string.
   *
   * The sOut char array should be maximum 58 B long.
   *
   * @param sOut
   */
  void vFormat(char *sOut);

  /**
   * @brief Provides an alternative data formatting method that is used for sending the data to the KOLEDAR GUI.
   *
   * @param sOut
   */
  void vFormat_alternative(char *sOut);

  /**
   * @brief Decodes and returns the name and surname into the paramater variables
   *
   * @param sOut_string
   */
  void vDecode_name_surname(char *sOut_string);

  /**
   * @brief Calculates the difference between the birtday date and the current day in days.
   *
   * This function is used to know when to display the birthday on the display.
   *
   * @return uint8_t
   */
  uint16_t wDays_till_BD(uint8_t byDay, uint8_t byMonth, uint16_t wYear);

  /**
   * @brief Used for saving the BD class object to the EEPROM.
   *
   * Param byIndex should be higher than -1 and less than 146.
   *
   * @param byIndex
   */
  void vSave_BD(uint8_t byIndex);
};

/**
 * @brief Returns the index of the given character from the acTable variable.
 *
 * @param cEl
 * @return int8_t
 */
int8_t cElementIndex(char cEl);

#endif