/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: systick_frt.c
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

#include "emp_type.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

enum Systick_interrupt {
    Systick_reload_value      = 16000, // 1 ms
    Systick_priority          = 0x7E,
    Nvic_int_ctrl_pend_syst   = 0x04000000, // Pend a systick int
    Nvic_int_ctrl_unpend_syst = 0x02000000, // Unpend a systick int
};

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_systick() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // Disable systick timer
    NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE);

    // Set current systick counter to reload value
    NVIC_ST_CURRENT_R = Systick_reload_value;
    // Set Reload value, Systick reload register
    NVIC_ST_RELOAD_R = Systick_reload_value;

    // NVIC systick setup, vector number 15
    // Clear pending systick interrupt request
    NVIC_INT_CTRL_R |= Nvic_int_ctrl_unpend_syst;

    // Set systick priority to 0x10, first clear then set
    NVIC_SYS_PRI3_R &= ~(NVIC_SYS_PRI3_TICK_M);
    NVIC_SYS_PRI3_R |=
        (NVIC_SYS_PRI3_TICK_M & (Systick_priority << NVIC_SYS_PRI3_TICK_S));

    // Select systick clock source, Use core clock
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC;

    // Enable systick interrupt
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_INTEN;

    // Enable and start timer
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}

/****************************** End Of Module *******************************/
