/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: lcd.h
 *
 * PROJECT....: fabse_bank
 *
 * DESCRIPTION: Initializing LCD-display and sending commands/data
 *
 * Change Log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 260424  MoH    Module created
 *
 *****************************************************************************/

#ifndef _LCD_H_
#define _LCD_H_

/***************************** Include files *******************************/

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/*****************************    Defines    *******************************/

extern QueueHandle_t     xLCDQueue;
extern SemaphoreHandle_t xLCDSemaphore;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_lcd();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Initialize data pins + RS & E
 ******************************************************************************/

void lcd_send(uint8_t data, uint8_t data_type);
/*****************************************************************************
 *   Input    : Data and type of transmission
 *   Output   : -
 *   Function : Send LCD-commands, e.g. 4-bit mode, or LCD-data, e.g. "a"
 ******************************************************************************/

void lcd_task(void *pvParameters);
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : LCD-task
 ******************************************************************************/

/****************************** End Of Module *******************************/

#endif
