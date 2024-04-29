/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: flags.h
 *
 * PROJECT....: Assignment8
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

enum Delay {
    Timer1a_on = 1 // 0 = use systick for delay
                   // 1 = use timer1a for delay (can still use systick)
};

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/

#endif
