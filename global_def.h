/*****************************************************************************
 * University of Southern Denmark
 * Embedded Programming (EMP)
 * Fabian Petrus
 *
 * MODULENAME.: glob_def.h
 *
 * PROJECT....: fabse_bank
 *
 * DESCRIPTION: Global variables/defines
 *
 * Change Log.:
 ******************************************************************************
 * Date    Id    Change
 * DDMMYY
 * --------------------
 * 300424  MoH    Module created
 *
 *****************************************************************************/

#ifndef _GLOB_DEF_H_
#define _GLOB_DEF_H_

/***************************** Include files *******************************/

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"

/*****************************    Defines    *******************************/

extern QueueHandle_t     xLCDQueue;
extern SemaphoreHandle_t xLCDSemaphore;

extern QueueHandle_t     xUARTQueue;
extern SemaphoreHandle_t xUARTSemaphore;

extern QueueHandle_t     xKeypadQueue;
extern SemaphoreHandle_t xKeypadSemaphore;

extern QueueHandle_t     xADCQueue;
extern SemaphoreHandle_t xADCSemaphore;

extern QueueHandle_t     xRotaryQueue;
extern SemaphoreHandle_t xRotarySemaphore;

extern QueueHandle_t     xSW1Queue;
extern SemaphoreHandle_t xSW1Semaphore;

extern QueueHandle_t     xSW2Queue;
extern SemaphoreHandle_t xSW2Semaphore;

// 0 = use systick for delay
// 1 = use timer1a for delay (can still use systick)
enum Delay { Timer1a_on = 0 };

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/

extern uint8_t  withdraw_type;
extern uint16_t money, password, withdraw_amount;

enum BankState {
    Welcome,
    Money,
    Password,
    Withdraw,
    Coinage,
    Print_money,

    TotalStates
};

extern int BankState; //

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/

#endif
