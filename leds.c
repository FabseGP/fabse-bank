/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: leds.c
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
#include "adc.h"
#include "global_def.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "tm4c123gh6pm.h"

/*****************************    Defines    *******************************/

enum States { Clear = ~(0x0E), Led_off = 0x0E };

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

void init_leds() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOF) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
    }

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOD) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD;
    }

    // Set the direction as output (PF1 - PF3 & PD6)
    GPIO_PORTF_DIR_R |= 0x0E;
    GPIO_PORTD_DIR_R |= 0x40;

    // Enable the GPIO pins for digital function (PF1 - PF3 & PD6)
    GPIO_PORTF_DEN_R |= 0x0E;
    GPIO_PORTD_DEN_R |= 0x40;

    // Clear LEDs without manipulating the other bits
    GPIO_PORTF_DATA_R &= Clear;
    GPIO_PORTF_DATA_R |= Led_off;
}

void red_led_task(void *pvParameters) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    while (1) {
        GPIO_PORTF_DATA_R ^= 0x02;

        /*
        uint16_t adc_value;

        // Start conversion at sequencer 3
        ADC0_PSSI_R = 0x08;

        if (xQueueReceive(xADCQueue, &adc_value, (TickType_t)10) == pdPASS) {
            xSemaphoreTake(xADCSemaphore, (TickType_t)10);
            // Wait 100-1000 ms (200-1000)
            portTickType delay = 1000 - adc_value / 5;
            vTaskDelay(delay / portTICK_RATE_MS);
            xSemaphoreGive(xADCSemaphore);
        } */

        uint16_t adc_value = get_adc();
        // Wait 100-1000 ms (200-1000)
        portTickType delay = 1000 - adc_value / 5;
        vTaskDelay(delay / portTICK_RATE_MS);
        xSemaphoreGive(xADCSemaphore);
    }
}

void yellow_led_task(void *pvParameters) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    while (1) {
        GPIO_PORTF_DATA_R ^= 0x04;
        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void green_led_task(void *pvParameters) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    while (1) {
        GPIO_PORTF_DATA_R ^= 0x08;
        vTaskDelay(1500 / portTICK_RATE_MS);
    }
}

void status_led_task(void *pvParameters) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    while (1) {
        GPIO_PORTD_DATA_R ^= 0x40;
        vTaskDelay(500 / portTICK_RATE_MS);
    }
}

/****************************** End Of Module *******************************/
