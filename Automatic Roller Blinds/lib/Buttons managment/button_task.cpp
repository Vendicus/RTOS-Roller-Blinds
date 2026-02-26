<<<<<<< HEAD
#include <button_task.h>
#include <Arduino.h>
#include <Button_Queue.h>
#include <global.h>
#include <Mutex_mode.h>
#include <main_task.h>

namespace BUTTON_TASK
{
    TaskHandle_t buttonTaskHandle = NULL;
    uint8_t receivedButtonID {0};

    void button_task(void* parameter)
    {
        while (1)
        {
            // Wait indefinitely for a button ID from the queue, it will block here until a button press is detected and does not consume CPU cycles
            if (xQueueReceive(QUEUE::ButtonQueue, &receivedButtonID, portMAX_DELAY) == pdTRUE)
            {
                // debouncing
                vTaskDelay(pdMS_TO_TICKS(40));

                // reading the button state after debounce delay
                switch (receivedButtonID)
                {
                case 1: // up button ISR called
                    if (digitalRead(Pin_up) == LOW && digitalRead(Pin_down) == LOW) // two buttons pressed
                    {
                        Serial.println("Both UP and DOWN buttons pressed");
                        xTaskNotify(MAIN_TASK::mainTaskHandle, 4, eSetValueWithOverwrite); // 4 - encoder positioning
                    }
                    else if (digitalRead(Pin_up) == LOW) // button pressed
                    {
                        Serial.println("UP button pressed");
                        xTaskNotify(MAIN_TASK::mainTaskHandle, receivedButtonID, eSetValueWithOverwrite); // 1 - up button pressed
                    }
                    else // button released
                    {
                        xTaskNotify(MAIN_TASK::mainTaskHandle, 5, eSetValueWithOverwrite); // 5 - means button unpressed, motors should stop
                    }
                    break;
                
                case 2: // down button ISR called
                    if (digitalRead(Pin_up) == LOW && digitalRead(Pin_down) == LOW) // two buttons pressed
                    {
                        Serial.println("Both UP and DOWN buttons pressed");
                        xTaskNotify(MAIN_TASK::mainTaskHandle, 4, eSetValueWithOverwrite); // 4 - encoder positioning
                    }
                    else if (digitalRead(Pin_down) == LOW) // button pressed
                    {
                        Serial.println("DOWN button pressed");
                        xTaskNotify(MAIN_TASK::mainTaskHandle, receivedButtonID, eSetValueWithOverwrite); // 2 - down button pressed
                    }
                    else // button released
                    {
                        xTaskNotify(MAIN_TASK::mainTaskHandle, 5, eSetValueWithOverwrite); // 5 - means button unpressed, motors should stop
                    }
                    break;
                
                case 3: // emergency stop button ISR called
                    xTaskNotify(MAIN_TASK::mainTaskHandle, 3, eSetValueWithOverwrite); // 3 - emergency stop button pressed                 
                    break;
                }
            }
        }
    }

    void button_task_init(const uint8_t pin_up, const uint8_t pin_down, const uint8_t pin_stop, unsigned portBASE_TYPE stack_size, UBaseType_t priority)
    {
        Pin_up = pin_up;
        Pin_down = pin_down;
        Pin_stop = pin_stop;

        xTaskCreate(
            button_task,            // Task function
            "Button Task",          // Name of the task
            stack_size,             // Stack size in words
            NULL,                   // Task input parameter
            priority,               // Priority of the task
            &buttonTaskHandle       // Task handle
        );
    }
=======
#include <button_task.h>
#include <Arduino.h>
#include <Button_Queue.h>
#include <global.h>
#include <Mutex_mode.h>
#include <main_task.h>

namespace BUTTON_TASK
{
    TaskHandle_t buttonTaskHandle = NULL;
    uint8_t receivedButtonID {0};

    void button_task(void* parameter)
    {
        while (1)
        {
            // Wait indefinitely for a button ID from the queue, it will block here until a button press is detected and does not consume CPU cycles
            if (xQueueReceive(QUEUE::ButtonQueue, &receivedButtonID, portMAX_DELAY) == pdTRUE)
            {
                // debouncing
                vTaskDelay(pdMS_TO_TICKS(40));

                // reading the button state after debounce delay
                switch (receivedButtonID)
                {
                case 1: // up button ISR called
                    if (digitalRead(Pin_up) == LOW && digitalRead(Pin_down) == LOW) // two buttons pressed
                    {
                        Serial.println("Both UP and DOWN buttons pressed");
                        xTaskNotify(MAIN_TASK::mainTaskHandle, 4, eSetValueWithOverwrite); // 4 - encoder positioning
                    }
                    else if (digitalRead(Pin_up) == LOW) // button pressed
                    {
                        Serial.println("UP button pressed");
                        xTaskNotify(MAIN_TASK::mainTaskHandle, receivedButtonID, eSetValueWithOverwrite); // 1 - up button pressed
                    }
                    else // button released
                    {
                        xTaskNotify(MAIN_TASK::mainTaskHandle, 5, eSetValueWithOverwrite); // 5 - means button unpressed, motors should stop
                    }
                    break;
                
                case 2: // down button ISR called
                    if (digitalRead(Pin_up) == LOW && digitalRead(Pin_down) == LOW) // two buttons pressed
                    {
                        Serial.println("Both UP and DOWN buttons pressed");
                        xTaskNotify(MAIN_TASK::mainTaskHandle, 4, eSetValueWithOverwrite); // 4 - encoder positioning
                    }
                    else if (digitalRead(Pin_down) == LOW) // button pressed
                    {
                        Serial.println("DOWN button pressed");
                        xTaskNotify(MAIN_TASK::mainTaskHandle, receivedButtonID, eSetValueWithOverwrite); // 2 - down button pressed
                    }
                    else // button released
                    {
                        xTaskNotify(MAIN_TASK::mainTaskHandle, 5, eSetValueWithOverwrite); // 5 - means button unpressed, motors should stop
                    }
                    break;
                
                case 3: // emergency stop button ISR called
                    xTaskNotify(MAIN_TASK::mainTaskHandle, 3, eSetValueWithOverwrite); // 3 - emergency stop button pressed                 
                    break;
                }
            }
        }
    }

    void button_task_init(const uint8_t pin_up, const uint8_t pin_down, const uint8_t pin_stop, unsigned portBASE_TYPE stack_size, UBaseType_t priority)
    {
        Pin_up = pin_up;
        Pin_down = pin_down;
        Pin_stop = pin_stop;

        xTaskCreate(
            button_task,            // Task function
            "Button Task",          // Name of the task
            stack_size,             // Stack size in words
            NULL,                   // Task input parameter
            priority,               // Priority of the task
            &buttonTaskHandle       // Task handle
        );
    }
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
}