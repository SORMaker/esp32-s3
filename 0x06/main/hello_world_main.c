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
        printf("myTask1\n");

        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    TaskHandle_t pxTask = NULL;
    xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, &pxTask);
    UBaseType_t iStack = 0;
    //* case: Task Stack
    while (1)
    {
        iStack = uxTaskGetStackHighWaterMark(pxTask);
        printf("myTask1's Stack = %d\n", iStack);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}
