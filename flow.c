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
#include "string.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

uint8_t  withdraw_type;
uint16_t money, password, withdraw_amount;

/*****************************   Functions   *******************************/

void welcome() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // "/" = new line, ">" = 2s delay + new screen
    char i, fabse_text[] = ">Welcome to /Fabses bank>Come closer /Money awaits";

    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
    for (i = 0; i < strlen(fabse_text); i++) {
        xQueueSend(xLCDQueue, &fabse_text[i], (TickType_t)10);
    }
    xSemaphoreGive(xLCDSemaphore);

    vTaskDelay(5000 / portTICK_RATE_MS);
}

void balance() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    money           = 50000;
    uint8_t running = 1, index = 0;
    char    i, amount[4] = {}, money_text[] = ">Please enter /balance: ";

    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
    for (i = 0; i < strlen(money_text); i++) {
        xQueueSend(xLCDQueue, &money_text[i], (TickType_t)10);
    }
    xSemaphoreGive(xLCDSemaphore);

    while (running) {
        if (index == 3) {
            money =
                amount[0] * 1000 + amount[1] * 100 + amount[2] * 10 + amount[3];
            if (money <= 9999) {
                char congratulations[] = ">You succeeded!";
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                for (i = 0; i < strlen(congratulations); i++) {
                    xQueueSend(xLCDQueue, &congratulations[i], (TickType_t)10);
                }
                xSemaphoreGive(xLCDSemaphore);
                running = 0;
            } else {
                char baka_text[] = ">Why you dumb...? /Try again!";
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                for (i = 0; i < strlen(baka_text); i++) {
                    xQueueSend(xLCDQueue, &baka_text[i], (TickType_t)10);
                }
                xSemaphoreGive(xLCDSemaphore);
            }
        } else {
            if (xQueueReceive(xKeypadQueue, &amount[index], (TickType_t)10) ==
                pdPASS) {
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                xQueueSend(xLCDQueue, &amount[index], (TickType_t)10);
                xSemaphoreGive(xLCDSemaphore);
                index++;
            }
        }
    }
}

void security_code() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t running = 1, index = 0;
    char    i, security[4] = {}, security_text[] = ">Please enter /password: ";

    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
    for (i = 0; i < strlen(security_text); i++) {
        xQueueSend(xLCDQueue, &security_text[i], (TickType_t)10);
    }
    xSemaphoreGive(xLCDSemaphore);

    while (running) {
        if (index == 3) {
            password = security[0] * 1000 + security[1] * 100 +
                       security[2] * 10 + security[3];
            if (password % 8 == 0) {
                char congratulations[] = ">You succeeded!";
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                for (i = 0; i < strlen(congratulations); i++) {
                    xQueueSend(xLCDQueue, &congratulations[i], (TickType_t)10);
                }
                xSemaphoreGive(xLCDSemaphore);
                running = 0;
            } else {
                char baka_text[] = ">Why you dumb...? /Try again!";
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                for (i = 0; i < strlen(baka_text); i++) {
                    xQueueSend(xLCDQueue, &baka_text[i], (TickType_t)10);
                }
                xSemaphoreGive(xLCDSemaphore);
            }
        } else {
            if (xQueueReceive(xKeypadQueue, &security[index], (TickType_t)10) ==
                pdPASS) {
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                xQueueSend(xLCDQueue, &security[index], (TickType_t)10);
                xSemaphoreGive(xLCDSemaphore);
                index++;
            }
        }
    }
}

void withdraw() {
    uint8_t running = 1, state = 0, print_lcd = 1;

    char    i, withdraw[] = ">Withdraw amount: /";

    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
    for (i = 0; i < strlen(withdraw); i++) {
        xQueueSend(xLCDQueue, &withdraw[i], (TickType_t)10);
    }
    xSemaphoreGive(xLCDSemaphore);

    withdraw_amount = 500;

    while (running) {

        uint8_t button_press, double_press;

        switch (state) {
            case 0:
                if (print_lcd == 1) {
                    char amount[] = "/500";
                    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                    for (i = 0; i < strlen(amount); i++) {
                        xQueueSend(xLCDQueue, &amount[i], (TickType_t)10);
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
                        xQueueSend(xLCDQueue, &amount[i], (TickType_t)10);
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
                        xQueueSend(xLCDQueue, &amount[i], (TickType_t)10);
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
                        xQueueSend(xLCDQueue, &amount[i], (TickType_t)10);
                    }
                    xSemaphoreGive(xLCDSemaphore);
                    print_lcd = 0;
                }
                withdraw_amount = 50;
                break;
            default:
                break;
        }

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
                xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                for (i = 0; i < strlen(congratulations); i++) {
                    xQueueSend(xLCDQueue, &congratulations[i], (TickType_t)10);
                }
                xSemaphoreGive(xSW2Semaphore);
                xSemaphoreGive(xLCDSemaphore);
                running = 0;
            }
        }
    }
}

void coinage() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t running = 1, state = 0, print_lcd = 1;

    char    i, coinage[] = ">Withdraw type: /";

    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
    for (i = 0; i < strlen(coinage); i++) {
        xQueueSend(xLCDQueue, &coinage[i], (TickType_t)10);
    }
    xSemaphoreGive(xLCDSemaphore);

    withdraw_type = 100;

    while (running) {

        uint8_t rotary_event;

        switch (state) {
            case 0:
                if (print_lcd == 1) {
                    char type[] = "/100 kr";
                    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                    for (i = 0; i < strlen(type); i++) {
                        xQueueSend(xLCDQueue, &type[i], (TickType_t)10);
                    }
                    xSemaphoreGive(xLCDSemaphore);
                    print_lcd = 0;
                }
                withdraw_type = 100;
                break;
            case 1:
                if (print_lcd == 1) {
                    char type[] = "/50 kr ";
                    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                    for (i = 0; i < strlen(type); i++) {
                        xQueueSend(xLCDQueue, &type[i], (TickType_t)10);
                    }
                    xSemaphoreGive(xLCDSemaphore);
                    print_lcd = 0;
                }
                withdraw_type = 50;
                break;
            case 2:
                if (print_lcd == 1) {
                    char type[] = "/10 kr ";
                    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                    for (i = 0; i < strlen(type); i++) {
                        xQueueSend(xLCDQueue, &type[i], (TickType_t)10);
                    }
                    xSemaphoreGive(xLCDSemaphore);
                    print_lcd = 0;
                }
                withdraw_type = 10;
                break;
            default:
                break;
        }

        if (xQueueReceive(xRotaryQueue, &rotary_event, (TickType_t)10) ==
            pdPASS) {
            xSemaphoreTake(xRotarySemaphore, (TickType_t)10);

            if (rotary_event != 'P') {
                if (rotary_event == 'R') {
                    if (state < 2) {
                        state++;
                    } else if (state == 2) {
                        state = 0;
                    }
                } else if (rotary_event == 'L') {
                    if (state < 2) {
                        state++;
                    } else if (state == 2) {
                        state = 0;
                    }
                }
                print_lcd = 1;
            } else {
                if (money % withdraw_type == 0) {
                    char congratulations[] = ">You succeeded!";
                    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                    for (i = 0; i < strlen(congratulations); i++) {
                        xQueueSend(xLCDQueue, &congratulations[i],
                                   (TickType_t)10);
                    }
                    xSemaphoreGive(xLCDSemaphore);
                    running = 0;
                } else {
                    char baka_text[] = ">Why you dumb...? /Try again!";
                    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
                    for (i = 0; i < strlen(baka_text); i++) {
                        xQueueSend(xLCDQueue, &baka_text[i], (TickType_t)10);
                    }
                    xSemaphoreGive(xLCDSemaphore);
                }
            }
            xSemaphoreGive(xRotarySemaphore);
        }
    }
}

void print_money() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // "/" = new line, ">" = 2s delay + new screen
    char i, print_text[] = ">Stay calm /Printing money";

    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
    for (i = 0; i < strlen(print_text); i++) {
        xQueueSend(xLCDQueue, &print_text[i], (TickType_t)10);
    }
    xSemaphoreGive(xLCDSemaphore);

    vTaskDelay(5000 / portTICK_RATE_MS);
}

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
                security_code();
                BankState = Withdraw;
                break;
            case Withdraw:
                withdraw();
                BankState = Coinage;
                break;
            case Coinage:
                coinage();
                BankState = Print_money;
                break;
            case Print_money:
                print_money();
                BankState = Welcome;
                break;
            default:
                break;
        }
    }
}

/****************************** End Of Module ********************************/
