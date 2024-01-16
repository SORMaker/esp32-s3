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

#include "freertos/message_buffer.h" // added bt Sormaker
#include "esp_log.h"          // added by Sormaker
#include "string.h"

const char *APP_TAG = "app_main";
MessageBufferHandle_t MessageBufferHandle;

void Task1(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);

    int sendBytes = 0;
    int strLength = 0;
    char seText[50];

    for (int  i = 0; i < 3; i++)
    {
        strLength = snprintf(seText ,50 ,"Hello, i am Sormaker! NO.%d",i);
        sendBytes = xMessageBufferSend(MessageBufferHandle, (void *)seText, strLength, portMAX_DELAY);
        ESP_LOGI(pcName, "Send--> i : %d, sendBytes : %d", i, sendBytes);
    }
    vTaskDelete(NULL);


}

void Task2(void *pvParam)
{
    const char *pcName = pcTaskGetName(NULL);

    int ReceiveBytes = 0;
    char reText[50];
    
    while (1)
    {
        memset(reText, 0, sizeof(reText));
        ReceiveBytes = xMessageBufferReceive(MessageBufferHandle, (void *)reText, sizeof(reText), portMAX_DELAY);
        ESP_LOGI(pcName, "Receive--> ReceiveBytes : %d, reText : %s",ReceiveBytes, reText);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    //* case: Message Buffer
    MessageBufferHandle = xMessageBufferCreate(200);
    if (MessageBufferHandle != NULL)
    {
        ESP_LOGI(APP_TAG, "MessageBufferHandle create successfully!");
        vTaskSuspendAll();

        xTaskCreatePinnedToCore(Task1, "Task1", 1024*5, NULL, 1, NULL, 0);
        xTaskCreatePinnedToCore(Task2, "Task2", 1024*5, NULL, 1, NULL, 0);

        xTaskResumeAll();
    }
    else
    {
        ESP_LOGE(APP_TAG, "Fail to create MessageBufferHandle");
    }
    

}


