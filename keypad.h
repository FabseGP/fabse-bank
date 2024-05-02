/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: keypad.h
 *
 * PROJECT....: fabse_bank
 *
 * DESCRIPTION: Initializing keypad
 *
 * Change Log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 260424  MoH    Module created
 *
 *****************************************************************************/

#ifndef _KEYPAD_H_
#define _KEYPAD_H_

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_keypad_and_arr();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : init keypad
 ******************************************************************************/
char** createArray(int, int);

void destroyArray(char**);

void fillKeypadArr(char**);

void keypad_press();

void keypad_task(void* pvParameters);
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Finds the button press in a 3x4 matrix
 ******************************************************************************/

/****************************** End Of Module *******************************/

#endif
