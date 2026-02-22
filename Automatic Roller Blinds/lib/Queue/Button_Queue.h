#pragma once
#include <Arduino.h>

namespace QUEUE 
{
    extern QueueHandle_t ButtonQueue;

    void ButtonQueue_init(unsigned portBASE_TYPE queue_length = 10, unsigned portBASE_TYPE item_size = sizeof(uint8_t));
}