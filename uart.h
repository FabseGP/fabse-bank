/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: uart.h
 *
 * PROJECT....: fabse_bank
 *
 * DESCRIPTION: Initializing UART
 *
 * Change Log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 260424  MoH    Module created
 *
 *****************************************************************************/

#ifndef _UART_H_
#define _UART_H_

/***************************** Include files *******************************/

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/*****************************    Defines    *******************************/

extern QueueHandle_t     xUARTQueue;
extern SemaphoreHandle_t xUARTSemaphore;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

unsigned char uart0_rx_rdy();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Character ready at uart0 RX
 ******************************************************************************/

uint8_t uart0_getc();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Get character from uart0 RX
 ******************************************************************************/

unsigned char uart0_tx_rdy();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : uart0 TX buffer ready
 ******************************************************************************/

void uart0_putc(uint8_t ch);
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Put character to uart0 TX
 ******************************************************************************/

void uart0_init(uint32_t baud_rate, uint8_t data_size, uint8_t stop_bits,
                uint8_t parity);
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Initialize uart 0
 ******************************************************************************/

void uart0_task(void *pvParameters);
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : LCD-task
 ******************************************************************************/

/****************************** End Of Module *******************************/

#endif
