/* BSD Socket API Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include <sys/param.h>
#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "esp_timer.h"

/*USER INCLUDE*/
#include "wifi.h"
#include "dht11.h"
//#include "DHT22.h"


#define PORT                        CONFIG_EXAMPLE_PORT
#define KEEPALIVE_IDLE              CONFIG_EXAMPLE_KEEPALIVE_IDLE
#define KEEPALIVE_INTERVAL          CONFIG_EXAMPLE_KEEPALIVE_INTERVAL
#define KEEPALIVE_COUNT             CONFIG_EXAMPLE_KEEPALIVE_COUNT



/*------Global QUEUE---------*/
QueueHandle_t cmdQueue;



/*---------------------Task Handler-----------------------*/

void LED_INIT(int pin){
	gpio_config_t io_conf;
	io_conf.pin_bit_mask = 1<< pin;							//*** config GPIO
	io_conf.mode = GPIO_MODE_INPUT_OUTPUT;
	io_conf.pull_up_en = 0;
	io_conf.pull_down_en = 0;
	io_conf.intr_type = GPIO_INTR_DISABLE;							// vo hieu hoa interrupt
	gpio_config(&io_conf);
}





void LED_Handler(){

	LED_INIT(18);
	LED_INIT(19);
	LED_INIT(21);


	char rxBuffer[50];
	while (1){
		if (xQueuePeek(cmdQueue, &rxBuffer, portMAX_DELAY )){
			//Parse the message
			char *str[3]={0,0,0};
			char *token = strtok(rxBuffer, ":");

			int inValidToken = 0;
			for (int i = 0; i < 3; i++){
				if (!token){
					inValidToken = 1;
					break;
				}
				str[i] = token;
				token = strtok(NULL, ":");
			}
			if (inValidToken){
				vTaskDelay(pdMS_TO_TICKS(100));
				continue;
			}

			// This is LED task
			if(strcmp(str[1], "LED") == 0){



				xQueueReceive(cmdQueue, &rxBuffer, portMAX_DELAY );

				ESP_LOGI("LED", "Receive from queue %d bytes: %s", sizeof(rxBuffer), rxBuffer);

				if (strcmp(str[2], "red") == 0){
					gpio_set_level(21, 1 -  gpio_get_level(21));
				}
				else if (strcmp(str[2], "yellow") == 0){
					gpio_set_level(19, 1 -  gpio_get_level(19));
				}
				else if (strcmp(str[2], "green") == 0){
					gpio_set_level(18, 1 -  gpio_get_level(18));
				}
				else if (strcmp(str[2], "all") == 0){
					int temp = gpio_get_level(21);
					gpio_set_level(21, 1-temp);
					gpio_set_level(19, 1-temp);
					gpio_set_level(18, 1-temp);
				}

			}

			else vTaskDelay(pdMS_TO_TICKS(10));
		}

		else vTaskDelay(pdMS_TO_TICKS(100));

	}
}



void DHT11_Handler(){

	DHT11_init(22);
	char rxBuffer[50];
	while (1){
		if (xQueuePeek(cmdQueue, &rxBuffer, portMAX_DELAY )){
			//Parse the message
			char *str[3]={0,0,0};
			char *token = strtok(rxBuffer, ":");

			int inValidToken = 0;
			for (int i = 0; i < 3; i++){
				if (!token){
					inValidToken = 1;
					break;
				}
				str[i] = token;
				token = strtok(NULL, ":");
			}
			if (inValidToken){
				vTaskDelay(pdMS_TO_TICKS(100));
				continue;
			}

			// This is LED task
			if(strcmp(str[1], "DHT") == 0){
				xQueueReceive(cmdQueue, &rxBuffer, portMAX_DELAY );

				ESP_LOGI("DHT11", "Receive from queue %d bytes: %s", sizeof(rxBuffer), rxBuffer);

				if (strcmp(str[2], "temp") == 0){
					ESP_LOGE("DHT","Temperature %d degC\n\n", DHT11_read().temperature);
				}
				else if (strcmp(str[2], "humi") == 0){
					ESP_LOGE("DHT","Humidity %d %%\n", DHT11_read().humidity);

				}
				else if (strcmp(str[2], "all") == 0){
					ESP_LOGE("DHT","Temperature %d degC, Humidity %d %%\n\n", DHT11_read().temperature, DHT11_read().humidity );
				}
			}
			else vTaskDelay(pdMS_TO_TICKS(10));
		}

		else vTaskDelay(pdMS_TO_TICKS(100));

	}
}


void Cleaning_Handler(){
	char rxBuffer[50];
	while (1){
		if (xQueueReceive(cmdQueue, &rxBuffer, portMAX_DELAY )){
			//Parse the message
			char *str[3]={0,0,0};
			char *token = strtok(rxBuffer, ":");
			int inValidToken = 0;
			for (int i = 0; i < 3; i++){
				if (!token){
					inValidToken = 1;
					break;
				}
				str[i] = token;
				token = strtok(NULL, ":");
			}
			if (inValidToken){
				ESP_LOGW("IDLE","CLEANING... %s\n", rxBuffer);
				vTaskDelay(pdMS_TO_TICKS(50));
				continue;
			}

			// This is LED task
			if(strcmp(str[1], "CLN") == 0){

				ESP_LOGI("IDLE", "Receive from queue %d bytes: %s", sizeof(rxBuffer), rxBuffer);

				if (strcmp(str[2], "clean") == 0){
					ESP_LOGE("IDLE","CLEANING ALL REQUEST....\n\n");
					while (uxQueueMessagesWaiting(cmdQueue)){
						xQueueReceive(cmdQueue, &rxBuffer, portMAX_DELAY );
					}
				}
				else if (strcmp(str[2], "count") == 0){

					long long int start = esp_timer_get_time();
					while (esp_timer_get_time() - start < 3000000){
					}

					ESP_LOGE("IDLE","WAITING ITEMS %d\n", uxQueueMessagesWaiting(cmdQueue));
				}
				else ESP_LOGW("IDLE","CLEANING... %s\n", rxBuffer);
			}
			else ESP_LOGW("IDLE","CLEANING... %s\n", rxBuffer);

			vTaskDelay(pdMS_TO_TICKS(1000));
		}

		else vTaskDelay(pdMS_TO_TICKS(100));

	}
}





/*--------------------TCP FUNCTION---------------------------*/

// TCP server running first
//		---> Accept connection
//			 ----> Recv controller
//


const char *TAG = "TCPv4_Task";


void Recv_Controller(const int sock)
{
    int len;
    char rx_buffer[50];


    do {
        len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
        if (len < 0) {
            ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
        } else if (len == 0) {
            ESP_LOGW(TAG, "Connection closed");
        } else {
            rx_buffer[len] = 0; // Null-terminate whatever is received and treat it like a string
            ESP_LOGI(TAG, "Received %d bytes: %s", len, rx_buffer);

            // send() can return less bytes than supplied length.
            // Walk-around for robust implementation.
//            int to_write = len;
//            while (to_write > 0) {
//                int written = send(sock, rx_buffer + (len - to_write), to_write, 0);
//                if (written < 0) {
//                    ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
//                }
//                to_write -= written;
//            }

            if (len > 3){
            	char cmd_check[4];
            	strncpy(cmd_check, rx_buffer, 3);
                cmd_check[3] = '\0';
                if (strcmp(cmd_check,"cmd") == 0){
                	xQueueSend(cmdQueue, (void*)rx_buffer, portMAX_DELAY);
            	    ESP_LOGI("QUEUE", "Send to queue %d bytes: %s", len, rx_buffer);
            	}
            }



        }
    } while (len > 0);
}





void tcp_server_task(void *pvParameters)
{
    char addr_str[128];
    int addr_family = (int)pvParameters;
    int ip_protocol = 0;
    int keepAlive = 1;
    int keepIdle = KEEPALIVE_IDLE;
    int keepInterval = KEEPALIVE_INTERVAL;
    int keepCount = KEEPALIVE_COUNT;
    struct sockaddr_storage dest_addr;

    if (addr_family == AF_INET) {
        struct sockaddr_in *dest_addr_ip4 = (struct sockaddr_in *)&dest_addr;
        dest_addr_ip4->sin_addr.s_addr = htonl(INADDR_ANY);
        dest_addr_ip4->sin_family = AF_INET;
        dest_addr_ip4->sin_port = htons(PORT);
        ip_protocol = IPPROTO_IP;
    }

    int listen_sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (listen_sock < 0) {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        vTaskDelete(NULL);
        return;
    }
    int opt = 1;
    setsockopt(listen_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    ESP_LOGI(TAG, "Socket created");

    int err = bind(listen_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(TAG, "Socket unable to bind: errno %d", errno);
        ESP_LOGE(TAG, "IPPROTO: %d", addr_family);
        goto CLEAN_UP;
    }
    ESP_LOGI(TAG, "Socket bound, port %d", PORT);

    err = listen(listen_sock, 1);
    if (err != 0) {
        ESP_LOGE(TAG, "Error occurred during listen: errno %d", errno);
        goto CLEAN_UP;
    }

    while (1) {

        ESP_LOGI(TAG, "Socket listening");

        struct sockaddr_storage source_addr; // Large enough for both IPv4 or IPv6
        socklen_t addr_len = sizeof(source_addr);


        int sock = accept(listen_sock, (struct sockaddr *)&source_addr, &addr_len);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to accept connection: errno %d", errno);
            break;
        }

        // Set tcp keepalive option
        setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &keepAlive, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(int));
        setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(int));
        // Convert ip address to string
        if (source_addr.ss_family == PF_INET) {
            inet_ntoa_r(((struct sockaddr_in *)&source_addr)->sin_addr, addr_str, sizeof(addr_str) - 1);
        }



        ESP_LOGI(TAG, "Socket accepted ip address: %s", addr_str);



        Recv_Controller(sock);

        shutdown(sock, 0);
        close(sock);
    }



CLEAN_UP:
    close(listen_sock);
    vTaskDelete(NULL);
}




void app_main(void)
{

	//wifi_connect("Vjppro", "1111.1111");
	wifi_connect("Jackwrion", "16122002");


	while (!WIFI_FLAG){
		vTaskDelay(pdMS_TO_TICKS(1000));
	}

	cmdQueue = xQueueCreate(10, 50);
    xTaskCreate(tcp_server_task, "tcp_server", 4096, (void*)AF_INET, 5, NULL);


    xTaskCreatePinnedToCore(LED_Handler, "LED_handler", 2048, NULL , 4, NULL, 1);
    xTaskCreatePinnedToCore(DHT11_Handler, "DHT11_handler", 2048, NULL , 3, NULL, 1);
    xTaskCreatePinnedToCore(Cleaning_Handler, "Cleaning_Handler", 2048, NULL , 0, NULL, 1);

}
