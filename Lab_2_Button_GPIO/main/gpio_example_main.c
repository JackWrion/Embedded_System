/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
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

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output (ESP32C2/ESP32H2 uses GPIO8 as the second output pin)
 * GPIO19: output (ESP32C2/ESP32H2 uses GPIO9 as the second output pin)
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Note. These are the default GPIO pins to be used in the example. You can
 * change IO pins in menuconfig.
 *
 * Test:
 * Connect GPIO18(8) with GPIO4
 * Connect GPIO19(9) with GPIO5
 * Generate pulses on GPIO18(8)/19(9), that triggers interrupt on GPIO4/5
 *
 */

#define ESP_INTR_FLAG_DEFAULT 0

static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void* arg)
{
    uint32_t io_num;
    long long int timer_hold = 0;
    int button_state = 1;

    while(1) {

    	if (button_state == 0){
    		long long int diff = esp_timer_get_time() - timer_hold;

    		if (diff > 2000000){
    			printf("GPIO[%"PRIu32"] is HOLDING .....\n", io_num);

    		}

    	}

        if(xQueueReceive(gpio_evt_queue, &io_num, 1000 / portTICK_PERIOD_MS)) {
        	button_state = gpio_get_level(io_num);
			gpio_set_level(GPIO_NUM_2, 1- button_state );
            printf("GPIO[%"PRIu32"] is: %d\n", io_num, button_state);

            if (button_state == 0){
            	timer_hold = esp_timer_get_time();

            }
        }


        vTaskDelay(100 / portTICK_PERIOD_MS);

    }
}










static void LED_INIT(void){
	gpio_config_t io_conf;
	io_conf.pin_bit_mask = 1<< GPIO_NUM_2;							//*** config GPIO
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pull_up_en = 0;
	io_conf.pull_down_en = 0;
	io_conf.intr_type = GPIO_INTR_DISABLE;							// vo hieu hoa interrupt
	gpio_config(&io_conf);
}

static void BUTTON_INIT(int button_num){
	gpio_config_t io_conf;
	io_conf.pin_bit_mask = 1<< button_num;							//*** config GPIO
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_up_en = 1;
	io_conf.pull_down_en = 0;
	io_conf.intr_type = GPIO_INTR_ANYEDGE;							// vo hieu hoa interrupt
	gpio_config(&io_conf);
}






//create queue
//Install ISR --> ISR_HANLDER_ADD(button, handler_func) --> handler_func{send to queue}
//Vtask{get from queue} --> Do something


void app_main(void)
{

	//Init GPIO
	LED_INIT();
	BUTTON_INIT(0);


    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);
    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_0, gpio_isr_handler, (void*) GPIO_NUM_0);
    //hook isr handler for specific gpio pin


    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());

}
