/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: rotary.c
 * PROJECT....: fabse_bank
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

#include "FreeRTOS.h"
#include "global_def.h"
#include "queue.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

enum States { Debounce_time = 50, Clear_interrupt = 0xA0, Clear = 0 };

QueueHandle_t     xRotaryQueue;
SemaphoreHandle_t xRotarySemaphore;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_rotary_interrupt() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // Table 10-4 have a good overview of the interrupt registers

    // if cleared = detect falling/rising edges,  otherwise low/high levels
    GPIO_PORTA_IS_R &= ~(0xA0);

    // if cleared = can adjust if interrupted on falling or rising edges
    // otherwise on both falling and rising edges
    GPIO_PORTA_IBE_R |= 0xA0;

    // allows interrupts to (quote from datasheet) "be sent to the interrupt
    // controller on the combined interrupt signal"
    GPIO_PORTA_IM_R |= 0xA0;

    // clears any previous interrupts on pin PA5
    GPIO_PORTA_ICR_R |= Clear_interrupt;

    // Check table 2-9 on the datasheet for the interrupt table, there you can
    // see that the interrupt number for PORTA is 0

    // PDF-page 151 in datasheet; setting interrupt priority to 3
    NVIC_PRI7_R |= (3 << 21);

    // shifts 00000....1 (32 bits in total) 0 times to the left, setting the 0th
    // bit for enabling PORTA interrupt
    NVIC_EN0_R |= (1 << 0);
}

void init_rotary() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOA) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
    }

    // Enable the GPIO pins for digital function (PA5 - PA7)
    GPIO_PORTA_DEN_R |= 0xE0;

    // Enable internal pull-up (PA7)
    GPIO_PORTA_PUR_R |= 0x80;

    init_rotary_interrupt();
}

void rotary_debouncer() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t debounce_counter = 0, old_transition = GPIO_PORTA_DATA_R,
            new_transition;

    while (debounce_counter <= Debounce_time) {
        new_transition = GPIO_PORTA_DATA_R;
        if (debounce_counter == Debounce_time) {
            char direction;
            // Checks if PA5 and PA6 have the same state
            if ((new_transition & 0x20) && (new_transition & 0x40)) {
                direction = 'L';
            } else if (new_transition & 0x80) {
                direction = 'P';
            } else {
                direction = 'R';
            }
            xSemaphoreTake(xRotarySemaphore, (TickType_t)10);
            xQueueSend(xRotaryQueue, &direction, (TickType_t)10);
            xSemaphoreGive(xRotarySemaphore);
        }
        if (new_transition & old_transition) {
            debounce_counter++;
        } else {
            debounce_counter = Clear;
        }
        old_transition = new_transition;
    }

    // Clears any previous interrupts on pin PF4 & PF0
    GPIO_PORTA_ICR_R |= Clear_interrupt;
}

void rotary_interrupt_handler() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    rotary_debouncer();
}

/****************************** End Of Module *******************************/
