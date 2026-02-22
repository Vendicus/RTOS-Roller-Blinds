#pragma once
#include <Arduino.h>

namespace QUEUE 
{
    extern QueueHandle_t SSRQueue;

    void SRRQueue_init(unsigned portBASE_TYPE queue_length = 3, unsigned portBASE_TYPE item_size = sizeof(uint8_t));
}
