
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
	xTaskCreate(OneSecRoutine, "OneSecRoutine", 2048, NULL, 10, NULL);

}
