<<<<<<< HEAD
#pragma once
#include <Arduino.h>

namespace QUEUE 
{
    extern QueueHandle_t ButtonQueue;

    void ButtonQueue_init(unsigned portBASE_TYPE queue_length = 10, unsigned portBASE_TYPE item_size = sizeof(uint8_t));
=======
#pragma once
#include <Arduino.h>

namespace QUEUE 
{
    extern QueueHandle_t ButtonQueue;

    void ButtonQueue_init(unsigned portBASE_TYPE queue_length = 10, unsigned portBASE_TYPE item_size = sizeof(uint8_t));
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
}