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

#define rows  4
#define cols  3
#define colX1 0x04
#define colX2 0x08
#define colX3 0x10

#define rowY1 0x01
#define rowY2 0x02
#define rowY3 0x04
#define rowY4 0x08

#define CLEAR 0x00

QueueHandle_t     xKeypadQueue;
SemaphoreHandle_t xKeypadSemaphore;

/*****************************   Constants   *******************************/

char **keypadArr;

/*****************************   Variables   *******************************/

char              passwordArr[4]  = {0, 0, 0, 0};
int               passwordCounter = 0;
char              moneyArr[4]     = {0, 0, 0, 0};
int               moneyCounter    = 0;
SemaphoreHandle_t xBankStateSemaphore;

/*****************************   Functions   *******************************/

char **createArray(int m, int n) { // m = rows, n = columns

    // Values is the initial "box" of pointers. It has the size of
    // m*n*sizeof(int). Thereby it should be able to contain all the values
    // needed.
    char *values = (char *)(calloc(m * n, sizeof(char)));

    // Error check. If values is a null pointer the function returns NULL
    if (values == NULL) {
        // printf("Memory allocation failed for values!\n");
        return NULL;
    }

    // Rows is the "Start" of all the 2D arrays created within values.
    char **rowsInit = (char **)(malloc(m * sizeof(char *)));
    // Error check. If rows is a null pointer the function returns NULL
    if (rowsInit == NULL) {
        // printf("Memory allocation failed for rows!\n");
        free(values); // Free previously allocated memory
        return NULL;
    }
    int i;
    for (i = 0; i < m; i++) {
        rowsInit[i] =
            values + i * n; // Calculate memory address, corresponding to the
                            // i'th row in the values block. i * n is the offset
                            // to reach the start of the i'th row
    }
    return rowsInit;
}

void destroyArray(char **arr) {
    free(*arr); // Free the space
    free(arr);
}

void fillKeypadArr(char **arr) {
    arr[0][0] = '1';
    arr[0][1] = '2';
    arr[0][2] = '3';
    arr[1][0] = '4';
    arr[1][1] = '5';
    arr[1][2] = '6';
    arr[2][0] = '7';
    arr[2][1] = '8';
    arr[2][2] = '9';
    arr[3][0] = '*';
    arr[3][1] = '0';
    arr[3][2] = '#';
}

void init_keypad_and_arr() {
    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOA) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
    }
    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOE) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;
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
    // Make an array for the keypad
    keypadArr =
        createArray(rows, cols); // The size of the Keypad 4 rows and 3 columns
    fillKeypadArr(&keypadArr);
}

void keypad_press() {

    uint8_t colCheckVal =
        0x04; // This value corresponds with the first GPIO port and will be
              // bitshifted to correspond with the next
    char keypadPressVal;
    int  i = 0;
    for (i = 0; i < cols; i++) {

        uint8_t rowCheckVal =
            0x01; // This value corresponds with the first GPIO port and will be
                  // bitshifted to correspond with the next
        int j = 0;
        GPIO_PORTA_DATA_R &= CLEAR;
        GPIO_PORTA_DATA_R |= colCheckVal;

        switch (colCheckVal) {
            case colX1: {
                for (j = 0; j < rows; j++) {
                    if (GPIO_PORTE_DATA_R & (rowCheckVal)) {
                        xSemaphoreTake(xKeypadSemaphore, (TickType_t)10);
                        keypadPressVal = keypadArr[j][i];
                        xQueueSend(xKeypadQueue, &keypadPressVal,
                                   (TickType_t)10);
                        xSemaphoreGive(xKeypadSemaphore);
                        break;
                    }
                }
                break;
            }
            case colX2: {
                for (j = 0; j < rows; j++) {
                    if (GPIO_PORTE_DATA_R & (rowCheckVal)) {
                        xSemaphoreTake(xKeypadSemaphore, (TickType_t)10);
                        keypadPressVal = keypadArr[j][i];
                        xQueueSend(xKeypadQueue, &keypadPressVal,
                                   (TickType_t)10);
                        xSemaphoreGive(xKeypadSemaphore);
                        break;
                    }
                }
                break;
            }
            case colX3: {
                for (j = 0; j < rows; j++) {
                    if (GPIO_PORTE_DATA_R & (rowCheckVal)) {
                        xSemaphoreTake(xKeypadSemaphore, (TickType_t)10);
                        keypadPressVal = keypadArr[j][i];
                        xQueueSend(xKeypadQueue, &keypadPressVal,
                                   (TickType_t)10);
                        xSemaphoreGive(xKeypadSemaphore);
                        break;
                    }
                }
                break;
            }
            default:
                break;
        }
        colCheckVal <<= 1;
    }
}

void keypad_task(void *pvParameters) {
    init_keypad_and_arr();
    while (1) {
        char a = 'a';

        xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
        xQueueSend(xLCDQueue, &a, (TickType_t)0);
        xSemaphoreGive(xLCDSemaphore);
    }
    while (1) {
        keypad_press();
    }
    destroyArray(keypadArr);
}

/****************************** End Of Module *******************************/
