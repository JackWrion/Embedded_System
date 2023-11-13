/*
This is Button Handler Library

I. How to install:
	1. Copy "ButtonLIB.c", "ButtonLIB.h"  to the same hierarchical with "main.c"
    2. Adjust CMakeLists.txt, adding "ButtonLIB.c", it will look like this:..

    	idf_component_register(SRCS "gpio_example_main.c" "ButtonLIB.c"
                    			INCLUDE_DIRS ".")


II. How to use
1. Config your Button Pin at BUTTON_INIT(int button_num)
2. Config your LED Pin at LED_INIT()

3. I have completed all features: Click, DoubleClick, Holding... in Button_RUN()
   Wanna some config features, go for it after "TODO"



*/

#ifndef BUTTONLIB_H
#define BUTTONLIB_H


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



#define ESP_INTR_FLAG_DEFAULT 0

void IRAM_ATTR gpio_isr_handler(void* arg);

void gpio_task_example(void* arg);


void LED_INIT(void);

void BUTTON_INIT(int button_num);


void Button_RUN(void);

#endif
