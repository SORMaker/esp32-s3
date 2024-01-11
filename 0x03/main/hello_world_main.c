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
    while (1)
    {
        printf("In myTask1-111!\n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
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
    //* case1: get priority
    // UBaseType_t iPriority;
    // TaskHandle_t pxTask = NULL;
    // xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, &pxTask);
    // iPriority = uxTaskPriorityGet(pxTask);
    // printf("iPriority = %d\n", iPriority);

    //* case2: same priority
    // xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, NULL);
    // xTaskCreate(myTask2, "myTask2", 2048, NULL, 1, NULL);


    //* case3: different priority
    // xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, NULL);
    // xTaskCreate(myTask2, "myTask2", 2048, NULL, 2, NULL);

    //* case4: set priority
    UBaseType_t iPriority;
    TaskHandle_t pxTask = NULL;

    xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, &pxTask);
    xTaskCreate(myTask2, "myTask2", 2048, NULL, 2, NULL);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    iPriority = 3;
    vTaskPrioritySet(pxTask,iPriority);
}
