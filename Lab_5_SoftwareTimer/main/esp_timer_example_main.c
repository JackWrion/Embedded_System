/* esp_timer (high resolution timer) example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"
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
#include "esp_system.h"





int timerID_1 = 1;
int timerID_2 = 2;

int counter_1 = 10;
int counter_2 = 5;



// 1. Get the ID of this timer called it.
// 2. Print info and Decrease Counter_ID.
// 3. Delete this timer if Counter_ID == 0;
void PrintingCallBack(TimerHandle_t pxTimer ){

	int id = (int)pvTimerGetTimerID( pxTimer );
	long long int time = esp_timer_get_time() / 1000000;


	if (id == 1){
		ESP_LOGI("Timer 1:", "AHIHI   at %llu  no.%d\n", time ,  11 -counter_1);
		counter_1--;
		if (counter_1 == 0) xTimerStop(pxTimer,0);
	}


	else {
		ESP_LOGW("Timer 2:", "IHAHA   at %llu  no.%d\n", time , 6- counter_2);
		counter_2--;
		if (counter_2 == 0) xTimerStop(pxTimer,0);
	}

}





void app_main(void)
{

	// Create 2 timers. Each will drive only one Call_back_function


	TimerHandle_t timer1 = xTimerCreate("Timer 1:", pdMS_TO_TICKS(2000), pdTRUE, (void*)timerID_1,  PrintingCallBack);
	TimerHandle_t timer2 = xTimerCreate("Timer 2:", pdMS_TO_TICKS(3000), pdTRUE, (void*)timerID_2,  PrintingCallBack);

	vTaskDelay(pdMS_TO_TICKS(2000));

	xTimerStart(timer1, 0);
	xTimerStart(timer2, 0);
}



