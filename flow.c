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
#include "queue.h"
#include "semphr.h"
#include "string.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdlib.h>

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

uint8_t  withdraw_type;
uint16_t money, password, withdraw_amount, delay_timer;

/*****************************   Functions   *******************************/

void lcd_array_send(char data[]) {
    char i;
    xSemaphoreTake(xLCDSemaphore, (TickType_t)10);
    for (i = 0; i < strlen(data); i++) {
        xQueueSend(xLCDQueue, &data[i], (TickType_t)10);
    }
    xSemaphoreGive(xLCDSemaphore);
}

void welcome() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // "/" = new line, ">" = 2s delay + new screen
    char    fabse_text[] = ">Welcome to/Fabse's bank!>Come closer/Money awaits";
    uint8_t running      = 1, button_press;

    lcd_array_send(fabse_text);

    while (running) {
        if (xQueueReceive(xSW1Queue, &button_press, (TickType_t)10) == pdPASS) {
            xSemaphoreTake(xSW1Semaphore, (TickType_t)10);
            running = 0;
            xSemaphoreGive(xSW1Semaphore);
        }
    }
}

void balance() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    money           = 50000;
    uint8_t running = 1, index = 0;
    char    amount[4] = {}, money_text[] = ">Please enter/balance: ";

    lcd_array_send(money_text);

    while (running) {
        if (index == 4) {
            money = atoi(amount[0]) * 1000 + atoi(amount[1]) * 100 +
                    atoi(amount[2]) * 10 + atoi(amount[3]);
            if (money <= 9999) {
                char congratulations[] = ">You succeeded!";
                lcd_array_send(congratulations);
                running = 0;
            } else {
                char baka_text[] = ">Why you dumb...?/Try again!";
                lcd_array_send(baka_text);
                vTaskDelay(2000 / portTICK_RATE_MS);
                lcd_array_send(money_text);
                index = 0;
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
    char    security[4] = {}, security_text[] = ">Please enter/password: ";

    lcd_array_send(security_text);

    while (running) {
        if (index == 4) {
            password = atoi(security[0]) * 1000 + atoi(security[1]) * 100 +
                       atoi(security[2]) * 10 + atoi(security[3]);
            if (password % 8 == 0) {
                char congratulations[] = ">You succeeded!";
                lcd_array_send(congratulations);
                running = 0;
            } else {
                char baka_text[] = ">Why you dumb...?/Try again!";
                lcd_array_send(baka_text);
                vTaskDelay(2000 / portTICK_RATE_MS);
                lcd_array_send(security_text);
                index = 0;
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

    char    withdraw_text[] = ">Withdraw amount:/";

    lcd_array_send(withdraw_text);

    withdraw_amount = 500;

    while (running) {

        uint8_t button_press, double_press;

        switch (state) {
            case 0:
                if (print_lcd == 1) {
                    char amount[] = "/500";
                    lcd_array_send(amount);
                    print_lcd = 0;
                }
                withdraw_amount = 500;
                break;
            case 1:
                if (print_lcd == 1) {
                    char amount[] = "/200";
                    lcd_array_send(amount);
                    print_lcd = 0;
                }
                withdraw_amount = 200;
                break;
            case 2:
                if (print_lcd == 1) {
                    char amount[] = "/100";
                    lcd_array_send(amount);
                    print_lcd = 0;
                }
                withdraw_amount = 100;
                break;
            case 3:
                if (print_lcd == 1) {
                    char amount[] = "/50 ";
                    lcd_array_send(amount);
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
                if (withdraw_amount > money) {
                    char baka_text[] = ">Why you dumb...?/Try again!";
                    lcd_array_send(baka_text);
                    vTaskDelay(2000 / portTICK_RATE_MS);
                    lcd_array_send(withdraw_text);
                    print_lcd = 1;
                } else {
                    char congratulations[] = ">You succeeded!";
                    lcd_array_send(congratulations);
                }
                xSemaphoreGive(xSW2Semaphore);
                running = 0;
            }
        }
    }
}

void coinage() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t running = 1, state = 0, print_lcd = 1, double_press;

    char    coinage[] = ">Withdraw type:/";

    lcd_array_send(coinage);

    withdraw_type = 100;

    while (running) {

        uint8_t rotary_event;

        switch (state) {
            case 0:
                if (print_lcd == 1) {
                    char type[] = "/100 kr";
                    lcd_array_send(type);
                    print_lcd = 0;
                }
                withdraw_type = 100;
                delay_timer   = 1000;
                break;
            case 1:
                if (print_lcd == 1) {
                    char type[] = "/50 kr ";
                    lcd_array_send(type);
                    print_lcd = 0;
                }
                withdraw_type = 50;
                delay_timer   = 500;
                break;
            case 2:
                if (print_lcd == 1) {
                    char type[] = "/10 kr ";
                    lcd_array_send(type);
                    print_lcd = 0;
                }
                withdraw_type = 10;
                delay_timer   = 250;
                break;
            default:
                break;
        }

        if (xQueueReceive(xRotaryQueue, &rotary_event, (TickType_t)10) ==
            pdPASS) {
            xSemaphoreTake(xRotarySemaphore, (TickType_t)10);
            if (state < 2) {
                state++;
            } else if (state == 2) {
                state = 0;
            } else if (state == 0) {
                state = 2;
            }
            print_lcd = 1;
            xSemaphoreGive(xRotarySemaphore);
        }

        if (xQueueReceive(xSW2Queue, &double_press, (TickType_t)10) == pdPASS) {
            vTaskDelay(200 / portTICK_RATE_MS);
            if (xQueueReceive(xSW2Queue, &double_press, (TickType_t)10) ==
                pdPASS) {
                xSemaphoreTake(xSW2Semaphore, (TickType_t)10);
                if (withdraw_amount % withdraw_type == 0) {
                    char congratulations[] = ">You succeeded!";
                    lcd_array_send(congratulations);
                    running = 0;
                } else {
                    char baka_text[] = ">Why you dumb...?/Try again!";
                    lcd_array_send(baka_text);
                    vTaskDelay(2000 / portTICK_RATE_MS);
                    lcd_array_send(coinage);
                    print_lcd = 1;
                }
                xSemaphoreGive(xSW2Semaphore);
            }
        }
    }
}

void print_money() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // "/" = new line, ">" = 2s delay + new screen
    char    print_text[] = ">Stay calm/Printing money";
    uint8_t running      = 1;

    lcd_array_send(print_text);

    while (running) {

        withdraw_amount -= withdraw_type;
        vTaskDelay(delay_timer / portTICK_RATE_MS);

        if (withdraw_amount == 0) {
            char next_customer[] = ">Money withdrawn/Now move aside!";
            lcd_array_send(next_customer);
            vTaskDelay(5000 / portTICK_RATE_MS);
            running = 0;
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
