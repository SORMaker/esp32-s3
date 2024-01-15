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

TaskHandle_t    xTask1;

void Task1(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        ESP_LOGI(pcName, "Task1 wait to be Notified!");

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        ESP_LOGI(pcName, "Task1 got Notification!");

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void Task2(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(5000));

        ESP_LOGI(pcName, "Task2 notifi Task1!");

        xTaskNotifyGive(xTask1);
    }
}



void app_main(void)
{
    //* case: Notification Sync
    vTaskSuspendAll();

    xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, &xTask1, 0);

    xTaskResumeAll();
}


