/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include "freertos/semphr.h"  // added by Sormaker
#include "esp_log.h"          // added by Sormaker

static const char *APP_TAG = "app_main";
SemaphoreHandle_t RecursiveMutexHandler;

void Task1(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    ESP_LOGI(pcName, "task1 begin!");

    while (1)
    {
        xSemaphoreTakeRecursive(RecursiveMutexHandler, portMAX_DELAY);
        ESP_LOGI(pcName, "task1 take A!");
        for (int i = 0; i < 10; i++)
        {
            ESP_LOGI(pcName, "task1 i = %d for A",i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        
        xSemaphoreTakeRecursive(RecursiveMutexHandler, portMAX_DELAY);
        ESP_LOGI(pcName, "task1 take B!");
        for (int i = 0; i < 10; i++)
        {
            ESP_LOGI(pcName, "task1 i = %d for B",i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        
        xSemaphoreGiveRecursive(RecursiveMutexHandler);
        ESP_LOGI(pcName, "task1-B give!");

        vTaskDelay(pdMS_TO_TICKS(3000));

        xSemaphoreGiveRecursive(RecursiveMutexHandler);
        ESP_LOGI(pcName, "task1-A give!");

        vTaskDelay(pdMS_TO_TICKS(3000));

        
    } 
}

void Task2(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    ESP_LOGI(pcName, "task2 begin!");
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        xSemaphoreTakeRecursive(RecursiveMutexHandler, portMAX_DELAY);
        ESP_LOGI(pcName, "task2 take!");
        for (int i = 0; i < 10; i++)
        {
            ESP_LOGI(pcName, "task2 i = %d",i);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        xSemaphoreGiveRecursive(RecursiveMutexHandler);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

void app_main(void)
{
    //* case: RecursiveMutexHandler Semaphores
    RecursiveMutexHandler = xSemaphoreCreateRecursiveMutex();
    if (RecursiveMutexHandler != NULL)
    {
        ESP_LOGI(APP_TAG,"RecursiveMutexHandler Create Successfully!");
        xSemaphoreGiveRecursive(RecursiveMutexHandler);
    }
    else
    {
        ESP_LOGE(APP_TAG, "RecursiveMutexHandler Create Fail!");
    }

    vTaskSuspendAll();

    xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, NULL, 0);

    xTaskResumeAll();
}
