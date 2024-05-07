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

uint16_t withdraw_amount = 500;

/*****************************   Functions   *******************************/

void welcome() {
    // "/" = new line, ">" = 2s delay + new screen
    char fabse_text[] = "Welcome to /Fabses bank >Come closer /Money awaits";

    // somehow using "char i = 0" in the for loops result in an error, need
    // to define it outside
    char i;

    for (i = 0; i < strlen(fabse_text); i++) {
        xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
        for (i = 0; i < strlen(fabse_text); i++) {
            xQueueSend(xLCDQueue, &fabse_text[i], (TickType_t)0);
        }
        xSemaphoreGive(xLCDSemaphore);
    }
    vTaskDelay(5000 / portTICK_RATE_MS);
}

void balance() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint16_t balance = 50000;
    uint8_t  exit = 1, index = 0;
    char     amount[4] = {};

    char     i;
    char     money_text[] = ">Please enter /balance: ";

    while (exit) {
        if (index == 3) {
            balance =
                amount[0] * 1000 + amount[1] * 100 + amount[2] * 10 + amount[3];
            if (balance >= 0 && balance <= 9999) {
                char congratulations[] = ">You succeeded!";
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                for (i = 0; i < strlen(congratulations); i++) {
                    xQueueSend(xLCDQueue, &congratulations[i], (TickType_t)0);
                }
                xSemaphoreGive(xLCDSemaphore);
                exit = 0;
            } else {
                char baka_text[] = ">Why you dumb...? /Try again!";
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                for (i = 0; i < strlen(baka_text); i++) {
                    xQueueSend(xLCDQueue, &baka_text[i], (TickType_t)0);
                }
                xSemaphoreGive(xLCDSemaphore);
            }
        } else {
            if (xQueueReceive(xKeypadQueue, &amount[index], (TickType_t)10) ==
                pdPASS) {
                index++;
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                xQueueSend(xLCDQueue, &amount[index], (TickType_t)0);
                xSemaphoreGive(xLCDSemaphore);
            }
        }
    }
}

void withdraw() {
    uint8_t exit = 1, state = 0, print_lcd = 1;

    // hvis vi først incrementer last case i slutningen af hver case men
    // current case i starten burde det virke?
    char fabse_amount[] = ">Withdraw amount: /";
    char i;
    for (i = 0; i < strlen(fabse_amount); i++) {
        xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
        for (i = 0; i < strlen(fabse_amount); i++) {
            xQueueSend(xLCDQueue, &fabse_amount[i], (TickType_t)0);
        }
        xSemaphoreGive(xLCDSemaphore);
    }

    while (exit) {

        uint8_t button_press = 0, double_press;

        if (xQueueReceive(xSW1Queue, &button_press, (TickType_t)10) == pdPASS) {
            xSemaphoreTake(xSW1Semaphore, (TickType_t)10);
            state++;
            if (state > 3) {
                state = 0;
            }
            print_lcd = 1;
            xSemaphoreGive(xSW1Semaphore);
        }

        if (xQueueReceive(xSW2Queue, &double_press, (TickType_t)10) == pdPASS) {
            vTaskDelay(200 / portTICK_RATE_MS);
            if (xQueueReceive(xSW2Queue, &double_press, (TickType_t)10) ==
                pdPASS) {
                xSemaphoreTake(xSW2Semaphore, (TickType_t)10);
                char congratulations[] = ">You succeeded!";
                for (i = 0; i < strlen(congratulations); i++) {
                    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                    for (i = 0; i < strlen(congratulations); i++) {
                        xQueueSend(xLCDQueue, &congratulations[i],
                                   (TickType_t)0);
                    }
                    xSemaphoreGive(xSW2Semaphore);
                }
                exit = 0;
            }
        }

        if (exit != 0) {
            switch (state) {
                case 0:
                    if (print_lcd == 1) {
                        char amount[] = "/500";
                        xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                        for (i = 0; i < strlen(amount); i++) {
                            xQueueSend(xLCDQueue, &amount[i], (TickType_t)0);
                        }
                        xSemaphoreGive(xLCDSemaphore);
                        print_lcd = 0;
                    }
                    withdraw_amount = 500;
                    break;
                case 1:
                    if (print_lcd == 1) {
                        char amount[] = "/200";
                        xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                        for (i = 0; i < strlen(amount); i++) {
                            xQueueSend(xLCDQueue, &amount[i], (TickType_t)0);
                        }
                        xSemaphoreGive(xLCDSemaphore);
                        print_lcd = 0;
                    }
                    withdraw_amount = 200;
                    break;
                case 2:
                    if (print_lcd == 1) {
                        char amount[] = "/100";
                        xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                        for (i = 0; i < strlen(amount); i++) {
                            xQueueSend(xLCDQueue, &amount[i], (TickType_t)0);
                        }
                        xSemaphoreGive(xLCDSemaphore);
                        print_lcd = 0;
                    }
                    withdraw_amount = 100;
                    break;
                case 3:
                    if (print_lcd == 1) {
                        char amount[] = "/50 ";
                        xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                        for (i = 0; i < strlen(amount); i++) {
                            xQueueSend(xLCDQueue, &amount[i], (TickType_t)0);
                        }
                        xSemaphoreGive(xLCDSemaphore);
                        print_lcd = 0;
                    }
                    withdraw_amount = 50;
                    break;
            }
        }
    }
}

void coinage() {}

void print_money() {}

void flow_task(void *pvParameters) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    while (1) {
        switch (BankState) {
            case Welcome:
                welcome();
                BankState = Money;
                break;
            case Money:
                balance();
                BankState = Password;
                break;
            case Password:
                BankState = Withdraw;
                break;
            case Withdraw:
                withdraw();
                BankState = Coinage;
                break;
            case Coinage:
                coinage();
                BankState = Coinage;
                break;
            case Print_money:
                print_money();
                BankState = Print_money;
                break;
            default:
                break;
        }
    }
}

/****************************** End Of Module * *******************************/