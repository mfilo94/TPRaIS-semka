/*
 * important.h
 *
 *  Created on: 5. 2. 2021
 *      Author: m.filo94
 */

/**
 * @file    important.h
 * @brief   Obsahuje includy a definicie.
 * @details Subor obsahuje definicie dolezite pre vsetky triedy aplikacie.
 */

#ifndef IMPORTANT_H_
#define IMPORTANT_H_

#include <stdio.h>
#include "fsl_device_registers.h"
#include "board.h"
#include "fsl_lpsci.h"

#include "clock_config.h"
#include "pin_mux.h"
#include <time.h>

#define DEMO_LPSCI UART0
#define DEMO_LPSCI_CLKSRC kCLOCK_CoreSysClk
#define DEMO_LPSCI_CLK_FREQ CLOCK_GetFreq(kCLOCK_CoreSysClk)

#define ON 0x01
#define OFF 0x00
#define UP 0x01
#define DOWN 0x02

#endif /* IMPORTANT_H_ */
