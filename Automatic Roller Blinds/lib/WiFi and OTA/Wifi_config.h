#pragma once 

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

namespace WIFI{
    extern unsigned long wifi_connect_time;
    extern TaskHandle_t otaTaskHandle;

    bool wifi_start();
    void wifi_end();
    void ota_start();

    void ota_task(void* parameter);
    void ota_task_init(unsigned portBASE_TYPE stack_size = 2048, UBaseType_t priority = 1);
}