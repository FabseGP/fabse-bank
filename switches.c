/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: portf.c
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

#include "switches.h"
#include "rotary.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

enum Sw1_debouncer { Debounce_time = 50, Clear_interrupt = 0x10, Clear = 0 };

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_sw1() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOF) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
    }

    // Enable the GPIO pins for digital function (PF4)
    GPIO_PORTF_DEN_R |= 0x10;

    // Enable internal pull-up (PF4).
    GPIO_PORTF_PUR_R |= 0x11;

    init_sw1_interrupt();
}

void init_sw1_interrupt() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // Table 10-4 have a good overview of the interrupt registers

    // if cleared = detect falling/rising edges, otherwise low/high levels
    GPIO_PORTF_IS_R &= ~(0x10);

    // if cleared = can adjust if interrupted on falling or rising edges
    // otherwise on both falling and rising edges
    GPIO_PORTF_IBE_R &= ~(0x10);

    // if cleared = interrupt on falling edge
    GPIO_PORTF_IEV_R &= ~(0x10);

    // allows interrupts to (quote from datasheet) "be sent to the interrupt
    // controller on the combined interrupt signal"
    GPIO_PORTF_IM_R |= 0x10;

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

void sw1_debouncer() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t debounce_counter = 0, old_press = 0;

    // Clears any previous interrupts on pin PF4
    GPIO_PORTF_ICR_R |= Clear_interrupt;

    old_press = GPIO_PORTF_DATA_R;

    while (debounce_counter < Debounce_time) {
        if (GPIO_PORTF_DATA_R & old_press) {
            debounce_counter++;
        } else {
            debounce_counter = Clear;
        }
        if (debounce_counter == Debounce_time) {
            direction = 'P';
        }
        old_press = GPIO_PORTF_DATA_R;
    }
}

void sw1_interrupt_handler() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    sw1_debouncer();
}

/****************************** End Of Module *******************************/
