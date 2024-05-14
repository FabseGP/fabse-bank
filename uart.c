/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: uart.c
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

QueueHandle_t     xUARTQueue;
SemaphoreHandle_t xUARTSemaphore;

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

/*****************************   Functions   *******************************/

uint32_t lcrh_databits(uint8_t data_size) {
    /*****************************************************************************
     *   Input    :
     *   Output   :
     *   Function : sets bit 5 and 6 according to the wanted number of data
     *bits. 5: bit5 = 0, bit6 = 0. 6: bit5 = 1, bit6 = 0. 7: bit5 = 0, bit6 = 1.
     *   		    8:
     *bit5 = 1, bit6 = 1  (default). all other bits are returned = 0
     ******************************************************************************/

    if ((data_size < 5) || (data_size > 8)) {
        data_size = 8;
    }

    // Control bit 5-6, WLEN
    return (((uint32_t)data_size - 5) << 5);
}

uint32_t lcrh_stopbits(uint8_t stop_bits) {
    /*****************************************************************************
     *   Input    :
     *   Output   :
     *   Function : sets bit 3 according to the wanted number of stop bits.
     *   		    1
     *stpobit:  bit3 = 0 (default). 2 stopbits: bit3 = 1. all other bits are
     *returned = 0
     ******************************************************************************/

    if (stop_bits == 2) {
        return (0x00000008); // return bit 3 = 1
    } else {
        return (0x00000000); // return all zeros
    }
}

uint32_t lcrh_parity(uint8_t parity) {
    /*****************************************************************************
     *   Input    :
     *   Output   :
     *   Function : sets bit 1, 2 and 7 to the wanted parity.
     *   		    'e':
     *00000110b. 'o':  00000010b. '0':  10000110b. '1':  10000010b. 'n':
     *00000000b. all other bits are returned = 0
     ******************************************************************************/

    uint32_t result;

    switch (parity) {
        case 'e':
            result = 0x00000006;
            break;
        case 'o':
            result = 0x00000002;
            break;
        case '0':
            result = 0x00000086;
            break;
        case '1':
            result = 0x00000082;
            break;
        case 'n':
            break;
        default:
            result = 0x00000000;
            break;
    }

    return result;
}

unsigned char uart0_rx_rdy() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    return (UART0_FR_R & UART_FR_RXFF);
}

uint8_t uart0_getc() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    return UART0_DR_R;
}

unsigned char uart0_tx_rdy() {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    return (UART0_FR_R & UART_FR_TXFE);
}

void uart0_putc(uint8_t ch) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    UART0_DR_R = ch;
}

void uart0_init(uint32_t baud_rate, uint8_t data_size, uint8_t stop_bits,
                uint8_t parity) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    if (SYSCTL_RCGC2_R != SYSCTL_RCGC2_GPIOA) {
        SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
    }

    if (SYSCTL_RCGC1_R != SYSCTL_RCGC1_UART0) {
        SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;
    }

    // Set PA0 and PA1 to alternativ function (uart0)
    GPIO_PORTA_AFSEL_R |= 0x00000003;

    // Set PA1 (uart0 tx) to output
    GPIO_PORTA_DIR_R |= 0x00000002;

    // Set PA0 (uart0 rx) to input
    GPIO_PORTA_DIR_R &= 0xFFFFFFFE;

    // Snable digital operation of PA0 and PA1
    GPIO_PORTA_DEN_R |= 0x00000003;

    // X-sys*64/(16*baudrate) = 16M*4/baudrate
    uint32_t BRD = 64000000 / baud_rate;

    UART0_IBRD_R = BRD / 64;
    UART0_FBRD_R = BRD & 0x0000003F;

    UART0_LCRH_R = lcrh_databits(data_size);
    UART0_LCRH_R += lcrh_stopbits(stop_bits);
    UART0_LCRH_R += lcrh_parity(parity);

    // Disable fifos
    UART0_LCRH_R &= 0xFFFFFFEF;

    // Enable fifos
    // UART0_LCRH_R |= 0x00000010;

    // Enable UART
    UART0_CTL_R |= (UART_CTL_UARTEN | UART_CTL_TXE);
}

void uart0_task(void *pvParameters) {
    /*****************************************************************************
     *   Function : See module specification (.h-file)
     *****************************************************************************/

    // 9600 baudrate, 8 data-bits, 1 stop-bit and no parity
    uart0_init(9600, 8, 1, 'n');

    while (1) {
        uint8_t data;
        if (xQueueReceive(xUARTQueue, &data, (TickType_t)10) == pdPASS) {
            xSemaphoreTake(xUARTSemaphore, (TickType_t)10);
            uart0_putc(data);
            xSemaphoreGive(xUARTSemaphore);
        }
    }
}

/****************************** End Of Module * *******************************/
