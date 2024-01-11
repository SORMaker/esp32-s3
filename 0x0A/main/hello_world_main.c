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
static const char *SEND_TASK1_TAG = "sendTask1";
static const char *SEND_TASK2_TAG = "sendTask2";
static const char *REC_TASK_TAG = "recTask";


void sendTask1(void *pvParam)
{
    QueueHandle_t sendQHandle = (QueueHandle_t)pvParam;
    BaseType_t xStatus;
    int i = 1;
    while (1)
    {
        xStatus = xQueueSend(sendQHandle, &i, 0);
        if (xStatus == pdPASS)
        {
            ESP_LOGI(SEND_TASK1_TAG,"Send successfully!");
        }
        else if(xStatus == errQUEUE_FULL)
        {
            ESP_LOGW(SEND_TASK1_TAG,"Queue is full");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void sendTask2(void *pvParam)
{
    QueueHandle_t sendQHandle = (QueueHandle_t)pvParam;
    BaseType_t xStatus;
    int i = 2;
    while (1)
    {
        xStatus = xQueueSend(sendQHandle, &i, 0);
        if (xStatus == pdPASS)
        {
            ESP_LOGI(SEND_TASK2_TAG,"Send successfully!");
        }
        else if(xStatus == errQUEUE_FULL)
        {
            ESP_LOGW(SEND_TASK2_TAG,"Queue is full");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void recTask(void *pvParam)
{
    QueueSetHandle_t recQSetHandle = (QueueSetHandle_t)pvParam;
    QueueHandle_t recQHandle = NULL;
    BaseType_t xStatus;
    UBaseType_t QNum = 0;
    int j = 0;
    while (1)
    {
        recQHandle = xQueueSelectFromSet(recQSetHandle, portMAX_DELAY);
        QNum = uxQueueMessagesWaiting(recQHandle);
        ESP_LOGI(REC_TASK_TAG,"QNum is %d",QNum);
        xStatus = xQueueReceive(recQHandle, &j, portMAX_DELAY);
        if (xStatus == pdPASS)
        {
            ESP_LOGI(REC_TASK_TAG,"Receive data j=%d!",j);
        }
        else if(xStatus == errQUEUE_EMPTY)
        {
            ESP_LOGW(REC_TASK_TAG,"Queue is empty!");
        }
    }
}

void app_main(void)
{
    //* case: Queue Set
    QueueHandle_t QHandle1 = NULL;
    QueueHandle_t QHandle2 = NULL;

    QueueSetHandle_t QSetHandle = NULL;

    TaskHandle_t sTaskHandle1 = NULL;
    TaskHandle_t sTaskHandle2 = NULL;
    TaskHandle_t rTaskHandle = NULL;

    uint8_t QLength = 5;
    QHandle1 = xQueueCreate(QLength, sizeof(int));
    QHandle2 = xQueueCreate(QLength, sizeof(int));
    QSetHandle = xQueueCreateSet(QLength * 2);

    xQueueAddToSet(QHandle1, QSetHandle);
    xQueueAddToSet(QHandle2, QSetHandle);

    if ((QHandle1 != NULL) && (QHandle2 != NULL) && (QSetHandle != NULL))
    {
        ESP_LOGI(APP_TAG,"Create Queue successfully!");
        xTaskCreate(sendTask1, "sendTask1", 1024 * 5, (void *)QHandle1, 1, &sTaskHandle1);
        xTaskCreate(sendTask2, "sendTask2", 1024 * 5, (void *)QHandle2, 1, &sTaskHandle2);
        xTaskCreate(recTask, "recTask", 1024 * 5, (void *)QSetHandle, 1, &rTaskHandle);
    }
    else
    {
        ESP_LOGE(APP_TAG,"Create Queue fail!");
    }
    
    // Monitor the Stack
    UBaseType_t sStack1 = 0;
    UBaseType_t sStack2 = 0;
    UBaseType_t rStack = 0;
    while (1)
    {
        sStack1 = uxTaskGetStackHighWaterMark(sTaskHandle1);
        sStack2 = uxTaskGetStackHighWaterMark(sTaskHandle2);
        rStack = uxTaskGetStackHighWaterMark(rTaskHandle);
        ESP_LOGI(APP_TAG,"remaining sStack1 = %d\tremaining sStack2 = %d\tremaining rStack = %d\n",(1024*5 - sStack1), (1024*5 - sStack2), (1024*5 - rStack));
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
