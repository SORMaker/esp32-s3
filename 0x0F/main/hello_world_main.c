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
SemaphoreHandle_t MutexHandler;

void Task1(void *pvParam)
{
    BaseType_t iRet;
    const char *pcName = pcTaskGetName(NULL);
    ESP_LOGI(pcName, "task1 begin!");

    while (1)
    {
        iRet = xSemaphoreTake(MutexHandler, 1000);
        if (iRet == pdPASS)
        {
            ESP_LOGI(pcName, "task1 take!");
            for (int i = 0; i < 50; i++)
            {
                ESP_LOGI(pcName, "task1 i = %d",i);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            
            xSemaphoreGive(MutexHandler);
            ESP_LOGI(pcName, "task1 give!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            ESP_LOGI(pcName, "task1 didn't take!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        
    } 
}

void Task2(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    ESP_LOGI(pcName, "task2 begin!");
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1);
}

void Task3(void *pvParam)
{
    BaseType_t iRet;
    const char *pcName = pcTaskGetName(NULL);
    ESP_LOGI(pcName,"task3 begin!");
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        iRet = xSemaphoreTake(MutexHandler, 1000);
        if (iRet == pdPASS)
        {
            ESP_LOGI(pcName, "task3 take!");
            for (int i = 0; i < 10; i++)
            {
                ESP_LOGI(pcName, "task3 i = %d",i);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }

            xSemaphoreGive(MutexHandler);
            ESP_LOGI(pcName, "task3 give!");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        else
        {
            ESP_LOGI(pcName, "task3 didn't take!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        
    } 
}


void app_main(void)
{
    //* case: Mutex Semaphores
    // MutexHandler = xSemaphoreCreateBinary();
    MutexHandler = xSemaphoreCreateMutex();
    if (MutexHandler != NULL)
    {
        ESP_LOGI(APP_TAG,"Mutex Create Successfully!");
        xSemaphoreGive(MutexHandler);
    }
    else
    {
        ESP_LOGE(APP_TAG, "Mutex Create Fail!");
    }

    vTaskSuspendAll();

    xTaskCreatePinnedToCore(Task3, "Task3", 1024*5, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 2, NULL, 0);
    xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, NULL, 0);

    xTaskResumeAll();
}
