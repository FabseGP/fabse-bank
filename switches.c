/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: portf.c
 * PROJECT....: fabse_bank
 * DESCRIPTION: See module specification file (.h-file)
 * Change log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------direction
 *****************************************************************************/

/***************************** Include files *******************************/

#include "FreeRTOS.h"
#include "global_def.h"
#include "queue.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

enum Sw1_debouncer {
    Debounce_time   = 50,
    Clear_interrupt = 0x11,
    Clear           = 0,
    SW1             = 0x10,
    SW2             = 0x01
};

QueueHandle_t     xSW1Queue, xSW2Queue;
SemaphoreHandle_t xSW1Semaphore, xSW2Semaphore;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_switches_interrupt() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // Table 10-4 have a good overview of the interrupt registers

    // if cleared = detect falling/rising edges, otherwise low/high levels
    GPIO_PORTF_IS_R &= ~(0x11);

    // if cleared = can adjust if interrupted on falling or rising edges
    // otherwise on both falling and rising edges
    GPIO_PORTF_IBE_R &= ~(0x11);

    // if cleared = interrupt on falling edge
    GPIO_PORTF_IEV_R &= ~(0x11);

    // allows interrupts to (quote from datasheet) "be sent to the interrupt
    // controller on the combined interrupt signal"
    GPIO_PORTF_IM_R |= 0x11;

    // clears any previous interrupts on pin PF4
    GPIO_PORTF_ICR_R |= Clear_interrupt;

    // Check table 2-9 on the datasheet for the interrupt table, there you can
    // see that the interrupt number for PORTF is 30

    // PDF-page 151 in datasheet; setting interrupt priority to 3
    NVIC_PRI7_R |= (3 << 21);

    // shifts 00000....1 (32 bits in total) 30 times to the left, setting the
    // 30th bit for enabling PORTF interrupt
    NVIC_EN0_R |= (1 << 30);
}

void init_switches() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOF) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
    }

    // Unlock sw2
    GPIO_PORTF_LOCK_R = 0x4C4F434B;
    GPIO_PORTF_CR_R   = 0x1F;

    // Enable the GPIO pins for digital function (PF4 & PF0)
    GPIO_PORTF_DEN_R |= 0x11;

    // Enable internal pull-up (PF4 & PF0).
    GPIO_PORTF_PUR_R |= 0x11;

    init_switches_interrupt();
}

void switch_debouncer() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t debounce_counter = 0, old_press = GPIO_PORTF_DATA_R, new_press;

    while (debounce_counter <= Debounce_time) {
        new_press = GPIO_PORTF_DATA_R;
        if (debounce_counter == Debounce_time) {
            uint8_t button_press = 1;
            if (new_press & ~(SW1)) {
                xQueueSend(xSW1Queue, &button_press, (TickType_t)10);
                xSemaphoreGive(xSW1Semaphore);
            } else if (new_press & ~(SW2)) {
                xQueueSend(xSW2Queue, &button_press, (TickType_t)10);
                xSemaphoreGive(xSW2Semaphore);
            }
        }
        if (new_press & old_press) {
            debounce_counter++;
        } else {
            debounce_counter = Clear;
        }
        old_press = new_press;
    }

    // Clears any previous interrupts on pin PF4 & PF0
    GPIO_PORTF_ICR_R |= Clear_interrupt;
}

void switch_interrupt_handler() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    switch_debouncer();
}

/****************************** End Of Module ********************************/
