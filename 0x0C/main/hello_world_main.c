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

#include "freertos/timers.h"   // added by Sormaker
#include "esp_log.h"          // added by Sormaker

static const char *APP_TAG = "app_main";

void TimerCallBack( TimerHandle_t xTimer )
{
    const char *pcName = pcTimerGetName(xTimer);
    int *i = (int *)pvTimerGetTimerID(xTimer);
    //* if we want to use ESP_LOGI, we should set configTIMER_TASK_STACK_DEPTH to 4096
    ESP_LOGI(pcName,"time id : %d!",*i);
    //* print timer_id's address
    // ESP_LOGI(pcName,"time id : %d!",(int)i);
}

void app_main(void)
{
    //* case: Software Timer
    static const int timer1_id = 1;
    static const int timer2_id = 2;
    TimerHandle_t Timer1;
    TimerHandle_t Timer2;
    BaseType_t tStatus = 0;

    Timer1 = xTimerCreate("Timer1", pdMS_TO_TICKS(1000), pdTRUE, (void *)&timer1_id, TimerCallBack);
    Timer2 = xTimerCreate("Timer2", pdMS_TO_TICKS(2000), pdTRUE, (void *)&timer2_id, TimerCallBack);

    tStatus += xTimerStart(Timer1, 0);
    tStatus += xTimerStart(Timer2, 0);

    if (tStatus == (pdPASS * 2))
    {
        ESP_LOGI(APP_TAG,"Timer start Successfully!");
    }
    else
    {
        ESP_LOGE(APP_TAG,"Timer start Fail!");
    }
    //* Reset Timer
    // while (1)
    // {
    //     xTimerReset(Timer2, 0);
    //     vTaskDelay(pdMS_TO_TICKS(1000));
    // }
    
    vTaskDelay(pdMS_TO_TICKS(6000));
    //* Change Timer's Period
    xTimerChangePeriod(Timer1, pdMS_TO_TICKS(4000), 0);
    vTaskDelay(pdMS_TO_TICKS(12000));
    //* Stop Timer
    xTimerStop(Timer1,0);
    xTimerStop(Timer2,0);
}
