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

//* Input Parameter -> four cases ： int; array; struct; strings;

//* case 3 : struct
// typedef struct A_STRUCT
// {
//     int iNum1;
//     int iNum2;
// }hStruct_t;

// hStruct_t hStruct = {9,0};

void myTask(void *pvParam)
{
    //* case 1 : int
    // int *pInt = NULL;
    // pInt = (int *)pvParam;

    //* case 2 : array
    // int *pIntArray = NULL;
    // pIntArray = (int *)pvParam;

    //* case 3 : struct
    // hStruct_t *pStruct = NULL;
    // pStruct = (hStruct_t *)pvParam;

    //* case 4 : strings
    char *pStr = NULL;
    pStr = (char *)pvParam;

    printf("I got a massage : %s\n", pStr);

    vTaskDelay(1000 / portTICK_PERIOD_MS);

    vTaskDelete(NULL);
}

//* case 1 : int
// int inputNum = 1;

//* case 2 : array
// int inputArray[] = {1,2,3};

//* case 4 : strings
static const char *pcTxt = "i am Sormaker!";

void app_main(void)
{
    //* case 1 : int
    // xTaskCreate(myTask, "myTask", 2048, (void *)&inputNum, 1, NULL);
    //* case 2 : array
    // xTaskCreate(myTask, "myTask", 2048, (void *)inputArray, 1, NULL);
    //* case 3 : struct
    // xTaskCreate(myTask, "myTask", 2048, (void *)&hStruct, 1, NULL);
    //* case 4 : strings
    xTaskCreate(myTask, "myTask", 2048, (void *)pcTxt, 1, NULL);
}
