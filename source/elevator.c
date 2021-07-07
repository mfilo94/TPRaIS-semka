/*
 * elevator.c
 *
 *  Created on: 4. 2. 2021
 *      Author: m.filo94
 */
/**
 * @file    elevator.c
 * @brief   Subor pre funkcie priamo suvisiace s vytahom.
 * @details Obsahuje funkcie pre citanie sprav, ovladanie dveri, motoru,
 * 			LEDiek, a zistavanie polohy vytahu.
 */
#include "elevator.h"

/***************************************************************************//**
 * Funkcia sluziaca na citanie sprav. Spravu cita po bitoch, kazdy si ulozi do
 * zodpovedajucej premennej. Ak spravu poslal limit switch, ulozi si ho aby
 * poznal poslednu polohu vytahu. Po uspesnom precitani spravy odosle ACK.
 *
 * @param
 * @return
 ******************************************************************************/
void readMsg(void)
{
	LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
	startB = data;
	if (startB == 0 || startB == 1) return;
	do {
		LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
		adr1 = data;
	} while (adr1 != 0);
	LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
	adr2 = data;
	LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
	dataSize = data;
	for (int i = 0; i < dataSize; i++) {
		LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
		dataData[i] = data;
	}
	LPSCI_ReadBlocking(DEMO_LPSCI, &data, 1);
	crc = data;

	/// get last limit switch
	if (adr2 == 0xE0 || adr2 == 0xE1 || adr2 == 0xE2 || adr2 == 0xE3 || adr2 == 0xE4) lastLimitSwitch = adr2;

	/// send ACK
	if (startB == 0xA0) {
		uint8_t crcData[3] = {adr1, adr2, 0x00};
		uint8_t newCrc = dallas_crc8(crcData, 3);
		uint8_t msg[] = {0xA1, adr2, adr1, 0x00, newCrc};
		LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	}
	readed = true;
}

/***************************************************************************//**
 * Funkcia ktora zatvori na vytahu dvere.
 *
 * @param
 * @return
 ******************************************************************************/
void closeDoors(void) {
	uint8_t msg[] = {0xA0, 0xF0, 0x00, 0x01, 0x01, 0xD3};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	readMsg();
	delay(25);
	opened = false;
}

/***************************************************************************//**
 * Funkcia ktora otvori na vytahu dvere.
 *
 * @param
 * @return
 ******************************************************************************/
void openDoors(void) {
	delay(80);
	uint8_t msg[] = {0xA0, 0xF0, 0x00, 0x01, 0x00, 0x8D};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	readMsg();
	opened = false;
}

/***************************************************************************//**
 * Funkcia ktora posiela na displej spravy so smerom pohybu vytahu a
 * s jeho aktualnou polohou.
 *
 * @param smer pohybu vytahu
 * @return
 ******************************************************************************/
void getCurrentFloor(uint8_t direction) {
	switch (adr2) {
		case 0xE0:
			currentFloor = 0x50;
			break;
		case 0xE1:
			currentFloor = 0x31;
			break;
		case 0xE2:
			currentFloor = 0x32;
			break;
		case 0xE3:
			currentFloor = 0x33;
			break;
		case 0xE4:
			currentFloor = 0x34;
			break;
	}
	uint8_t crcData[] = {0x30, 0x00, direction, currentFloor};
	uint8_t msg[] = {0xA0, 0x30, 0x00, 0x02, direction, currentFloor, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	readMsg();
}

/***************************************************************************//**
 * Funkcia zistujuca adresu LED pri stlacenom tlacidle a
 *  nasledujucu polohu vytahu.
 *
 * @param
 * @return
 ******************************************************************************/
void getNextFloorLED(void) {
	switch (adr2) {
		case 0xb0:
			nextFloorLed = 0x20;
			newLimitSwitch = 0xE0;
			break;
		case 0xc0:
			nextFloorLed = 0x10;
			newLimitSwitch = 0xE0;
			break;
		case 0xb1:
			nextFloorLed = 0x21;
			newLimitSwitch = 0xE1;
			break;
		case 0xc1:
			nextFloorLed = 0x11;
			newLimitSwitch = 0xE1;
			break;
		case 0xb2:
			nextFloorLed = 0x22;
			newLimitSwitch = 0xE2;
			break;
		case 0xc2:
			nextFloorLed = 0x12;
			newLimitSwitch = 0xE2;
			break;
		case 0xb3:
			nextFloorLed = 0x23;
			newLimitSwitch = 0xE3;
			break;
		case 0xc3:
			nextFloorLed = 0x13;
			newLimitSwitch = 0xE3;
			break;
		case 0xb4:
			nextFloorLed = 0x24;
			newLimitSwitch = 0xE4;
			break;
		case 0xc4:
			nextFloorLed = 0x14;
			newLimitSwitch = 0xE4;
			break;
		}
}

/***************************************************************************//**
 * Funkcia ktora ktora sluzi na rozsvecovanie a zhasinanie LED-iek.
 *
 * @param adresa led
 * @param zapnutie alebo vypnutie led
 * @return
 ******************************************************************************/
void LED(uint8_t led, uint8_t onOff) {
	uint8_t crcData[] = {led, 0x00, onOff};
	uint8_t msg[] = {0xA0, led, 0x00, 0x01, onOff, dallas_crc8(crcData, sizeof(crcData))};
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	readMsg();
}

/***************************************************************************//**
 * Funkcia ktora uvedie vytah do pohybu smerom hore alebo dole, pripadne len
 * zavrie a otvori dvere ak bolo stlacene tlacidlo poschodia na ktorom sa nachadza.
 *
 * @param
 * @return
 ******************************************************************************/
void startMoving(void) {
	closeDoors();
	delay(70);

	if (newLimitSwitch > lastLimitSwitch) {
		isMoving = true;
		up = true;
		uint8_t msg[] = {0xa0, 0xf1, 0x00, 0x05, 0x02, 0x64, 0x00, 0x00, 0x00, 0x64}; 	// HORE (100)
		LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
		readMsg();
		getCurrentFloor(UP);
	}
	else {
		if (newLimitSwitch == lastLimitSwitch) {
			if (!isMoving) {
				delay(30);
				openDoors();
				LED(nextFloorLed, OFF);
			}
		}
		else {
			isMoving = true;
			up = false;
			uint8_t msg[] = {0xa0, 0xf1, 0x00, 0x05, 0x02, 0x9c, 0xff, 0xff, 0xff, 0x6f};	// DOLE (-100)
			LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
			readMsg();
			getCurrentFloor(DOWN);
		}
	}
}


/***************************************************************************//**
 * Funckia cita spravy od vytahu pokial nepride na tu, ktora udava pozadovvanu polohu vytahu.
 * Nasledne nastavi vytahu mensiu rychlost a potom ho uplne zastavi.
 * Nakoniec otvori dvere.
 *
 * @param
 * @return
 ******************************************************************************/
void stop(void) {
	do {
		readMsg();
		up ? getCurrentFloor(UP) : getCurrentFloor(DOWN);
	} while (lastLimitSwitch != newLimitSwitch);
	if (lastLimitSwitch == 0xE0) delay(5);
	if (up) {
		uint8_t msg[] = {0xa0, 0xf1, 0x00, 0x05, 0x02, 0x32, 0x00, 0x00, 0x00, 0xB5}; 	// HORE (50)
		LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	} else {
		uint8_t msg1[] = {0xa0, 0xf1, 0x00, 0x05, 0x02, 0xce, 0xff, 0xff, 0xff, 0xB0};	// DOLE (-50)
		LPSCI_WriteBlocking(DEMO_LPSCI, msg1, sizeof(msg1));
	}
	do {
		readMsg();
	} while (dataData[0] != 0x01);
	delay(10);
	if (up) delay(5);
	uint8_t msg2[] = {0xa0, 0xf1, 0x00, 0x01, 0x01, 0x78}; 	// STOP
	LPSCI_WriteBlocking(DEMO_LPSCI, msg2, sizeof(msg2));
	readMsg();
	delay(50);
	openDoors();
	getCurrentFloor(OFF);
	isMoving = false;
}

 /***************************************************************************//**
  * Funckia zatvori dvere vytahu, rozdehne ho smerom dole a zastavi na poschodí P.
  * Nasledne otvori dvere.
  * @param
  * @return
 ******************************************************************************/
void defState(void) {
	closeDoors();
	delay(10);
	if (lastLimitSwitch == 0xC0) return;
	uint8_t msg[] = {0xa0, 0xf1, 0x00, 0x05, 0x02, 0x9c, 0xff, 0xff, 0xff, 0x6f};	// DOLE (-100)
	LPSCI_WriteBlocking(DEMO_LPSCI, msg, sizeof(msg));
	readMsg();
	do {
		readMsg();
		getCurrentFloor(DOWN);
	}
	while (lastLimitSwitch != 0xE0);
	delay(10);
	uint8_t msg2[] = {0xa0, 0xf1, 0x00, 0x01, 0x01, 0x78}; 	// STOP
	LPSCI_WriteBlocking(DEMO_LPSCI, msg2, sizeof(msg2));
	readMsg();
	delay(50);
	getCurrentFloor(OFF);
	openDoors();
}

/***************************************************************************//**
 * Funckia na zaklade toho ci je vytah v pohybe vyberie akciu ktora ma nasledovat.
 * Nasledne otvori dvere.
 * @param
 * @return
******************************************************************************/
void chooseAction(void) {
	if (isMoving) {
		stop();
		LED(nextFloorLed, OFF);
	} else {
		getNextFloorLED();
		LED(nextFloorLed, ON);
		delay(25);
		startMoving();
	}
}
