/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: flags.h
 *
 * PROJECT....: fabse_bank
 *
 * DESCRIPTION: Toggle functionalities in code with flags
 *
 * Change Log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 260424  MoH    Module created
 *
 *****************************************************************************/

#ifndef _FLAGS_H_
#define _FLAGS_H_

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

// 0 = use systick for delay
// 1 = use timer1a for delay (can still use systick)
enum Delay { Timer1a_on = 1 };

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/

#endif
