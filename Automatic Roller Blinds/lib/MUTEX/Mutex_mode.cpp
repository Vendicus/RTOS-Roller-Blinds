#include <Arduino.h>
#include <Mutex_mode.h>

namespace MUTEX
{
    SemaphoreHandle_t MUTEXmode = NULL; // MUTEX to use with global MODE var selector for state machine.
    SemaphoreHandle_t MUTEXwifi = NULL; // MUTEX to use with WIFI, check wifi_config for details.
    SemaphoreHandle_t MUTEXtest = NULL; // MUTEX test variable.
    
    void MUTEXmode_init()
    {
        MUTEXmode = xSemaphoreCreateMutex();

        if (MUTEXmode == NULL)
        {
            Serial.println("Mutex for MODE switch could not be created");
        }
        else
        {
            Serial.println("Mutex for MODE switch created successfully");
        }

        MUTEXwifi = xSemaphoreCreateMutex();

        if (MUTEXwifi == NULL)
        {
            Serial.println("Mutex for wifi counter could not be created");
        }
        else
        {
            Serial.println("Mutex for wifi counter created successfully");
        }

        MUTEXtest = xSemaphoreCreateMutex();
        if (MUTEXtest == NULL)
        {
            Serial.println("Mutex test could not be created");
        }
        else
        {
            Serial.println("Mutex test created successfully");
        }
    }
}