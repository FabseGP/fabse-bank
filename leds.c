/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: leds.c
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
#include "adc.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void red_led_task(void *pvParameters) {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	while (1) {
		// Toggle red led
		GPIO_PORTF_DATA_R ^= 0x02;
		uint16_t adc_value = get_adc();
		portTickType delay = 1000 - adc_value / 5;
		vTaskDelay(delay / portTICK_RATE_MS); // Wait 100-1000 ms (200-1000)
	}

}

void yellow_led_task(void *pvParameters) {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	while (1) {
		// Toggle yellow led
		GPIO_PORTF_DATA_R ^= 0x04;
		vTaskDelay(1000 / portTICK_RATE_MS); // Wait 1000 ms
	}

}

void green_led_task(void *pvParameters) {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	while (1) {
		// Toggle green led
		GPIO_PORTF_DATA_R ^= 0x08;
		vTaskDelay(1500 / portTICK_RATE_MS); // Wait 1500 ms
	}

}

/****************************** End Of Module *******************************/
