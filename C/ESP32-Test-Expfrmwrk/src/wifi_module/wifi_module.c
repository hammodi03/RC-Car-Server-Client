#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/sockets.h"
#include "init/init.h"
#include "tcp_client/tcp_client.h"
#include "driver/gpio.h"
#include "rom/gpio.h"

//#define wifi_ssid "TN-RR8102"
//#define wifi_pass "abusneineh"

//#define wifi_ssid "Hammodeh"
//#define wifi_pass "987654321"

#define wifi_ssid "S23 Ultra"
#define wifi_pass "Zmqrej03!"


//Add logging for all events
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    switch (event_id) {
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            ESP_LOGI("WIFI", "Attempting connection");
            break;
        case WIFI_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            gpio_set_level(GPIO_NUM_2, 0);
            ESP_LOGI("WIFI", "Disconnected, reconnecting");
            //Log disconnect and reconnection attempt
            //Will need to make it so the ball stops dead in its tracks if connection to remote control is lost
            break;
        case IP_EVENT_STA_GOT_IP:
            ESP_LOGI("WIFI", "IP acquired");
            connectServer();
            break;
        }
}

void init_wifi() {
    ESP_LOGI("WIFI", "init_wifi running");
    esp_netif_init();
    esp_netif_create_default_wifi_sta();

    // Might want to make a full cfg later instead of using default one
    wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_cfg));
    
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    //Do we need to handle below?
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    ESP_LOGI("WIFI", "init_wifi finished");
}

void start_wifi() {
    ESP_LOGI("WIFI", "start_wifi running");
    wifi_config_t wifi_cfg = {
        .sta = {
            .ssid = wifi_ssid,
            .password = wifi_pass,
            .bssid_set = false
        }
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_cfg));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI("WIFI", "start_wifi finished");
}

void stop_wifi() {
    ESP_LOGI("WIFI", "Stopping WiFi");

    // Disconnect WiFi (if connected)
    ESP_ERROR_CHECK(esp_wifi_disconnect());
    
    // Stop the WiFi driver (deactivates the WiFi interface)
    ESP_ERROR_CHECK(esp_wifi_stop());
    
    // Unregister WiFi event handlers to prevent callbacks during or after deinitialization
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
    
    // Deinitialize the WiFi module. This is typically done when WiFi is no longer needed at all.
    ESP_ERROR_CHECK(esp_wifi_deinit());
    
    // Optionally, free any dynamic memory or resources allocated in related WiFi or network setups if any
    // This might include deallocating instances of esp_netif_t if created dynamically

    ESP_LOGI("WIFI", "WiFi stopped");
}

