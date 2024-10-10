/**
 * @file EEPROM_utils.h
 * @author Filip Zevnik
 * @brief The functions that store and load from the EEPROM
 * @version 4.24
 * @date 2024-07-26
 *
 *  The memory map is available in the documentation.
 *
 */

#ifndef eeprom_utils_h
#define eeprom_utils_h

#include "BD.h"

#define BD_SIZE 0x1C
#define BD_OFFSET 0x08
#define TO_ADR(ind) (ind * BD_SIZE) + BD_OFFSET

/**
 * @brief Used to read a BD object from memory
 *
 */
#define READ_BD(bd, i) EEPROM.get(TO_ADR(i), bd);

/**
 * @brief Saves a byte of data to an index (0 --> 7)
 *
 * @param bySave
 * @param byIndex
 */
void vSave_byte(uint8_t bySave, uint8_t byIndex);

/**
 * @brief Read a byte of data from an index
 *
 * @param byIndex
 */
uint8_t vRead_byte(uint8_t byIndex);

/**
 * @brief Initializes the background and text color and the brightness of the display from the EEPROM value.
 *
 */
void vInit_bytes();

#endif