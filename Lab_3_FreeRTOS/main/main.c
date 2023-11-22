/*
 * main.c
 *
 *  Created on: 12 thg 11, 2023
 *      Author: JACKWR
 */



#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "FreeRTOSConfig.h"
#include "driver/gpio.h"
#include "freertos/timers.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "esp_task_wdt.h"




void LowTask(void* pTaskID){
	while (1){
		long long int start = esp_timer_get_time();
		int count = 0;

		printf("The Idle task %s on core %d START at %lld \n", (char*)pTaskID,xPortGetCoreID(), start/100000);

		while (1){
			if (esp_timer_get_time() - start > 100000){
				start = esp_timer_get_time();
				count++;
				printf("The Idle task %s on core %d is CALLED at %lld, work remain: %d\n\n", (char*)pTaskID, xPortGetCoreID(), esp_timer_get_time()/100000, 10-count);

			}
			if (count >= 10){
				count = 0;
				vTaskDelay(10/portTICK_PERIOD_MS);
			}
		}

	}
}


void HighTask(){
	while (1){
		long long int start = esp_timer_get_time();
		int count = 0;

		printf("----> The Highest on core %d start at %lld \n", xPortGetCoreID(), start/100000);


		while (1){
			if (esp_timer_get_time() - start > 1000000){
				count++;
				start = esp_timer_get_time();
			}
			if (count >= 2) break;
		}


		printf("----> The Highest on core %d END at %lld \n\n", xPortGetCoreID(), esp_timer_get_time()/100000);
		vTaskDelay(10000/portTICK_PERIOD_MS);
	}
}



void app_main(void)
{


	//Button_RUN();


	//xTaskCreatePinnedToCore(OneSecRoutine_preEmp, "OneSecRoutine2", 2048, NULL, 2, NULL, 0);

	xTaskCreatePinnedToCore(LowTask, "LowTaskRoutine1", 2048, "1", 0, NULL, 0);
	xTaskCreatePinnedToCore(LowTask, "LowTaskRoutine2", 2048, "2", 0, NULL, 0);

	xTaskCreatePinnedToCore(HighTask, "HighTaskRoutine", 2048, NULL, 4, NULL, 0);
	//vTaskStartScheduler();
}
