    #include "nvs_flash.h"
    #include "driver/gpio.h"
    #include "rom/gpio.h"
    #include "esp_log.h"
    #include "esp_event.h"
    
    //Error handling should be made more robust later, figure out how to output messages into terminal with serial connection
    esp_err_t err_ret;    
    bool wifi_established = false;
    bool conn_established = false;

    void gen_init() {
    ESP_LOGI("INIT", "gen_init running");


    // Konfigurera GPIO_NUM_2
    gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);

    // Konfigurera GPIO_NUM_27
    gpio_pad_select_gpio(GPIO_NUM_27);
    gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT);

      gpio_pad_select_gpio(GPIO_NUM_16);
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_OUTPUT);

      gpio_pad_select_gpio(GPIO_NUM_17);
    gpio_set_direction(GPIO_NUM_17, GPIO_MODE_OUTPUT);

      gpio_pad_select_gpio(GPIO_NUM_18);
    gpio_set_direction(GPIO_NUM_18, GPIO_MODE_OUTPUT);

      gpio_pad_select_gpio(GPIO_NUM_19);
    gpio_set_direction(GPIO_NUM_19, GPIO_MODE_OUTPUT);

      gpio_pad_select_gpio(GPIO_NUM_4);
    gpio_set_direction(GPIO_NUM_4, GPIO_MODE_OUTPUT);

      gpio_pad_select_gpio(GPIO_NUM_5);
    gpio_set_direction(GPIO_NUM_5, GPIO_MODE_OUTPUT);

    // Om det behövs, skapa en standard händelseslinga
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_LOGI("INIT", "gen_init finished");
}


    void flash_init() {
        ESP_LOGI("INIT", "flash_init running");
        err_ret = nvs_flash_init();
        if (err_ret == ESP_ERR_NVS_NO_FREE_PAGES) {
            ESP_ERROR_CHECK(nvs_flash_erase());
            nvs_flash_init();
        }
        ESP_LOGI("INIT", "flash_init finished");
    }