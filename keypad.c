/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: keypad.c
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
#include "tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

uint8_t row(uint8_t y) {

	uint8_t result = 0;

	switch (y) {
		case 0x01:
			result = 1;
			break;
		case 0x02:
			result = 2;
			break;
		case 0x04:
			result = 3;
			break;
		case 0x08:
			result = 4;
			break;
	}

	return result;
}

uint8_t key_catch(x, y)
	uint8_t x, y; {
	const uint8_t matrix[3][4] = { { '*', '7', '4', '1' },
			{ '0', '8', '5', '2' }, { '#', '9', '6', '3' } };

	return (matrix[x - 1][y - 1]);
}

unsigned char get_keyboard(uint8_t *pch) {
	//return (get_queue(Q_KEY, pch, WAIT_FOREVER));
	return 2;
}

unsigned char check_column(uint8_t x) {
	uint8_t y = GPIO_PORTE_DATA_R & 0x0F; // Save the values of the 4 bits for the rows
	if (y) { // If one of them are set...
		// ...we first find the row number with the function row()
		uint8_t ch = key_catch(x, row(y)); // Now that we have the row and column we look up the corresponding character using the function key_catch
		//put_queue(Q_KEY, ch, 1); // Put the character in a queue so it can be used by another task
		return 1;
	}
	return 0;
}

extern void key_task(uint8_t my_id, uint8_t my_state, uint8_t event,
		uint8_t data) {
	/*****************************************************************************
	 *   Input    :
	 *   Output   :
	 *   Function :
	 ******************************************************************************/

	switch (my_state) {
		case 0:
			GPIO_PORTA_DATA_R &= 0xE3;       // Clear the 3 bits for the columns
			GPIO_PORTA_DATA_R |= 0x10;          // Set the bit for column 1
			if (check_column(1)) { // Check all the rows for column 1, using the function check_column
				// If a button press is registered we go to next state so the press is only registered once
				set_state(1);
				break;
			}
			GPIO_PORTA_DATA_R &= 0xE3; // Repeat the above for the two other columns
			GPIO_PORTA_DATA_R |= 0x08;
			if (check_column(2)) {
				set_state(1);
				break;
			}
			GPIO_PORTA_DATA_R &= 0xE3;
			GPIO_PORTA_DATA_R |= 0x04;
			if (check_column(3)) {
				set_state(1);
				break;
			}
			break;
		case 1:
			if (!(GPIO_PORTE_DATA_R & 0x0F)) { // We stay here until the button is released so a button press is not counted more than once
				set_state(0);
			}
			break;
	}
}

/****************************** End Of Module *******************************/
