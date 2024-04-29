/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: status_led.h
 *
 * PROJECT....: Assignment8
 *
 * DESCRIPTION: Blinking the status LED
 *
 * Change Log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 260424  MoH    Module created
 *
 *****************************************************************************/

#ifndef _STATUS_LED_H
#define _STATUS_LED_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void status_led_init();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Sets PF0 as output and enables the port
 ******************************************************************************/

void status_led_task(void *pvParameters);
/*****************************************************************************
 *   Input    : Delay time for blinking
 *   Output   : -
 *   Function : Blinking the status LED for a defined time period
 ******************************************************************************/

/****************************** End Of Module *******************************/

#endif
