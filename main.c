/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: main.c
 * PROJECT....: Assignment8
 * DESCRIPTION: Main file
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
#include "systick_frt.h"
#include "FreeRTOS.h"
#include "task.h"
#include "status_led.h"
#include "leds.h"
#include "adc.h"
#include "rotary.h"
#include "lcd.h"
#include "timer1a.h"
#include "portf.h"
#include "uart.h"
#include "flags.h"

/*****************************    Defines    *******************************/

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE // minimum stack size allocated for idle tasks

enum Priorities {
	IDLE_PRIO, LOW_PRIO, MED_PRIO, HIGH_PRIO
};

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

int main() {

	init_systick();
	init_adc();
	init_sw1();
	init_leds();
	init_rotary();
	if (Timer1a_on == 1) {
		init_timer1a(250);        // 1 = 4 ms, 250 = 1 s (setting the prescaler)
	}

	xTaskCreate(status_led_task, "status_led", USERTASK_STACK_SIZE, NULL,
			LOW_PRIO, NULL);
	xTaskCreate(red_led_task, "red_led", USERTASK_STACK_SIZE, NULL, LOW_PRIO,
			NULL);
	xTaskCreate(yellow_led_task, "yellow_led", USERTASK_STACK_SIZE, NULL,
			LOW_PRIO, NULL);
	xTaskCreate(green_led_task, "green_led", USERTASK_STACK_SIZE, NULL,
			LOW_PRIO, NULL);
	xTaskCreate(lcd_task, "LCD", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);
	xTaskCreate(uart0_task, "UART", USERTASK_STACK_SIZE, NULL, LOW_PRIO, NULL);
	vTaskStartScheduler();
	return 0;

}

/****************************** End Of Module *******************************/
