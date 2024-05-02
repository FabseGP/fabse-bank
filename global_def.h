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

extern SemaphoreHandle_t xBankStateSemaphore;
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
extern int money;
extern int readyCheck; // Password control, withdraw control, money check

enum BankState {
    Welcome,
    Money,
    Password,
    Withdraw,

    TotalStates
};

extern int BankState; //
/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/

#endif
