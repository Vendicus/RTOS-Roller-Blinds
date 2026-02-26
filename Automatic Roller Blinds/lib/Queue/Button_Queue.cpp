<<<<<<< HEAD
#include <Arduino.h>
#include <Button_Queue.h>

namespace QUEUE 
{
    QueueHandle_t ButtonQueue = NULL;

    void ButtonQueue_init(unsigned portBASE_TYPE queue_length, unsigned portBASE_TYPE item_size)
    {
        ButtonQueue = xQueueCreate(queue_length, item_size);

        if (ButtonQueue == NULL) 
        {
            // Handle error: Queue was not created
            Serial.println("Error creating ButtonQueue");
        }
        else 
        {
            Serial.println("ButtonQueue created successfully");
        }
    }
=======
#include <Arduino.h>
#include <Button_Queue.h>

namespace QUEUE 
{
    QueueHandle_t ButtonQueue = NULL;

    void ButtonQueue_init(unsigned portBASE_TYPE queue_length, unsigned portBASE_TYPE item_size)
    {
        ButtonQueue = xQueueCreate(queue_length, item_size);

        if (ButtonQueue == NULL) 
        {
            // Handle error: Queue was not created
            Serial.println("Error creating ButtonQueue");
        }
        else 
        {
            Serial.println("ButtonQueue created successfully");
        }
    }
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
}