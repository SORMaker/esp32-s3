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

#include "freertos/event_groups.h"  // added by Sormaker
#include "esp_log.h"          // added by Sormaker

static const char *APP_TAG = "app_main";
EventGroupHandle_t EventGroupSyncHandle;

#define ALL_SYNC_BITS   (BIT(0) | BIT(1) | BIT(2))

void Task0(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        ESP_LOGI(pcName, "Task0 begin!");

        vTaskDelay(pdMS_TO_TICKS(1000));

        ESP_LOGI(pcName, "Task0 set Bit0!");
        xEventGroupSync(EventGroupSyncHandle, BIT(0), ALL_SYNC_BITS, portMAX_DELAY);

        ESP_LOGI(pcName, "Task0 Sync!");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void Task1(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        ESP_LOGI(pcName, "Task1 begin!");

        vTaskDelay(pdMS_TO_TICKS(3000));

        ESP_LOGI(pcName, "Task1 set Bit1!");
        xEventGroupSync(EventGroupSyncHandle, BIT(1), ALL_SYNC_BITS, portMAX_DELAY);

        ESP_LOGI(pcName, "Task1 Sync!");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

void Task2(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        ESP_LOGI(pcName, "Task2 begin!");

        vTaskDelay(pdMS_TO_TICKS(5000));

        ESP_LOGI(pcName, "Task2 set Bit2!");
        xEventGroupSync(EventGroupSyncHandle, BIT(2), ALL_SYNC_BITS, portMAX_DELAY);

        ESP_LOGI(pcName, "Task2 Sync!");
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}



void app_main(void)
{
    //* case: Event Group Sync
    EventGroupSyncHandle = xEventGroupCreate();
    if (EventGroupSyncHandle != NULL)
    {
        ESP_LOGI(APP_TAG,"EventGroupSyncHandle Create Successfully!");


        vTaskSuspendAll();

        xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 1, NULL, 0);
        xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, NULL, 0);
        xTaskCreatePinnedToCore(Task0, "Task0", 1024*5, NULL, 1, NULL, 0);

        xTaskResumeAll();
    }
    else
    {
        ESP_LOGE(APP_TAG, "EventGroupSyncHandle Create Fail!");
    }

}
