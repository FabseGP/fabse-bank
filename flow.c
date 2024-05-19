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
#include "adc.h"
#include "global_def.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include <stdlib.h>

/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

char congratulations[] = ">You succeeded!",
     baka_text[]       = ">Why you dumb...?/Try again!";

/*****************************   Variables   *******************************/

uint8_t  withdraw_type, blink_led;
uint16_t money, password, withdraw_amount, delay_timer;

/*****************************   Functions   *******************************/

void lcd_array_send(char data[]) {
    uint8_t i;
    for (i = 0; i < strlen(data); i++) {
        xQueueSend(xLCDQueue, &data[i], (TickType_t)10);
    }
    xSemaphoreGive(xLCDSemaphore);
}

void baka(char arr[]) {
    lcd_array_send(baka_text);
    vTaskDelay(1000 / portTICK_RATE_MS);
    lcd_array_send(arr);
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
        if (xSemaphoreTake(xSW1Semaphore, (TickType_t)10) == pdTRUE) {
            if (xQueueReceive(xSW1Queue, &button_press, (TickType_t)10) ==
                pdPASS) {
                running = 0;
            }
        }
    }
}

void balance() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t running = 1, index      = 0;
    char    amount[4], money_text[] = ">Please enter/balance: ";

    lcd_array_send(money_text);

    while (running) {
        if (index == 4) {
            money = atoi(amount);
            if (money > 50 && money <= 9999) {
                lcd_array_send(congratulations);
                running = 0;
            } else {
                baka(money_text);
                index = 0;
            }
        } else {
            if (xSemaphoreTake(xKeypadSemaphore, (TickType_t)10) == pdTRUE) {
                if (xQueueReceive(xKeypadQueue, &amount[index],
                                  (TickType_t)10) == pdPASS) {
                    if (amount[index] == '#' || amount[index] == '*') {
                        baka(money_text);
                        index = 0;
                    } else {
                        xQueueSend(xLCDQueue, &amount[index], (TickType_t)10);
                        xSemaphoreGive(xLCDSemaphore);
                        index++;
                    }
                }
            }
        }
    }
}

void security_code() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t running = 1, index           = 0;
    char    security[4], security_text[] = ">Please enter/password: ";

    lcd_array_send(security_text);

    while (running) {
        if (index == 4) {
            password = atoi(security);
            if (password % 8 == 0) {
                lcd_array_send(congratulations);
                running = 0;
            } else {
                baka(security_text);
                index = 0;
            }
        } else {
            if (xSemaphoreTake(xKeypadSemaphore, (TickType_t)10) == pdTRUE) {
                if (xQueueReceive(xKeypadQueue, &security[index],
                                  (TickType_t)10) == pdPASS) {
                    if (security[index] == '#' || security[index] == '*') {
                        baka(security_text);
                        index = 0;
                    } else {
                        xQueueSend(xLCDQueue, &security[index], (TickType_t)10);
                        xSemaphoreGive(xLCDSemaphore);
                        index++;
                    }
                }
            }
        }
    }
}

void withdraw() {
    uint8_t running = 1, state = 0, print_lcd = 1, button_press, double_press;
    char    withdraw_text[] = ">Withdraw amount:/", amount[6];

    lcd_array_send(withdraw_text);

    while (running) {
        if (print_lcd == 1) {
            switch (state) {
                case 0:
                    strcpy(amount, "/500 kr");
                    withdraw_amount = 500;
                    break;
                case 1:
                    strcpy(amount, "/200 kr");
                    withdraw_amount = 200;
                    break;
                case 2:
                    strcpy(amount, "/100 kr");
                    withdraw_amount = 100;
                    break;
                case 3:
                    strcpy(amount, "/50 kr ");
                    withdraw_amount = 50;
                    break;
                default:
                    break;
            }
            lcd_array_send(amount);
            print_lcd = 0;
        }
        if (xSemaphoreTake(xSW1Semaphore, (TickType_t)10) == pdTRUE) {
            if (xQueueReceive(xSW1Queue, &button_press, (TickType_t)10) ==
                pdPASS) {
                if (state >= 3) {
                    state = 0;
                } else {
                    state++;
                }
                print_lcd = 1;
            }
            xQueueReset(xSW1Queue);
        }

        if (xSemaphoreTake(xSW2Semaphore, (TickType_t)10) == pdTRUE) {
            if (xQueueReceive(xSW2Queue, &double_press, (TickType_t)10) ==
                pdPASS) {
                vTaskDelay(200 / portTICK_RATE_MS);
                if (xSemaphoreTake(xSW2Semaphore, (TickType_t)10) == pdTRUE) {
                    if (xQueueReceive(xSW2Queue, &double_press,
                                      (TickType_t)10) == pdPASS) {
                        if (money < withdraw_amount) {
                            baka(withdraw_text);
                            print_lcd = 1;
                        } else {
                            lcd_array_send(congratulations);
                            running = 0;
                        }
                    }
                } else {
                    if (state == 0) {
                        state = 3;
                    } else {
                        state--;
                    }
                    print_lcd = 1;
                }
            }
            xQueueReset(xSW2Queue);
        }
    }
}

void coinage() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t running = 1, state = 0, print_lcd     = 1;
    char    rotary_event, type[7], coinage_text[] = ">Withdraw type:/";

    lcd_array_send(coinage_text);

    while (running) {
        if (print_lcd == 1) {
            switch (state) {
                case 0:
                    strcpy(type, "/100 kr");
                    withdraw_type = 100;
                    delay_timer   = 1000;
                    blink_led     = 0x02;
                    break;
                case 1:
                    strcpy(type, "/50 kr ");
                    withdraw_type = 50;
                    delay_timer   = 500;
                    blink_led     = 0x04;
                    break;
                case 2:
                    strcpy(type, "/10 kr ");
                    withdraw_type = 10;
                    delay_timer   = 250;
                    blink_led     = 0x08;
                    break;
                default:
                    break;
            }
            lcd_array_send(type);
            print_lcd = 0;
        }

        if (xSemaphoreTake(xRotarySemaphore, (TickType_t)10) == pdTRUE) {
            if (xQueueReceive(xRotaryQueue, &rotary_event, (TickType_t)10) ==
                pdPASS) {
                switch (rotary_event) {
                    case 'R':
                        if (state < 2) {
                            state++;
                        } else {
                            state = 0;
                        }
                        print_lcd = 1;
                        break;
                    case 'L':
                        if (state > 0 && state <= 2) {
                            state--;
                        } else {
                            state = 2;
                        }
                        print_lcd = 1;
                        break;
                    case 'P':
                        if (withdraw_amount % withdraw_type == 0) {
                            lcd_array_send(congratulations);
                            running = 0;
                        } else {
                            baka(coinage_text);
                            print_lcd = 1;
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void print_money() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // "/" = new line, ">" = 2s delay + new screen
    char     print_text[] = ">Stay calm/Printing money";
    uint8_t  running      = 1;
    uint16_t adc_value;

    lcd_array_send(print_text);
    vTaskDelay(2000 / portTICK_RATE_MS);
    withdraw_amount *= 2;
    while (running) {
        adc_value = get_adc();
        withdraw_amount -= withdraw_type;
        GPIO_PORTF_DATA_R ^= blink_led;
        vTaskDelay(delay_timer / portTICK_RATE_MS);
        delay_timer = 1000 - adc_value / 5;
        if (withdraw_amount == 0) {
            char next_customer[] = ">Money withdrawn/Now move aside!";
            lcd_array_send(next_customer);
            vTaskDelay(3000 / portTICK_RATE_MS);
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

/****************************** End Of Module *
 * ********************************/
