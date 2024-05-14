/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: leds.c
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
#include "adc.h"
#include "global_def.h"
#include "queue.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

enum States { Clear = ~(0x0E), Led_off = 0x0E };

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_leds() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOF) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
    }

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOD) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
    }

    // Set the direction as output (PF1 - PF3 & PD6)
    GPIO_PORTF_DIR_R |= 0x0E;
    GPIO_PORTD_DIR_R |= 0x40;

    // Enable the GPIO pins for digital function (PF1 - PF3 & PD6)
    GPIO_PORTF_DEN_R |= 0x0E;
    GPIO_PORTD_DEN_R |= 0x40;

    // Clear LEDs without manipulating the other bits
    GPIO_PORTF_DATA_R &= Clear;
    GPIO_PORTF_DATA_R |= Led_off;
}

/****************************** End Of Module *******************************/
