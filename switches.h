/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: portf.h
 *
 * PROJECT....: fabse_bank
 *
 * DESCRIPTION: Initializing portf
 *
 * Change Log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 260424  MoH    Module created
 *
 *****************************************************************************/

#ifndef _PORTF_H_
#define _PORTF_H_

/***************************** Include files *******************************/

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/*****************************    Defines    *******************************/

extern QueueHandle_t     xDirectionQueue;
extern SemaphoreHandle_t xDirectionSemaphore;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_sw1();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Initialize sw1 as output with pullup-resistor
 ******************************************************************************/

void init_sw1_interrupt();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Initialize interrupt vector 30 (sw1)
 ******************************************************************************/

void sw1_debouncer();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Debouncer for sw1
 ******************************************************************************/

void sw1_interrupt_handler();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Handler when falling edge on sw1
 ******************************************************************************/

/****************************** End Of Module *******************************/

#endif
