/**
 * @file    main.c
 * @brief   Subor s funkcou main spustajucou cely program.
 * @details Subor obsahuje main funkciu ktora inicializuje
 * 			komunikaciu a spusti nekonecnu slucku.
 */
#include <help_functions.h>
#include <elevator.h>
uint8_t data, startB, adr1, adr2, dataSize, crc;
uint8_t dataData[10];
_Bool readed, isMoving, opened, up;
uint8_t lastLimitSwitch, newLimitSwitch, currentFloor, nextFloorLed;
/***************************************************************************//**
 * Funckia inicializuje vsetky komunikaciu a nastavi vytah na poschodie P.
 * V nekonečnom while prebieha citanie sprav, ak je sprava uspesne precitana,
 * zavola funkciu ktora urci co bude dalej nasledovat.
 *
 * @param
******************************************************************************/
int main(void)
{
    lpsci_config_t config;

    BOARD_InitPins();
    BOARD_BootClockRUN();
    CLOCK_SetLpsci0Clock(0x1U);

    LPSCI_GetDefaultConfig(&config);
    config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;

    LPSCI_Init(DEMO_LPSCI, &config, DEMO_LPSCI_CLK_FREQ);
    LPSCI_EnableTx(DEMO_LPSCI, true);
    LPSCI_EnableRx(DEMO_LPSCI, true);

    defState();

    while(1){
    	readMsg();
    	if (readed) {
    		chooseAction();
    		readed = false;
    	}
    }
}
