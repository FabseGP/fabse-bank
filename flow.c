/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: portf.c
 * PROJECT....: fabse_bank
 * DESCRIPTION: See module specification file (.h-file)
 * Change log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------direction
 *****************************************************************************/

/***************************** Include files *******************************/

#include "FreeRTOS.h"
#include "global_def.h"
#include "lcd.h"
#include "queue.h"
#include "rotary.h"
#include "semphr.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void balance() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint16_t balance = 50000;
    uint8_t  exit    = 1;
    while (exit) {
        char i;
        char money_text[] = ">Please enter /balance: ";
        xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
        for (i = 0; i < strlen(money_text); i++) {
            xQueueSend(xLCDQueue, &money_text[i], (TickType_t)0);
        }
        xSemaphoreGive(xLCDSemaphore);
        vTaskDelay(4000 / portTICK_RATE_MS);
        if (balance > 0 && balance < 9999) {
            exit = 0;
        } else {
            char baka_text[] = ">Why you dumb...? /Try again!";
            xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
            for (i = 0; i < strlen(baka_text); i++) {
                xQueueSend(xLCDQueue, &baka_text[i], (TickType_t)0);
            }
            xSemaphoreGive(xLCDSemaphore);
        }
    }
}

void flow_task(void *pvParameters) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    while (1) {
        switch (BankState) {
            case Welcome:
                break;
            case Money:
                balance();
                BankState = Withdraw;
                break;
            case Password:
                break;
            case Withdraw:
                break;
            default:
                break;
        }
    }
}

/****************************** End Of Module *******************************/
