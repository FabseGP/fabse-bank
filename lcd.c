/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: lcd.c
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
#include <string.h>
#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "rotary.h"

/*****************************    Defines    *******************************/

enum Modes {
	Instruction, Output
};

enum Commands {
	Clear = 0x01,               // clear display
	Transfer_mode = 0x28,       // D5 = 1 = 4-bit mode, D3 = 1 = two lines
	On = 0x0F,                  // D2 = 1 = display on,
								// D1 = 1 = cursor displayed, D0 = 1 = cursor blinks
	Entry = 0x06,               // increments cursor, i.e. moves to the right
	Top_line = 0x00,            // moves to top line
	Bottom_line = 0xC0,         // moves to bottom line
	Cursor_right = 0x14,        // shifts cursor to the right
	Cursor_left = 0x10          // shifts cursor to the left
};

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_lcd() {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD; // enable gpio ports C and D

	// D4 - D7 as outputs
	GPIO_PORTC_DIR_R |= 0xF0;
	GPIO_PORTC_DEN_R |= 0xF0;

	// RS + E + LED as outputs
	GPIO_PORTD_DIR_R |= 0x4C;
	GPIO_PORTD_DEN_R |= 0x4C;

	// Toggling 4-bit mode
	GPIO_PORTD_DATA_R = 0x00;   // RS = 0
	GPIO_PORTC_DATA_R = 0x20;   // 4-bit mode
	GPIO_PORTD_DATA_R = 0x08;   // E = 1
	vTaskDelay(2 / portTICK_RATE_MS);
	GPIO_PORTD_DATA_R ^= 0x08;  // E = 0

}

void lcd_send(uint8_t data, uint8_t data_type) { // data_type = 0 -> command, data_type = 1 -> 'data'
/*****************************************************************************
 *   Function : See module specification (.h-file)
 *****************************************************************************/

	GPIO_PORTD_DATA_R = (data_type << 2);   // Setting mode for data-transfer
	GPIO_PORTC_DATA_R = data;               // Sending 4 most significant bit
	GPIO_PORTD_DATA_R |= 0x08;              // E = 1
	vTaskDelay(2 / portTICK_RATE_MS);
	GPIO_PORTD_DATA_R ^= 0x08;              // E = 0
	GPIO_PORTC_DATA_R = (data << 4);        // Bit-shift 4 times to the left,
											// sending 4 least significant bit
	GPIO_PORTD_DATA_R ^= 0x08;
	vTaskDelay(2 / portTICK_RATE_MS);
	GPIO_PORTD_DATA_R ^= 0x08;

}

void lcd_task(void *pvParameters) {
	/*****************************************************************************
	 *   Function : See module specification (.h-file)
	 *****************************************************************************/

	init_lcd();

	lcd_send(Transfer_mode, Instruction);
	lcd_send(On, Instruction);
	lcd_send(Clear, Instruction);
	lcd_send(Entry, Instruction);

	char fabse_text[] = "Get linux! >:(";
	char i;             // somehow using "char i = 0" in the for loops result in
						// an error, need to define it outside

	for (i = 0; i < strlen(fabse_text); i++) {
		lcd_send(fabse_text[i], Output);
	}

	lcd_send(Bottom_line, Instruction);

	while (1) {
		lcd_send(direction, Output);
	}

}

/****************************** End Of Module *******************************/
