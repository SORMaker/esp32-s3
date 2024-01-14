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
EventGroupHandle_t EventGroupHandle;



void Task1(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        ESP_LOGI(pcName, "Task1 wait Bits");
        //* case: BIT0 or BIT4
        // xEventGroupWaitBits(EventGroupHandle, BIT(0) | BIT(4), pdTRUE, pdFALSE, portMAX_DELAY);
        // ESP_LOGI(pcName, "Bit0 or Bit4 is set!");
        // ESP_LOGI(pcName, "Bit0 or Bit4 is reset");

        //* case: BIT0 and BIT4
        xEventGroupWaitBits(EventGroupHandle, BIT(0) | BIT(4), pdTRUE, pdTRUE, portMAX_DELAY);
        ESP_LOGI(pcName, "Bit0 and Bit4 have been set!");
        vTaskDelay(pdMS_TO_TICKS(1000));
    } 
}

void Task2(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    vTaskDelay(pdMS_TO_TICKS(1000));

    while (1)
    {
        // xEventGroupSetBits(EventGroupHandle, BIT(0));
        // ESP_LOGI(pcName, "BIT0 is set!");
        // vTaskDelay(pdMS_TO_TICKS(5000));

        // xEventGroupSetBits(EventGroupHandle, BIT(4));
        // ESP_LOGI(pcName, "BIT4 is set!");
        // vTaskDelay(pdMS_TO_TICKS(5000));

        xEventGroupSetBits(EventGroupHandle, BIT(0) | BIT(4));
        ESP_LOGI(pcName, "BIT0 and BIT4 are set in Task2!");
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
    
}

void app_main(void)
{
    //* case: Event Group Wait
    EventGroupHandle = xEventGroupCreate();
    if (EventGroupHandle != NULL)
    {
        ESP_LOGI(APP_TAG,"EventGroupHandle Create Successfully!");


        vTaskSuspendAll();

        xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 1, NULL, 0);
        xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, NULL, 0);

        xTaskResumeAll();
    }
    else
    {
        ESP_LOGE(APP_TAG, "EventGroupHandle Create Fail!");
    }

}
