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
#include "global_def.h"
#include "keypad.h"
#include "lcd.h"
#include "leds.h"
#include "money.h"
#include "queue.h"
#include "rotary.h"
#include "semphr.h"
#include "switches.h"
#include "systick_frt.h"
#include "task.h"
#include "timer1a.h"
#include "tm4c123gh6pm.h"
#include "uart.h"

/*****************************    Defines    *******************************/

// Minimum stack size allocated for idle tasks
enum Stack_size { USERTASK_STACK_SIZE = configMINIMAL_STACK_SIZE };

enum Priorities { Idle_prio, Low_prio, Med_prio, High_prio };

BankState = Money;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_queues_semaphores() {
    // Queue with x elements of uint8_t-type + semaphore
    xLCDQueue           = xQueueCreate(100, sizeof(uint8_t));
    xLCDSemaphore       = xSemaphoreCreateBinary();

    xUARTQueue          = xQueueCreate(30, sizeof(uint8_t));
    xUARTSemaphore      = xSemaphoreCreateBinary();

    xKeypadQueue        = xQueueCreate(10, sizeof(uint8_t));
    xKeypadSemaphore    = xSemaphoreCreateBinary();

    xADCQueue           = xQueueCreate(10, sizeof(uint16_t));
    xADCSemaphore       = xSemaphoreCreateBinary();

    xRotaryQueue        = xQueueCreate(5, sizeof(uint8_t));
    xRotarySemaphore    = xSemaphoreCreateBinary();

    xSW1Queue           = xQueueCreate(5, sizeof(uint8_t));
    xSW1Semaphore       = xSemaphoreCreateBinary();

    xSW2Queue           = xQueueCreate(5, sizeof(uint8_t));
    xSW2Semaphore       = xSemaphoreCreateBinary();

    xBankStateSemaphore = xSemaphoreCreateBinary();

    xSemaphoreGive(xLCDSemaphore);
    xSemaphoreGive(xUARTSemaphore);
    xSemaphoreGive(xKeypadSemaphore);
    xSemaphoreGive(xADCSemaphore);
    xSemaphoreGive(xRotarySemaphore);
    xSemaphoreGive(xSW1Semaphore);
    xSemaphoreGive(xSW2Semaphore);
    xSemaphoreGive(xBankStateSemaphore);
}

int main() {

    init_systick();
    init_adc();
    init_switches();
    init_leds();
    init_rotary();
    init_queues_semaphores();
    if (Timer1a_on == 1) {
        // 1 = 4 ms, 250 = 1 s (setting the prescaler)
        init_timer1a(250);
    }

    xTaskCreate(status_led_task, "status_led", USERTASK_STACK_SIZE, NULL,
                Low_prio, NULL);
    xTaskCreate(red_led_task, "red_led", USERTASK_STACK_SIZE, NULL, Low_prio,
                NULL);
    xTaskCreate(yellow_led_task, "yellow_led", USERTASK_STACK_SIZE, NULL,
                Low_prio, NULL);
    xTaskCreate(green_led_task, "green_led", USERTASK_STACK_SIZE, NULL,
                Low_prio, NULL);
    xTaskCreate(lcd_task, "LCD", USERTASK_STACK_SIZE, NULL, Low_prio, NULL);
    xTaskCreate(uart0_task, "UART", USERTASK_STACK_SIZE, NULL, Low_prio, NULL);
    xTaskCreate(money_task, "money_task", USERTASK_STACK_SIZE, NULL, Low_prio,
                NULL);
    // xTaskCreate(keypad_task, "Keypad", USERTASK_STACK_SIZE, NULL, Low_prio,
    // NULL);

    vTaskStartScheduler();

    return 0;
}

/****************************** End Of Module *******************************/
