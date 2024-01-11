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

//* case: Task Watch Dog
#include "esp_task_wdt.h"   // added by Sormaker

void myTask1(void *pvParam)
{
    while (1)
    {
        ;
        // printf("myTask1\n");

        // vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void myTask2(void *pvParam)
{
    while (1)
    {
        
        printf("myTask2\n");
        // esp_task_wdt_reset();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    TaskHandle_t pxTask1 = NULL;
    TaskHandle_t pxTask2 = NULL;
    xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, &pxTask1);
    xTaskCreate(myTask2, "myTask2", 2048, NULL, 1, &pxTask2);

    esp_task_wdt_add(pxTask2);
    
}
