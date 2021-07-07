/*
 * help_functions.h
 *
 *  Created on: 4. 2. 2021
 *      Author: m.filo94
 */

/**
 * @file    help_functions.h
 * @brief   Obsahuje hlavicky funkcii definovanych v help_functions.c
 * @details Subor obsahuje hlavicky a deklaracie premennych.
 */

#ifndef HELP_FUNCTIONS_H_
#define HELP_FUNCTIONS_H_

#include <important.h>

/*******************************************************************************
 * Deklaracie premennych
 ******************************************************************************/
extern uint8_t dataData[10], data, startB, adr1, adr2, dataSize, crc;
extern _Bool readed;

extern uint8_t data;
extern uint8_t startB;
extern uint8_t adr1;
extern uint8_t adr2;
extern uint8_t dataSize;
extern uint8_t crc;

/***************************************************************************//**
 * Hlavicky funkcii
 ******************************************************************************/
void delay(int milli_seconds);
unsigned char dallas_crc8(const unsigned char * data, const unsigned int size);
void printData(void);

#endif /* HELP_FUNCTIONS_H_ */
