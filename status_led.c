/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: status_led.c
 * PROJECT....: Assignment8
 * DESCRIPTION: See module specification file (.h-file)
 * Change log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 260424  MoH    Module created
 *
 *****************************************************************************/

/***************************** Include files *******************************/

#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void status_led_init() {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	// Enable the GPIO port that is used for the on-board LED
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;

	// Set the direction as output (PF0)
	GPIO_PORTD_DIR_R |= 0x40;

	// Enable the GPIO pins for digital function (PF0)
	GPIO_PORTD_DEN_R |= 0x40;

}

void status_led_task(void *pvParameters) {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	status_led_init();

	while (1) {
		GPIO_PORTD_DATA_R ^= 0x40; // Toggle status LED
		vTaskDelay(500 / portTICK_RATE_MS); // Wait 500 ms
	}

}

/****************************** End Of Module *******************************/
