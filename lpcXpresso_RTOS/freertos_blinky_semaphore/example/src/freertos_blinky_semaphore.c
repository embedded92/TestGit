/*
 * @brief FreeRTOS Blinky example
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/
xSemaphoreHandle barrierSemaphore;
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Sets up system hardware */
static void prvSetupHardware(void) {
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED0 state is off */
	Board_LED_Set(0, 1);
	Board_LED_Set(1, 1);
	Board_LED_Set(2, 1);
}

/* LED1 toggle thread */
static void vLEDTask1(void *pvParameters) {
	bool LedState = false;

	while (1) {
		if (xSemaphoreTake(barrierSemaphore, 10) == pdTRUE) {

			Board_LED_Set(0, LedState);
			LedState = (bool) !LedState;

			vTaskDelay(500);
			xSemaphoreGive(barrierSemaphore);
		} else {
			vTaskDelay(10);
		}
	}
}

/* LED2 toggle thread */
static void vLEDTask2(void *pvParameters) {
	bool LedState = false;

	while (1) {
		if (xSemaphoreTake(barrierSemaphore, 10) == pdTRUE) {

			Board_LED_Set(1, LedState);
			LedState = (bool) !LedState;

			vTaskDelay(500);
			xSemaphoreGive(barrierSemaphore);
		} else {
			vTaskDelay(10);
		}
	}
}

/* LED3 toggle thread */
static void vLEDTask3(void *pvParameters) {
	bool LedState = false;

	while (1) {
		if (xSemaphoreTake(barrierSemaphore, 10) == pdTRUE) {
			Board_LED_Set(2, LedState);
			LedState = (bool) !LedState;

			vTaskDelay(500);
			xSemaphoreGive(barrierSemaphore);
		} else {
			vTaskDelay(10);
		}
	}
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/**
 * @brief main routine for FreeRTOS blinky example
 * @return Nothing, function should not exit
 */
int main(void) {
	prvSetupHardware();

	/* LED1 toggle thread */
	xTaskCreate(vLEDTask1, (signed char* ) "vTaskLed1",
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle *) NULL);

	/* LED2 toggle thread */
	xTaskCreate(vLEDTask2, (signed char* ) "vTaskLed2",
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle *) NULL);

	/* LED3 toggle thread */
	xTaskCreate(vLEDTask3, (signed char* ) "vTaskLed3",
			configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
			(xTaskHandle *) NULL);

	barrierSemaphore = xSemaphoreCreateCounting(2, 0);
	if (barrierSemaphore == NULL) {
		for (;;)
			;
	}
	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}

/**
 * @}
 */
