/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: main.c
 * PROJECT....: fabse_bank
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

#include "FreeRTOS.h"
#include "adc.h"
#include "flow.h"
#include "global_def.h"
#include "keypad.h"
#include "lcd.h"
#include "leds.h"
#include "queue.h"
#include "rotary.h"
#include "semphr.h"
#include "switches.h"
#include "systick_frt.h"
#include "task.h"
#include "tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

// Minimum stack size allocated for idle tasks
enum Stack_size { USERTASK_STACK_SIZE = configMINIMAL_STACK_SIZE };

enum Priorities { Idle_prio, Low_prio, Med_prio, High_prio };

BankState = Welcome;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_queues_semaphores() {
    // Queue with x elements of uint8_t-type + semaphore
    xLCDQueue        = xQueueCreate(140, sizeof(uint8_t));
    xLCDSemaphore    = xSemaphoreCreateBinary();

    xKeypadQueue     = xQueueCreate(30, sizeof(uint8_t));
    xKeypadSemaphore = xSemaphoreCreateBinary();

    xRotaryQueue     = xQueueCreate(30, sizeof(uint8_t));
    xRotarySemaphore = xSemaphoreCreateBinary();

    xSW1Queue        = xQueueCreate(30, sizeof(uint8_t));
    xSW1Semaphore    = xSemaphoreCreateBinary();

    xSW2Queue        = xQueueCreate(30, sizeof(uint8_t));
    xSW2Semaphore    = xSemaphoreCreateBinary();

    xSemaphoreGive(xLCDSemaphore);
    xSemaphoreGive(xKeypadSemaphore);
    xSemaphoreGive(xRotarySemaphore);
    xSemaphoreGive(xSW1Semaphore);
    xSemaphoreGive(xSW2Semaphore);
}

int main() {

    init_systick();
    init_adc();
    init_switches();
    init_leds();
    init_rotary();
    init_queues_semaphores();

    xTaskCreate(lcd_task, "LCD_task", USERTASK_STACK_SIZE, NULL, Low_prio,
                NULL);
    xTaskCreate(flow_task, "flow_task", USERTASK_STACK_SIZE, NULL, Low_prio,
                NULL);
    xTaskCreate(keypad_task, "keypad_task", USERTASK_STACK_SIZE, NULL, Low_prio,
                NULL);

    vTaskStartScheduler();

    return 0;
}

/****************************** End Of Module *******************************/
