/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: rotary.c
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

#include "rotary.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

enum States { Clear_interrupt = 0x20 };

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

uint8_t direction = 'N';

/*****************************   Functions   *******************************/

void init_rotary() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOA) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
    }

    // Enable the GPIO pins for digital function (PA5 - PA7)
    GPIO_PORTA_DEN_R |= 0xE0;

    init_rotary_interrupt();
}

void init_rotary_interrupt() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // Table 10-4 have a good overview of the interrupt registers
    GPIO_PORTA_IS_R &= ~(0x20); // if cleared = detect falling/rising edges,
                                // otherwise low/high levels
    GPIO_PORTA_IBE_R |=
        0x20; // if cleared = can adjust if interrupted on falling or rising
              // edges otherwise on both falling and rising edges
    // GPIO_PORTA_IEV_R &= ~(0x20);    // if cleared = interrupt on falling edge
    GPIO_PORTA_IM_R |= 0x20;  // allows interrupts to (quote from datasheet)
                              // "be sent to the interrupt controller on the
                              // combined interrupt signal"
    GPIO_PORTA_ICR_R |= 0x20; // clears any previous interrupts on pin PA5

    // Check table 2-9 on the datasheet for the interrupt table, there you can
    // see that the interrupt number for PORTA is 0
    NVIC_PRI7_R |=
        (3 << 21); // PDF-page 151 in datasheet; setting interrupt priority to 3
    NVIC_EN0_R |=
        (1 << 0); // shifts 00000....1 (32 bits in total) 0 times to the left,
                  // setting the 0th bit for enabling PORTA interrupt
}

void rotary_interrupt_handler() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    GPIO_PORTA_ICR_R |=
        Clear_interrupt; // Clears any previous interrupts on PA5

    if ((GPIO_PORTA_DATA_R & 0x20) &&
        (GPIO_PORTA_DATA_R &
         0x40)) { // checks if PA5 and PA6 have the same state
        direction = 'L';
    } else {
        direction = 'R';
    }
}

/****************************** End Of Module *******************************/
