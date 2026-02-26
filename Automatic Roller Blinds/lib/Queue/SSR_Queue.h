<<<<<<< HEAD
#pragma once
#include <Arduino.h>

namespace QUEUE 
{
    extern QueueHandle_t SSRQueue;

    void SRRQueue_init(unsigned portBASE_TYPE queue_length = 3, unsigned portBASE_TYPE item_size = sizeof(uint8_t));
}
=======
#pragma once
#include <Arduino.h>

namespace QUEUE 
{
    extern QueueHandle_t SSRQueue;

    void SRRQueue_init(unsigned portBASE_TYPE queue_length = 3, unsigned portBASE_TYPE item_size = sizeof(uint8_t));
}
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
