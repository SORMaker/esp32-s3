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
static const char *SEND_TASK_TAG = "sendTask";
static const char *REC_TASK_TAG = "recTask";


typedef struct A_STRUCT
{
    char id;
    int score;
}xStruce_t;


void sendTask(void *pvParam)
{
    QueueHandle_t sendQHandle = (QueueHandle_t)pvParam;
    BaseType_t xStatus;
    //* case1: Transmit int type between two tasks
    // int i = 0;
    // while (1)
    // {
    //     xStatus = xQueueSend(sendQHandle, &i, 0);
    //     if (xStatus == pdPASS)
    //     {
    //         ESP_LOGI(SEND_TASK_TAG,"Send successfully!");
    //     }
    //     else if(xStatus == errQUEUE_FULL)
    //     {
    //         ESP_LOGE(SEND_TASK_TAG,"Queue is full");
    //     }
    //     i++;
    //     if (i == 8)i = 0;
    //// * Queue Will be Empty if Delay 2s
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }

    //* case2: Transmit struct type between two tasks
    // xStruce_t sStruct = {0,96};
    // while (1)
    // {
    //     xStatus = xQueueSend(sendQHandle, &sStruct, 0);
    //     if (xStatus == pdPASS)
    //     {
    //         ESP_LOGI(SEND_TASK_TAG,"Send successfully!");
    //     }
    //     else if(xStatus == errQUEUE_FULL)
    //     {
    //         ESP_LOGE(SEND_TASK_TAG,"Queue is full");
    //     }
    //     sStruct.id++;
    //     if (sStruct.id == 8)sStruct.id = 0;
    //// * Queue Will be Empty if Delay 2s
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }

    //* case3: Transmit pointer type between two tasks
    char *sendp = NULL;
    uint8_t i = 32;
    while (1)
    {
        sendp = (char *)malloc(50*sizeof(char));
        snprintf(sendp, 50, "i love ESP%d!", i);         // different from sprint, snprint will restrice the number of characters output
        xStatus = xQueueSend(sendQHandle, &sendp, 0);    // 底层调用的是memcpy，所以需要传递地址
        if (xStatus == pdPASS)
        {
            ESP_LOGI(SEND_TASK_TAG,"Send successfully!");
        }
        else if(xStatus == errQUEUE_FULL)
        {
            ESP_LOGE(SEND_TASK_TAG,"Queue is full");
        }
        // * Queue Will be Empty if Delay 2s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
}

void recTask(void *pvParam)
{
    QueueHandle_t recQHandle = (QueueHandle_t)pvParam;
    BaseType_t xStatus;
    UBaseType_t QNum = 0;
    //* case1: Transmit int type between two tasks
    // int j = 0;
    // while (1)
    // {
    //     QNum = uxQueueMessagesWaiting(recQHandle);
    //     ESP_LOGI(REC_TASK_TAG,"QNum is %d",QNum);
    //     xStatus = xQueueReceive(recQHandle, &j, 0);
    //     if (xStatus == pdPASS)
    //     {
    //         ESP_LOGI(REC_TASK_TAG,"Receive data j=%d!",j);
    //     }
    //     else if(xStatus == errQUEUE_EMPTY)
    //     {
    //         ESP_LOGE(REC_TASK_TAG,"Queue is empty!");
    //     }
    //// * Queue Will Block if Delay 2s
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    
    //* case2: Transmit Struct type between two tasks
    // xStruce_t rStruct = {0};
    // while (1)
    // {
    //     QNum = uxQueueMessagesWaiting(recQHandle);
    //     ESP_LOGI(REC_TASK_TAG,"QNum is %d",QNum);
    //     xStatus = xQueueReceive(recQHandle, &rStruct, 0);
    //     if (xStatus == pdPASS)
    //     {
    //         ESP_LOGI(REC_TASK_TAG,"Receive data id = %d\tscore = %d!",rStruct.id, rStruct.score);
    //     }
    //     else if(xStatus == errQUEUE_EMPTY)
    //     {
    //         ESP_LOGE(REC_TASK_TAG,"Queue is empty!");
    //     }
    ////* Queue Will Block if Delay 2s
    //     vTaskDelay(1000 / portTICK_PERIOD_MS);
    // }
    
    //* case3: Transmit pointer type between two tasks
    char *recp = NULL;
    while (1)
    {
        QNum = uxQueueMessagesWaiting(recQHandle);
        ESP_LOGI(REC_TASK_TAG,"QNum is %d",QNum);
        xStatus = xQueueReceive(recQHandle, &recp, 0);
        if (xStatus == pdPASS)
        {
            ESP_LOGI(REC_TASK_TAG,"Receive data strings = %s",recp);
        }
        else if(xStatus == errQUEUE_EMPTY)
        {
            ESP_LOGE(REC_TASK_TAG,"Queue is empty!");
        }
        free(recp);
    // * Queue Will Block if Delay 2s
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    QueueHandle_t QHandle = NULL;
    TaskHandle_t sTaskHandle = NULL;
    TaskHandle_t rTaskHandle = NULL;
    //* case1: Trans int type in two tasks
    // QHandle = xQueueCreate(5, sizeof(int));
    //* case2: Transmit Struct type between two tasks
    // QHandle = xQueueCreate(5, sizeof(xStruce_t));
    //* case3: Transmit pointer type between two tasks
    QHandle = xQueueCreate(5, sizeof(char *));
    if (QHandle != NULL)
    {
        ESP_LOGI(APP_TAG,"Create Queue successfully!");
        xTaskCreate(sendTask, "sendTask", 1024 * 5, (void *)QHandle, 1, &sTaskHandle);
        xTaskCreate(recTask, "recTask", 1024 * 5, (void *)QHandle, 1, &rTaskHandle);
    }
    else
    {
        ESP_LOGE(APP_TAG,"Create Queue fail!");
    }
    
    // Monitor the Stack
    UBaseType_t sStack = 0;
    UBaseType_t rStack = 0;
    while (1)
    {
        sStack = uxTaskGetStackHighWaterMark(sTaskHandle);
        rStack = uxTaskGetStackHighWaterMark(rTaskHandle);
        ESP_LOGI(APP_TAG,"remaining sStack = %d\tremaining rStack = %d\n",(1024*5 - sStack), (1024*5 - rStack));
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
