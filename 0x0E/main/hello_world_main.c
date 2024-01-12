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

void carInTask(void *pvParam)
{
    UBaseType_t semCount;
    BaseType_t iCount;
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        semCount = uxSemaphoreGetCount(semHandler);
        ESP_LOGI(pcName,"semLeft = %d", semCount);
        iCount = xSemaphoreTake(semHandler, 0);
        if (iCount == pdPASS)
            ESP_LOGI(pcName,"Car in!");
        else
            ESP_LOGI(pcName,"Full!");
        vTaskDelay(pdMS_TO_TICKS(1000));
    } 
}

void carOutTask(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));
        xSemaphoreTake(semHandler, 0);
        ESP_LOGI(pcName,"Car Out!");
        xSemaphoreGive(semHandler);
    } 
}

void app_main(void)
{
    //* case: Count Semaphores
    semHandler = xSemaphoreCreateCounting(5,5);
    xSemaphoreGive(semHandler);
    ESP_LOGI(APP_TAG,"Semaphores Create Successfully!");
    xTaskCreate(carInTask, "carInTask", 1024*5, NULL, 1, NULL);
    xTaskCreate(carOutTask, "carOutTask", 1024*5, NULL, 1, NULL);
}
