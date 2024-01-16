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

#include "freertos/stream_buffer.h" // added bt Sormaker
#include "esp_log.h"          // added by Sormaker
#include "string.h"

const char *APP_TAG = "app_main";
StreamBufferHandle_t StreamBufferHandle;

void Task1(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);

    int sendBytes = 0;
    int strLength = 0;
    char seText[50];
    char i = 0;

    while (1)
    {
        i++;
        strLength = snprintf(seText ,50 ,"Hello, i am Sormaker! %d",i);
        sendBytes = xStreamBufferSend(StreamBufferHandle, (void *)seText, strLength, portMAX_DELAY);
        ESP_LOGI(pcName, "Send--> strLength : %d, sendBytes : %d", strLength, sendBytes);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void Task2(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);

    int ReceiveBytes = 0;
    char reText[50];
    
    while (1)
    {
        memset(reText, 0, sizeof(reText));
        ReceiveBytes = xStreamBufferReceive(StreamBufferHandle, (void *)reText, sizeof(reText), portMAX_DELAY);
        ESP_LOGI(pcName, "Receive--> ReceiveBytes : %d, reText : %s",ReceiveBytes, reText);
    }
}

void Task3(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);
    int bufSpace = 0;
    int minBufSpace = 1000;
    while (1)
    {
        bufSpace = xStreamBufferSpacesAvailable(StreamBufferHandle);
        if (bufSpace < minBufSpace)
        {
            minBufSpace = bufSpace;
        }
        
        ESP_LOGI(pcName, "minBufSpace : %d, bufSpace : %d",minBufSpace, bufSpace);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void app_main(void)
{
    //* case: Stream Buffer 2
    StreamBufferHandle = xStreamBufferCreate(200, 10);
    if (StreamBufferHandle != NULL)
    {
        ESP_LOGI(APP_TAG, "StreamBufferHandle create successfully!");
        vTaskSuspendAll();

        xTaskCreatePinnedToCore(Task3, "Task3", 1024*5, NULL, 1, NULL, 0);
        xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 1, NULL, 0);
        xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, NULL, 0);

        xTaskResumeAll();
    }
    else
    {
        ESP_LOGE(APP_TAG, "Fail to create StreamBufferHandle");
    }
    

}


