
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "freertos/timers.h"
#include "esp_timer.h"
#include "esp_system.h"
#include "esp_task_wdt.h"
/* Personal INCLUDE*/
#include "ButtonLIB.h"



void OneSecRoutine(){
	while (1){
		printf("1s: 2012215 and 2014550\n");
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}


void app_main(void)
{

	Button_RUN();
	xTaskCreatePinnedToCore(OneSecRoutine, "OneSecRoutine", 2048, NULL, 9, NULL, 1);

}
