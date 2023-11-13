#include "ButtonLIB.h"


static QueueHandle_t gpio_evt_queue = NULL;

void IRAM_ATTR gpio_isr_handler(void* arg){
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}




void gpio_task_example(void* arg)
{
    uint32_t io_num;
    long long int timer_hold = -99999999;
    long long int timer_double_click = 0;
    int button_state = 1;

    while(1) {

    	if (button_state == 0){
    		long long int diff = esp_timer_get_time() - timer_hold;
    		if (diff > 2000000){
    			//TODO when HOLDING button
    			printf("GPIO[%"PRIu32"] is HOLDING .....\n", io_num);
    		}
    	}


        if(xQueueReceive(gpio_evt_queue, &io_num, 1000 / portTICK_PERIOD_MS)) {
        	button_state = gpio_get_level(io_num);
			gpio_set_level(GPIO_NUM_2, 1- button_state );
            printf("GPIO[%"PRIu32"] is: %d\n", io_num, button_state);

            if (button_state == 1){
            	timer_double_click = esp_timer_get_time();
            }
            if (button_state == 0){
            	//TODO when CLICK
            	timer_hold = esp_timer_get_time();
            	printf("GPIO[%"PRIu32"] is CLICK ..... ESP32\n", io_num);
            	if (timer_hold - timer_double_click < 500000){
            		//TODO when DOUBLE CLICK
            		printf("GPIO[%"PRIu32"] is DOUBLE CLICK .....\n", io_num);
            	}
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}










void LED_INIT(void){
	gpio_config_t io_conf;
	io_conf.pin_bit_mask = 1<< GPIO_NUM_2;							//*** config GPIO
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pull_up_en = 0;
	io_conf.pull_down_en = 0;
	io_conf.intr_type = GPIO_INTR_DISABLE;							// vo hieu hoa interrupt
	gpio_config(&io_conf);
}

void BUTTON_INIT(int button_num){
	gpio_config_t io_conf;
	io_conf.pin_bit_mask = 1<< button_num;							//*** config GPIO
	io_conf.mode = GPIO_MODE_INPUT;
	io_conf.pull_up_en = 1;
	io_conf.pull_down_en = 0;
	io_conf.intr_type = GPIO_INTR_ANYEDGE;							// vo hieu hoa interrupt
	gpio_config(&io_conf);
}








//1. Create queue
//2. Install ISR --> ISR_HANLDER_ADD(button, handler_func) --> handler_func{put to queue}
//3. Vtask{get from queue} --> Do something


void Button_RUN(void)
{

	//Init GPIO
	LED_INIT();
	BUTTON_INIT(0);

    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_NUM_0, gpio_isr_handler, (void*) GPIO_NUM_0);


    //start gpio task, which will handle all signal of GPIO: click, double click, holding
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 10, NULL);

    printf("BUTTON RUNNING SUCCESSFULLY !!!\n\n");


}
