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

enum States {
    Withdraw_500 = 0,
    Withdraw_200 = 1,
    Withdraw_100 = 2,
    Withdraw_50  = 3,

    Coinage_100  = 0,
    Coinage_50   = 1,
    Coinage_10   = 2,
};

enum Leds {
    Red     = 0x02,
    Yellow  = 0x04,
    Green   = 0x08,

    One_hz  = 1000,
    Two_hz  = 500,
    Four_hz = 250,
};

enum Defines {
    Clear = 0,
    False = 0,
    True  = 1,
};

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
    uint8_t running      = True, button_press;

    lcd_array_send(fabse_text);

    while (running) {
        if (xSemaphoreTake(xSW1Semaphore, (TickType_t)10) == pdTRUE) {
            if (xQueueReceive(xSW1Queue, &button_press, (TickType_t)10) ==
                pdPASS) {
                running = False;
            }
        }
    }
}

void balance() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    uint8_t running = True, index   = Clear;
    char    amount[4], money_text[] = ">Please enter/balance: ";

    lcd_array_send(money_text);

    while (running) {
        if (index == 4) {
            money = atoi(amount);
            if (money > 50 && money <= 9999) {
                lcd_array_send(congratulations);
                running = False;
            } else {
                baka(money_text);
                index = Clear;
            }
        } else {
            if (xSemaphoreTake(xKeypadSemaphore, (TickType_t)10) == pdTRUE) {
                if (xQueueReceive(xKeypadQueue, &amount[index],
                                  (TickType_t)10) == pdPASS) {
                    if (amount[index] == '#' || amount[index] == '*') {
                        baka(money_text);
                        index = Clear;
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

    uint8_t running = True, index        = Clear;
    char    security[4], security_text[] = ">Please enter/password: ";

    lcd_array_send(security_text);

    while (running) {
        if (index == 4) {
            password = atoi(security);
            if (password % 8 == 0) {
                lcd_array_send(congratulations);
                running = False;
            } else {
                baka(security_text);
                index = Clear;
            }
        } else {
            if (xSemaphoreTake(xKeypadSemaphore, (TickType_t)10) == pdTRUE) {
                if (xQueueReceive(xKeypadQueue, &security[index],
                                  (TickType_t)10) == pdPASS) {
                    if (security[index] == '#' || security[index] == '*') {
                        baka(security_text);
                        index = Clear;
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
    uint8_t running = True, state = 0, print_lcd = True, button_press,
            double_press;
    char withdraw_text[] = ">Withdraw amount:/", amount[6];

    lcd_array_send(withdraw_text);

    while (running) {
        if (print_lcd == True) {
            switch (state) {
                case Withdraw_500:
                    strcpy(amount, "/500 kr");
                    withdraw_amount = 500;
                    break;
                case Withdraw_200:
                    strcpy(amount, "/200 kr");
                    withdraw_amount = 200;
                    break;
                case Withdraw_100:
                    strcpy(amount, "/100 kr");
                    withdraw_amount = 100;
                    break;
                case Withdraw_50:
                    strcpy(amount, "/50 kr ");
                    withdraw_amount = 50;
                    break;
                default:
                    break;
            }
            lcd_array_send(amount);
            print_lcd = False;
        }

        if (xSemaphoreTake(xSW1Semaphore, (TickType_t)10) == pdTRUE) {
            if (xQueueReceive(xSW1Queue, &button_press, (TickType_t)10) ==
                pdPASS) {
                if (state >= 3) {
                    state = Clear;
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
                if (xQueueReceive(xSW2Queue, &double_press, (TickType_t)10) ==
                    pdPASS) {
                    if (money < withdraw_amount) {
                        baka(withdraw_text);
                        print_lcd = True;
                    } else {
                        lcd_array_send(congratulations);
                        running = False;
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

    uint8_t running = True, state = 0, print_lcd  = True;
    char    rotary_event, type[7], coinage_text[] = ">Withdraw type:/";

    lcd_array_send(coinage_text);

    while (running) {
        if (print_lcd == True) {
            switch (state) {
                case Coinage_100:
                    strcpy(type, "/100 kr");
                    withdraw_type = 100;
                    delay_timer   = One_hz;
                    blink_led     = Red;
                    break;
                case Coinage_50:
                    strcpy(type, "/50 kr ");
                    withdraw_type = 50;
                    delay_timer   = Two_hz;
                    blink_led     = Yellow;
                    break;
                case Coinage_10:
                    strcpy(type, "/10 kr ");
                    withdraw_type = 10;
                    delay_timer   = Four_hz;
                    blink_led     = Green;
                    break;
                default:
                    break;
            }
            lcd_array_send(type);
            print_lcd = False;
        }

        if (xSemaphoreTake(xRotarySemaphore, (TickType_t)10) == pdTRUE) {
            if (xQueueReceive(xRotaryQueue, &rotary_event, (TickType_t)10) ==
                pdPASS) {
                switch (rotary_event) {
                    case 'R':
                        if (state < Coinage_10) {
                            state++;
                        } else {
                            state = Coinage_100;
                        }
                        print_lcd = True;
                        break;
                    case 'L':
                        if (state > Coinage_100 && state <= Coinage_10) {
                            state--;
                        } else {
                            state = Coinage_10;
                        }
                        print_lcd = True;
                        break;
                    case 'P':
                        if (withdraw_amount % withdraw_type == 0) {
                            lcd_array_send(congratulations);
                            running = False;
                        } else {
                            baka(coinage_text);
                            print_lcd = True;
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
    uint8_t  running      = True;
    uint16_t adc_value;

    lcd_array_send(print_text);
    vTaskDelay(2000 / portTICK_RATE_MS);
    withdraw_amount *= True;
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
            running = False;
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
