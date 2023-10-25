#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"

#define ESP_INR_FLAG_DEFAULT 0
#define LED_PIN  21
#define PUSH_BUTTON_PIN  19

TaskHandle_t ButtonTask = NULL;
TaskHandle_t LEDTask = NULL;

void button_task(void *arg) {
    while (1) {
        if (gpio_get_level(PUSH_BUTTON_PIN) == 0) {
            xTaskNotify(LEDTask, 0, eNoAction);
            vTaskDelay(pdMS_TO_TICKS(100)); // Debounce delay
            while (gpio_get_level(PUSH_BUTTON_PIN) == 0) {
                vTaskDelay(pdMS_TO_TICKS(10)); // Wait for button release
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10)); // Poll the button state every 10ms
    }
}

void LED_task(void *arg) {
    bool led_status = false;
    while (1) {
        if (xTaskNotifyWait(0, 0, NULL, portMAX_DELAY) == pdPASS) {
            led_status = !led_status;
            gpio_set_level(LED_PIN, led_status);
            printf("Button pressed for LED task!\n");
        }
    }
}

void app_main(void) {
    // Configure the push button in pull-up mode
    gpio_set_direction(PUSH_BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(PUSH_BUTTON_PIN, GPIO_PULLUP_ONLY);

    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0); // Initialize LED as OFF

    xTaskCreate(button_task, "Button_task", 4096, NULL, 2, &ButtonTask);
    xTaskCreate(LED_task, "LED_task", 4096, NULL, 1, &LEDTask);
}
