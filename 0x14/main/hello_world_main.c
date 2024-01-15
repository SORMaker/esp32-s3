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

#include "esp_log.h"          // added by Sormaker

TaskHandle_t xTask1;

void Task1(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    uint32_t ulNotifiedValue;
    while (1)
    {
        ESP_LOGI(pcName, "Task1 wait to be Notified!");

        xTaskNotifyWait(0x00, ULONG_MAX, &ulNotifiedValue, portMAX_DELAY); 
        if ((ulNotifiedValue & BIT0) != 0)
        {
            ESP_LOGI(pcName, "process Bit0 Event!");
        }
        else if ((ulNotifiedValue & BIT1) != 0)
        {
            ESP_LOGI(pcName ,"process Bit1 Event!");
        }
        else if ((ulNotifiedValue & BIT2) != 0)
        {
            ESP_LOGI(pcName,"process Bit2 Event!");
        }

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void Task2(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    vTaskDelay(pdMS_TO_TICKS(1000));
    while (1)
    {
        
        ESP_LOGI(pcName, "Task2 notify Task1 Bit0!");
        xTaskNotify(xTask1, BIT0, eSetValueWithOverwrite);
        vTaskDelay(pdMS_TO_TICKS(5000));

        ESP_LOGI(pcName, "Task2 notify Task1 Bit1!");
        xTaskNotify(xTask1, BIT1, eSetValueWithOverwrite);
        vTaskDelay(pdMS_TO_TICKS(5000));

        ESP_LOGI(pcName, "Task2 notify Task1 Bit2!");
        xTaskNotify(xTask1, BIT2, eSetValueWithOverwrite);
        vTaskDelay(pdMS_TO_TICKS(5000));

    }
}



void app_main(void)
{
    //* case: Notification Value
    vTaskSuspendAll();

    xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, &xTask1, 0);

    xTaskResumeAll();
}


