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

enum States { Clear_interrupt = 0xA0 };

QueueHandle_t     xRotaryQueue;
SemaphoreHandle_t xRotarySemaphore;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_encoder_interrupt() {
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
    GPIO_PORTA_ICR_R |= 0xA0;

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

    // Disable the GPIO pins for digital function (PA7)
    GPIO_PORTA_DEN_R &= ~(0x9F);
    GPIO_PORTA_DEN_R |= 0xE0;

    // Disable internal pull-up (PA7).
    GPIO_PORTA_PUR_R &= ~(0x80);
}

void init_button_press() {
    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOA) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
    }

    // Enable the GPIO pins for digital function (PA5 - PA7)
    GPIO_PORTA_DEN_R &= ~(0x7F);
    GPIO_PORTA_DEN_R |= 0x80;

    // Enable internal pull-up (PA7).
    GPIO_PORTA_PUR_R |= 0x80;
}

void encoder_button_interrupt_handler() {
    init_button_press();

    if (GPIO_PORTA_DATA_R & (0x80)) {
        char direction = 'P';
        xSemaphoreTake(xRotarySemaphore, (TickType_t)10);
        xQueueSend(xRotaryQueue, &direction, (TickType_t)10);
        xSemaphoreGive(xRotarySemaphore);
    }
}

void rotary_interrupt_handler() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/
    init_rotary();
    // Checks if PA5 and PA6 have the same state
    if ((GPIO_PORTA_DATA_R & 0x20) && (GPIO_PORTA_DATA_R & 0x40)) {
        char direction = 'L';
        xSemaphoreTake(xRotarySemaphore, (TickType_t)10);
        xQueueSend(xRotaryQueue, &direction, (TickType_t)10);
        xSemaphoreGive(xRotarySemaphore);
    } else {
        char direction = 'R';
        xSemaphoreTake(xRotarySemaphore, (TickType_t)10);
        xQueueSend(xRotaryQueue, &direction, (TickType_t)10);
        xSemaphoreGive(xRotarySemaphore);
    }

    // Clears any previous interrupts on PA5
    GPIO_PORTA_ICR_R |= Clear_interrupt;
}

/****************************** End Of Module *******************************/
