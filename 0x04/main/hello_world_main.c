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

void myTask1(void *pvParam)
{
    printf("Task Begin!\n");
    //* case3: Suspend All to make sure program runing safely
    vTaskSuspendAll();

    for (int i = 0; i < 9999; i++)
    {
        for (int j = 0; j < 5000; j++)
        {
            ;
        }
    }

    xTaskResumeAll();
    printf("Task End!\n");

    vTaskDelete(NULL);
    //* case2: Suspend Task through itself
    // vTaskSuspend(NULL);
}

void myTask2(void *pvParam)
{
    while (1)
    {
        printf("In myTask2-222!\n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    TaskHandle_t pxTask = NULL;
    xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, &pxTask);
    xTaskCreate(myTask2, "myTask2", 2048, NULL, 1, NULL);

    //* case1:Suspend and Resume Task through TaskHandle
    // vTaskDelay(3000 / portTICK_PERIOD_MS);
    // vTaskSuspend(pxTask);
    // vTaskDelay(3000 / portTICK_PERIOD_MS);
    // vTaskResume(pxTask);
}
