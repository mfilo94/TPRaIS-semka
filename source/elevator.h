/*
 * elevator.h
 *
 *  Created on: 4. 2. 2021
 *      Author: m.filo94
 */

/**
 * @file    elevator.h
 * @brief   Obsahuje hlavicky funkcii v elevator.c
 * @details Subor obsahuje hlavicky a deklaracie premennych.
 */

#ifndef ELEVATOR_H_
#define ELEVATOR_H_

#include <important.h>

/*******************************************************************************
 * Deklaracie premennych
 ******************************************************************************/
extern uint8_t dataData[10], data, startB, adr1, adr2, dataSize, crc;
extern uint8_t lastLimitSwitch, newLimitSwitch, currentFloor, nextFloorLed;
extern _Bool readed, isMoving, opened, up;

/***************************************************************************//**
 * Hlavicky funkcii
 ******************************************************************************/
void readMsg(void);
void closeDoors(void);
void openDoors(void);
void getCurrentFloor(uint8_t direction);
void getNextFloorLED(void);
void LED(uint8_t led, uint8_t onOff);
void startMoving(void);
void stop(void);
void defState(void);
void chooseAction(void);

#endif /* ELEVATOR_H_ */
