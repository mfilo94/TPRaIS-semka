/*
 * help_functions.c
 *
 *  Created on: 4. 2. 2021
 *      Author: m.filo94
 */

/**
 * @file    help_functions.c
 * @brief   Subor ktory obsahuje pomocne funkcie
 * @details Nachadzaju sa tu metody na cakanie,
 * 			vypocet kontrolneho suctu a vypisanie dat.
 */

#include "help_functions.h"

/***************************************************************************//**
 * Funkcia ktora caka zadany pocet milisekund.
 *
 * @param milli_seconds
 * @return
 ******************************************************************************/
void delay(int milli_seconds)
{
    /// Storing start time
    clock_t start_time = clock();

    /// looping till required time is not achieved
    while (clock() < start_time + milli_seconds);
}

/***************************************************************************//**
 * Funkcia ktora vypocita CRC8 maxim.
 *
 * @param data
 * @param size
 * @return crc
 ******************************************************************************/
unsigned char dallas_crc8(const unsigned char * data, const unsigned int size)
{
    unsigned char crc = 0;
    for ( unsigned int i = 0; i < size; ++i )
    {
        unsigned char inbyte = data[i];
        for ( unsigned char j = 0; j < 8; ++j )
        {
            unsigned char mix = (crc ^ inbyte) & 0x01;
            crc >>= 1;
            if ( mix ) crc ^= 0x8C;
            inbyte >>= 1;
        }
    }
    return crc;
}

/***************************************************************************//**
 * Funkcia na vypisanie dat.
 *
 * @param
 * @return
 ******************************************************************************/
void printData(void) {
	printf("\nStartBajt: %X\n", startB);
	printf("Adr1: %X\n", adr1);
	printf("Adr2: %X\n", adr2);
	printf("Data size: %X\n", dataSize);
	for (int i = 0; i < dataSize; i++) {
		printf("Data[%d]:%02X\n", i, dataData[i]);
	}
	printf("CRC: %X\n", crc);
	readed = false;
}
