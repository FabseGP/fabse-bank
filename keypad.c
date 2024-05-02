/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: keypad.c
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
#include "global_def.h"
#include "queue.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

#define rows  4
#define cols  3
#define colX1 0x04
#define colX2 0x08
#define colX3 0x10

#define rowY1 0x01;
#define rowY2 0x02;
#define rowY3 0x04;
#define rowY4 0x08;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_keypad() {
    if (SYSCTL_RCGC0_R != SYSCTL_RCGC0_ADC0) {
        SYSCTL_RCGC2_R =
            SYSCTL_RCGC2_R | SYSCTL_RCGC2_GPIOA | SYSCTL_RCGC2_GPIOE;
    }

    // Enable GPIO pins,
    GPIO_PORTA_DEN_R |= 0x1C;
    GPIO_PORTE_DEN_R |= 0x0E;

    // The rows get pull up pins, we will use these as inputs.
    // The rows are the ones with the most
    // GPIO_PORTE_PUR_R |= 0X1C;
    GPIO_PORTE_PDR_R |= 0x0E;

    // PORT A as outputs
    GPIO_PORTA_DIR_R |= 0X1C;
}

void keypad_task(void *pvParameters) {

    // uint8_t rowData =
    // int matrix[3][4] = {0};
    while (1) {

        uint8_t colCheckVal = 0x04;
        int     i           = 0;
        for (i = 0; i < cols; i++) {

            uint8_t rowCheckVal = 0x01;
            int     j           = 0;
            GPIO_PORTA_DATA_R &= 0x00;
            GPIO_PORTA_DATA_R |= colCheckVal;

            switch (colCheckVal) {
                case colX1: {
                    for (j = 0; j < rows; j++) {
                        if (GPIO_PORTE_DATA_R & (rowCheckVal)) {
                            xSemaphoreTake(xLCDSemaphore, portMAX_DELAY);
                            xQueueSend(xLCDQueue, &j, portMAX_DELAY);
                            xSemaphoreGive(xLCDSemaphore);
                            // matrix[i][j] = 1; // Maybe put in queue here

                            // Toggle red led
                            GPIO_PORTF_DATA_R ^= 0x02;
                            // while(1){}
                        }
                        rowCheckVal <<= 1;
                    }
                    break;
                }
                case colX2: {
                    for (j = 0; j < rows; j++) {
                        if (GPIO_PORTE_DATA_R & (rowCheckVal)) {
                            // matrix[i][j] = 1; // Maybe put in queue here
                            //  Toggle red led
                            GPIO_PORTF_DATA_R ^= 0x04;
                            // while(1){}
                        }
                        rowCheckVal <<= 1;
                    }
                    break;
                }
                case colX3: {
                    for (j = 0; j < rows; j++) {
                        if (GPIO_PORTE_DATA_R & (rowCheckVal)) {
                            // matrix[i][j] = 1; // Maybe put in queue here
                            //  Toggle red led
                            GPIO_PORTF_DATA_R ^= 0x08;
                            // while(1){}
                        }
                        rowCheckVal <<= 1;
                    }
                    break;
                }
                default:
                    break;
            }
            colCheckVal <<= 1;
            // TODO: Make this in a for-loop
        }
    }
}

/****************************** End Of Module *******************************/
