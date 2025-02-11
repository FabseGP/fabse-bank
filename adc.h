/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: adc.h
 *
 * PROJECT....: fabse_bank
 *
 * DESCRIPTION: Analog input from EMP-board pot-meter
 *
 * Change Log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 260424  MoH    Module created
 *
 *****************************************************************************/

#ifndef _ADC_H
#define _ADC_H

/***************************** Include files *******************************/

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

uint16_t get_adc();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Read current pot-meter value
 ******************************************************************************/

void init_adc();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Initialize the ADC
 ******************************************************************************/

void adc0_interrupt_handler();
/*****************************************************************************
 *   Input    : -
 *   Output   : -
 *   Function : Handler to start ADC-conversion
 ******************************************************************************/

/****************************** End Of Module *******************************/

#endif
