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
#include <stdlib.h>

/*****************************    Defines    *******************************/

QueueHandle_t     xKeypadQueue;
SemaphoreHandle_t xKeypadSemaphore;
TaskHandle_t      xKeypadHandle;

enum Keypad_size {
    Rows = 4,
    Cols = 3,
};

enum Defines {
    Clear = 0,
    False = 0,
    True  = 1,
};

/*****************************   Constants   *******************************/

char lookup_table[Rows][Cols] = {
    {'#', '0', '*'}, {'9', '8', '7'}, {'6', '5', '4'}, {'3', '2', '1'}};

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void keypad_scan() {
    uint8_t exit = False, i, j;
    for (i = 0; i < 3; i++) {
        GPIO_PORTA_DATA_R = (0x01 << i + 2);
        for (j = 0; j < 4; j++) {
            if (GPIO_PORTE_DATA_R & (0x01 << j)) {
                vTaskDelay(150 / portTICK_RATE_MS);
                xQueueSend(xKeypadQueue, &lookup_table[j][i], (TickType_t)10);
                xSemaphoreGive(xKeypadSemaphore);
                while (GPIO_PORTE_DATA_R & (0x01 << j)) {
                }
                GPIO_PORTA_DATA_R = Clear;
                exit              = True;
                break;
            }
            if (exit == True) {
                break;
            }
        }
    }
}

void init_keypad() {
    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOA) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
    }
    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOE) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
    }

    // Enable GPIO pins,
    GPIO_PORTA_DEN_R |= 0x1C;
    GPIO_PORTE_DEN_R |= 0x0F;

    // The rows get pull up pins, we will use these as inputs.
    // The rows are the ones with the most
    // GPIO_PORTE_PUR_R |= 0X1C;
    GPIO_PORTE_PDR_R |= 0x0F;

    // PORT A as outputs
    GPIO_PORTA_DIR_R |= 0x1C;
}

void keypad_task(void *pvParameters) {

    init_keypad();

    while (1) {
        keypad_scan();
    }
}

/****************************** End Of Module *******************************/
