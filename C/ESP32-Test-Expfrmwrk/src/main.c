#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "lwip/sockets.h"

#include "init/init.h"
#include "wifi_module/wifi_module.h"
#include "tcp_client/tcp_client.h"

// Define commands based on the byte values you provided
#define CMD_FORWARD 16777216
#define CMD_BACKWARD 33554432
#define CMD_RIGHT 67108864
#define CMD_LEFT 50331648

int cmnd = 0;

void app_main() {
    ESP_LOGI("MAIN", "Started");
    gen_init();
    flash_init();
    init_wifi();
    start_wifi();

    // Initial GPIO setup
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT);

    // Initialize all GPIOs to a known state
    gpio_set_level(GPIO_NUM_16, 0);
    gpio_set_level(GPIO_NUM_17, 0);
    gpio_set_level(GPIO_NUM_18, 0);
    gpio_set_level(GPIO_NUM_19, 0);

    bool motors_running = false;  // Track motor status

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(250));

        if (!conn_established) {
            connectServer();  // Attempt to connect or reconnect
            continue;
        }

        // Read commands from server
        cmnd = readCmd();

        switch (cmnd) {
            case CMD_FORWARD:
                gpio_set_level(GPIO_NUM_18, 0); 
                gpio_set_level(GPIO_NUM_19, 0);
                gpio_set_level(GPIO_NUM_16, 0);
                gpio_set_level(GPIO_NUM_17, 0);
                sys_delay_ms(1000);
                gpio_set_level(GPIO_NUM_18, 1); 
                gpio_set_level(GPIO_NUM_19, 0);
                gpio_set_level(GPIO_NUM_16, 1);
                gpio_set_level(GPIO_NUM_17, 0);
                motors_running = true;
                ESP_LOGI("MOTOR_CONTROL", "Motors set to move forward");
                send_status_to_server(1);
                break;

            case CMD_BACKWARD:
                gpio_set_level(GPIO_NUM_18, 0); 
                gpio_set_level(GPIO_NUM_19, 0);
                gpio_set_level(GPIO_NUM_16, 0);
                gpio_set_level(GPIO_NUM_17, 0);
                sys_delay_ms(1000);
                gpio_set_level(GPIO_NUM_18, 0);
                gpio_set_level(GPIO_NUM_19, 1);
                gpio_set_level(GPIO_NUM_16, 0);
                gpio_set_level(GPIO_NUM_17, 1);
                motors_running = true;
                ESP_LOGI("MOTOR_CONTROL", "Motors set to move backward");
                send_status_to_server(2);
                break;

            case CMD_RIGHT:
                gpio_set_level(GPIO_NUM_18, 0); 
                gpio_set_level(GPIO_NUM_19, 0);
                gpio_set_level(GPIO_NUM_16, 0);
                gpio_set_level(GPIO_NUM_17, 0);
                sys_delay_ms(1000);
                gpio_set_level(GPIO_NUM_18, 1);
                gpio_set_level(GPIO_NUM_19, 0);
                gpio_set_level(GPIO_NUM_16, 0);
                gpio_set_level(GPIO_NUM_17, 0);
                motors_running = true;
                ESP_LOGI("MOTOR_CONTROL", "Motors set to turn right");
                send_status_to_server(3);
                break;

            case CMD_LEFT:
                gpio_set_level(GPIO_NUM_18, 0); 
                gpio_set_level(GPIO_NUM_19, 0);
                gpio_set_level(GPIO_NUM_16, 0);
                gpio_set_level(GPIO_NUM_17, 0);
                sys_delay_ms(1000);
                gpio_set_level(GPIO_NUM_18, 0);
                gpio_set_level(GPIO_NUM_19, 0);
                gpio_set_level(GPIO_NUM_16, 1);
                gpio_set_level(GPIO_NUM_17, 0);
                motors_running = true;
                ESP_LOGI("MOTOR_CONTROL", "Motors set to turn left");
                send_status_to_server(2);
                break;

            default:
                gpio_set_level(GPIO_NUM_16, 0);
                gpio_set_level(GPIO_NUM_17, 0);
                gpio_set_level(GPIO_NUM_18, 0);
                gpio_set_level(GPIO_NUM_19, 0);
                motors_running = false;
                ESP_LOGI("MOTOR_CONTROL", "Motors stopped");
                send_status_to_server(5);
                break;
        }
    }
}
