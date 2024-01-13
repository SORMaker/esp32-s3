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
SemaphoreHandle_t semHandler;

int iCount = 0;

void Task1(void *pvParam)
{
    BaseType_t iRet;
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        iRet = xSemaphoreTake(semHandler, portMAX_DELAY);
        if (iRet == pdPASS)
        {
            for (int i = 0; i < 10; i++)
            {
                iCount++;
                ESP_LOGI(pcName,"iCount = %d",iCount);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            xSemaphoreGive(semHandler);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            ESP_LOGI(pcName, "Task1 didn't take!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        

    } 
}

void Task2(void *pvParam)
{
    BaseType_t iRet;
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        iRet = xSemaphoreTake(semHandler, portMAX_DELAY);
        if (iRet == pdPASS)
        {
            for (int i = 0; i < 10; i++)
            {
                iCount++;
                ESP_LOGI(pcName,"iCount = %d",iCount);
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
            xSemaphoreGive(semHandler);
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            ESP_LOGI(pcName, "Task2 didn't take!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        

    } 
}

void app_main(void)
{
    //* case: Binary Semaphore
    semHandler = xSemaphoreCreateBinary();
    if (semHandler != NULL)
    {
        ESP_LOGI(APP_TAG, "Binary Semaphore Create Successfully!");
        xSemaphoreGive(semHandler);
    }
    else
    {
        ESP_LOGE(APP_TAG, "Binary Semaphore Create Fail!");
    }
    
    vTaskSuspendAll();

    xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 1, NULL, 0);

    xTaskResumeAll();
    // xTaskCreate(Task1, "Task1", 1024*5, NULL, 1, NULL);
    // xTaskCreate(Task2, "Task2", 1024*5, NULL, 2, NULL);

}
