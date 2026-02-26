#pragma once
#include <Arduino.h>

namespace MUTEX
{
    extern SemaphoreHandle_t MUTEXmode;
    extern SemaphoreHandle_t MUTEXwifi;
    extern SemaphoreHandle_t MUTEXtest;
    void MUTEXmode_init();
}


