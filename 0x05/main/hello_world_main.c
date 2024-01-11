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
    TaskHandle_t pxTask = NULL;
    xTaskCreate(myTask1, "myTask1", 2048, NULL, 1, &pxTask);
    xTaskCreate(myTask2, "myTask2", 2048, NULL, 1, NULL);
    //* case: Task List
    //* we should set configUSE_TRACE_FACILITY and configUSE_STATS_FORMATTING_FUNCTIONS in menuconfig
    static char pcWriteBuffer[512] = {0};

    while (1)
    {
        vTaskList(pcWriteBuffer);
        printf("----------------------------------------------------------\n");
        printf("Name\t\tState\tPriority Stack\tNum\n");
        printf("%s\n",pcWriteBuffer);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }

}
