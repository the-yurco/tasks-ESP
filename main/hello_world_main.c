#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

// GLOBAL Task Handles set to NULL
TaskHandle_t firstTaskHandle = NULL;
TaskHandle_t secondTaskHandle = NULL;
TaskHandle_t thirdTaskHandle = NULL;

void firstTask(void *arg){
    while(1){
        printf("Task 1 - 1000m \n");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void secondTask(void *arg){
    while(1){
        printf("Task 2 - 100ms \n");
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void thirdTask(void *arg){
    while(1){
        printf("Task 3 - 3000ms \n");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

void app_main(void){
    xTaskCreate(firstTask, "First_Task", 4096, NULL, 1, &firstTaskHandle);
    xTaskCreate(secondTask, "Second_Task", 4096, NULL, 2, &secondTaskHandle);
    xTaskCreate(thirdTask, "Third_Task", 4096, NULL, 3, &thirdTaskHandle);
}