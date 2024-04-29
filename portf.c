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

#include <stdint.h>
#include "sw1_interrupt.h"
#include "tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

enum States {
	Clear = ~(0x0E), Led_off = 0x0E
};

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void enable_portf() {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	// Enable the GPIO port that is used for the on-board LEDs and switches
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;

}

void init_sw1() {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOF) {
		enable_portf();
	}

	// Enable the GPIO pins for digital function (PF4)
	GPIO_PORTF_DEN_R |= 0x10;

	// Enable internal pull-up (PF4).
	GPIO_PORTF_PUR_R |= 0x11;

	init_sw1_interrupt();

}

void init_leds() {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOF) {
		enable_portf();
	}

	// Set the direction as output (PF1 - PF3)
	GPIO_PORTF_DIR_R |= 0x0E;

	// Enable the GPIO pins for digital function (PF1 - PF3)
	GPIO_PORTF_DEN_R |= 0x0E;

	// Clear LEDs without manipulating the other bits
	GPIO_PORTF_DATA_R &= Clear;
	GPIO_PORTF_DATA_R |= Led_off;

}

/****************************** End Of Module *******************************/
