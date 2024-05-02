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
#include "flags.h"
#include "global_def.h"
#include "lcd.h"
#include "leds.h"
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
        // 1 = 4 ms, 250 = 1 s (setting the prescaler)
        init_timer1a(250);
    }
    // 9600 baudrate, 8 data-bits, 1 stop-bit and no parity
    uart0_init(9600, 8, 1, 'n');

    // Queue with 10 elements of uint8_t-type + semaphore
    xLCDQueue      = xQueueCreate(10, sizeof(uint8_t));
    xLCDSemaphore  = xSemaphoreCreateBinary();

    xUARTQueue     = xQueueCreate(10, sizeof(uint8_t));
    xUARTSemaphore = xSemaphoreCreateBinary();

    if (xUARTQueue == NULL) {
        while (1) {
            uart_putc('N');
        }
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

    xSemaphoreGive(xLCDSemaphore);
    xSemaphoreGive(xUARTSemaphore);

    vTaskStartScheduler();

    return 0;
}

/****************************** End Of Module *******************************/
