/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: timer1a.c
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

#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

enum Sw1_debouncer {
    Clear_interrupt = 0x10

};

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_timer1a(uint8_t prescaler) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1; // enable clock for timer1a
    TIMER1_CTL_R = 0x00;                       // disable timer1a output
    TIMER1_CFG_R = 0x04; // 16-bit mode, count up to 2^16 -> maximum delay (with
                         // 16 MHz clock) of 65536/16000000 = 4.096 ms (16 mhz
                         // clock signal, page 58 in datasheet)
    TIMER1_TAMR_R =
        0x02; // 3 modes: one-shot, periodic and capture
              // periodic chosen to enter interrupt periodically after set time
    TIMER1_TAILR_R = 64000 - 1;     // count to 64000, just about 4 ms alone
    TIMER1_TAPR_R  = prescaler - 1; // sets prescaler to 250, since 250*4 ms = 1
                                    // s; -1 here because 250 is 251 bits
    TIMER1_ICR_R = Clear_interrupt; // clear interrupt flag
    TIMER1_IMR_R |= 0x01;           // setting bit 0 enables interrupt mask
                          // apparently "mask" is said about any bit that
                          // toggles functionality
    TIMER1_CTL_R |= 0x01;    // enable timer1a output
    NVIC_EN0_R |= (1 << 21); // Shifts 00000....1 (32 bits in total) 21 times to
                             // the left setting the 21 bit for enabling timer1a
                             // interrupt (its interrupt vector is 21)
}

void timer1a_interrupt_handler() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    TIMER1_ICR_R = Clear_interrupt;
}

/****************************** End Of Module *******************************/
