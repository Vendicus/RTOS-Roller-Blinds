<<<<<<< HEAD
#include <Arduino.h>
#include <SSR_Queue.h>

namespace QUEUE 
{
    QueueHandle_t SSRQueue = NULL;

    void SRRQueue_init(unsigned portBASE_TYPE queue_length, unsigned portBASE_TYPE item_size)
    {
        SSRQueue = xQueueCreate(queue_length, item_size);

        if (SSRQueue == NULL) 
        {
            // Handle error: Queue was not created
            Serial.println("Error creating SRRQueue.");
        }
        else 
        {
            Serial.println("SRRQueue created successfully.");
        }
    }
=======
#include <Arduino.h>
#include <SSR_Queue.h>

namespace QUEUE 
{
    QueueHandle_t SSRQueue = NULL;

    void SRRQueue_init(unsigned portBASE_TYPE queue_length, unsigned portBASE_TYPE item_size)
    {
        SSRQueue = xQueueCreate(queue_length, item_size);

        if (SSRQueue == NULL) 
        {
            // Handle error: Queue was not created
            Serial.println("Error creating SRRQueue.");
        }
        else 
        {
            Serial.println("SRRQueue created successfully.");
        }
    }
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
}