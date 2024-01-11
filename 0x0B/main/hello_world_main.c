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

#include "freertos/queue.h"   // added by Sormaker
#include "esp_log.h"          // added by Sormaker

static const char *APP_TAG = "app_main";
static const char *WRITE_TASK_TAG = "writeTask";

void writeTask(void *pvParam)
{
    QueueHandle_t MailBox = (QueueHandle_t)pvParam;
    BaseType_t xStatus;
    int i = 0;
    while (1)
    {
        xStatus = xQueueOverwrite(MailBox, &i);
        if (xStatus == pdPASS)
        {
            ESP_LOGI(WRITE_TASK_TAG,"Send1 successfully!");
        }
        i++;
        vTaskDelay(6000 / portTICK_PERIOD_MS);
    }
}

void readTask(void *pvParam)
{
    QueueHandle_t MailBox = (QueueHandle_t)pvParam;
    BaseType_t xStatus;
    int j = 0;
    char *pcName = pcTaskGetName(NULL);
    while (1)
    {
        xStatus = xQueuePeek(MailBox, &j, 0);
        if (xStatus == pdPASS)
        {
            ESP_LOGI(pcName,"read j = %d!",j);
        }
        else if(xStatus == errQUEUE_EMPTY)
        {
            ESP_LOGW(pcName,"Queue is empty!");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


void app_main(void)
{
    //* case: Queue MailBox
    QueueHandle_t MailBox = NULL;

    MailBox = xQueueCreate(1, sizeof(int));


    if (MailBox != NULL)
    {
        ESP_LOGI(APP_TAG,"Create Queue successfully!");
        //* High Priority Task should be created first!
        xTaskCreate(readTask, "readTask1", 1024 * 5, (void *)MailBox, 2, NULL);
        xTaskCreate(readTask, "readTask2", 1024 * 5, (void *)MailBox, 2, NULL);
        xTaskCreate(readTask, "readTask3", 1024 * 5, (void *)MailBox, 2, NULL);
        xTaskCreate(readTask, "readTask4", 1024 * 5, (void *)MailBox, 2, NULL);
        xTaskCreate(writeTask, "writeTask", 1024 * 5, (void *)MailBox, 1, NULL);
    }
    else
    {
        ESP_LOGE(APP_TAG,"Create Queue fail!");
    }
}
