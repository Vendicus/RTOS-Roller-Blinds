#include <Arduino.h>
#include <main_task.h>
#include <global.h>
#include <Mutex_mode.h>
#include <MotorA_class.h>
#include <MotorB_class.h>
#include <Software_timers.h>
#include <EncoderA_class.h>
#include <EncoderB_class.h>
#include <SSR_Queue.h>
#include <SSR_task.h>
#include <Pins.h>
#include <hw_timer.h>

namespace MAIN_TASK
{
    TaskHandle_t mainTaskHandle = NULL;
    uint32_t ManualModeValue {0};
    uint8_t SSRModeValue {0};
    uint8_t local_mode {0};
    uint8_t serial_latch {0};
    uint8_t counter {0};

    void main_task(void* parameter)
    {
        xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
        GLOBALS::MODE = 0; // Initial mode active
        xSemaphoreGive(MUTEX::MUTEXmode);

        while (1)
        {   
            // --------------------------------------------> MANUAL MODE <---------------------------------------------
            // Check if there is a notification from the button task
            if (xTaskNotifyWait(0, 0, &ManualModeValue, 0) == pdTRUE)
            {
                switch(ManualModeValue)
                {
                case 1: // UP button
                    digitalWrite(PIN::Relay_pin, HIGH); // turn on relay
                    xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
                    GLOBALS::MODE = 1; // Switch to manual mode
                    xSemaphoreGive(MUTEX::MUTEXmode);
                    local_mode = 1;
                    Serial.println("UP button pressed - MODE 1 active");
                    vTaskResume(MOTOR::MotorA::task_motorA_controller_handle);
                    vTaskResume(MOTOR::MotorB::task_motorB_controller_handle);
                    xTaskNotify(MOTOR::MotorA::task_motorA_controller_handle, ManualModeValue, eSetValueWithOverwrite);       
                    xTaskNotify(MOTOR::MotorB::task_motorB_controller_handle, ManualModeValue, eSetValueWithOverwrite);
                    break;
                
                case 2: // DOWN button
                    digitalWrite(PIN::Relay_pin, HIGH); // turn on relay
                    xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
                    GLOBALS::MODE = 1; // Switch to manual mode     
                    xSemaphoreGive(MUTEX::MUTEXmode);
                    local_mode = 1;
                    Serial.println("DOWN button pressed - MODE 1 active");
                    vTaskResume(MOTOR::MotorA::task_motorA_controller_handle);
                    vTaskResume(MOTOR::MotorB::task_motorB_controller_handle);
                    xTaskNotify(MOTOR::MotorA::task_motorA_controller_handle, ManualModeValue, eSetValueWithOverwrite);
                    xTaskNotify(MOTOR::MotorB::task_motorB_controller_handle, ManualModeValue, eSetValueWithOverwrite);
                    break;
                
                case 3: // EMERGENCY STOP button             
                    xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
                    GLOBALS::MODE = 3; // Switch to emergency mode
                    xSemaphoreGive(MUTEX::MUTEXmode);
                    vTaskResume(MOTOR::MotorA::task_motorA_controller_handle);
                    vTaskResume(MOTOR::MotorB::task_motorB_controller_handle);
                    xTimerStart(SOFTWARE_TIMERS::EmergencyMode_timer, 0); // start emergency mode timer for LED blinking
                    Serial.println("EMERGENCY STOP button pressed - MODE 3 active");
                    break;
                
                case 4: // MIN and MAX position of motors setting or reset of positions to default values
                    digitalWrite(PIN::Relay_pin, HIGH); // turn on relay
                    xTimerStart(SOFTWARE_TIMERS::EmergencyMode_timer, 0); // start emergency mode timer for LED blinking
                    xTaskNotifyWait(0, 0, &ManualModeValue, pdMS_TO_TICKS(50)); // wait for button release
                    while((ManualModeValue != 1 && ManualModeValue != 2 && ManualModeValue != 3 && counter < 5)) // wait for next button press or timeout after 5 seconds
                    {
                        if (xTaskNotifyWait(0, 0, &ManualModeValue, pdMS_TO_TICKS(1000)) == pdFALSE)
                        {
                            counter++;
                        }
                    }
                    counter = 0;
                    xTimerStop(SOFTWARE_TIMERS::EmergencyMode_timer, 0); // stop emergency mode timer for LED blinking
                    digitalWrite(PIN::LED_PIN, LOW); // turn off emergency mode LED
                    if (ManualModeValue == 1) // high section MAX setting
                    {
                        // ------------------> ENCODER A <-----------------
                        if (ENCODER::EncoderA::get_count() <= ENCODER::EncoderA::get_down_limit())
                        {
                            ENCODER::EncoderA::reset_count();
                            ENCODER::EncoderA::set_up_limit(1);
                            Serial.println("new MAX selected! Check MIN limit!");
                            digitalWrite(PIN::LED_PIN, HIGH); //signal to user that MAX limit is set below MIN limit and needs to be checked
                        } 
                        else 
                        {
                            ENCODER::EncoderA::set_up_limit(ENCODER::EncoderA::get_count());
                            Serial.println("new MAX selected!");
                        }
                        
                        // ------------------> ENCODER B <-----------------
                        if (ENCODER::EncoderB::get_count() <= ENCODER::EncoderB::get_down_limit())
                        {
                            ENCODER::EncoderB::reset_count();
                            ENCODER::EncoderB::set_up_limit(1);
                            Serial.println("new MAX selected! Check MIN limit!");
                            digitalWrite(PIN::LED_PIN, HIGH); //signal to user that MAX limit is set below MIN limit and needs to be checked
                        } 
                        else 
                        {
                            ENCODER::EncoderB::set_up_limit(ENCODER::EncoderB::get_count());
                            Serial.println("new MAX selected!");
                        }
                    }
                    else if (ManualModeValue == 2) // low section MIN setting
                    {
                        // ------------------> ENCODER A <-----------------
                        if (ENCODER::EncoderA::get_count() >= ENCODER::EncoderA::get_up_limit())
                        {
                            ENCODER::EncoderA::reset_count();
                            ENCODER::EncoderA::set_up_limit(1);
                            Serial.println("new MIN selected! Check MAX limit!");
                            digitalWrite(PIN::LED_PIN, HIGH); //signal to user that MAX limit is set below MIN limit and needs to be checked
                        } 
                        else 
                        {
                            ENCODER::EncoderA::set_up_limit(ENCODER::EncoderA::get_up_limit() - ENCODER::EncoderA::get_count());
                            ENCODER::EncoderA::reset_count();
                            Serial.println("new MIN selected!");
                        }
                        
                        // ------------------> ENCODER B <-----------------
                        if (ENCODER::EncoderB::get_count() >= ENCODER::EncoderB::get_up_limit())
                        {
                            ENCODER::EncoderB::reset_count();
                            ENCODER::EncoderB::set_up_limit(1);
                            Serial.println("new MIN selected! Check MAX limit!");
                            digitalWrite(PIN::LED_PIN, HIGH); //signal to user that MAX limit is set below MIN limit and needs to be checked
                        } 
                        else 
                        {
                            ENCODER::EncoderB::set_up_limit(ENCODER::EncoderB::get_up_limit() - ENCODER::EncoderB::get_count());
                            ENCODER::EncoderB::reset_count();
                            Serial.println("new MIN selected!");
                        }
                    }
                    else if (ManualModeValue == 3) // reset to default values
                    {
                        ENCODER::EncoderA::reset_count();
                        ENCODER::EncoderA::set_up_limit(1);
                        ENCODER::EncoderA::set_down_limit(0);
                        ENCODER::EncoderB::reset_count();
                        ENCODER::EncoderB::set_up_limit(1);
                        ENCODER::EncoderB::set_down_limit(0);   
                        Serial.println("MAX and MIN reset to default values!");
                    }
                    else // timeout, no button pressed
                    {
                        Serial.println("No button pressed, MAX and MIN limits not changed!");
                    }
                    break;
                
                case 5: // button released
                    xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
                    GLOBALS::MODE = 0; // Switch to idle mode
                    xSemaphoreGive(MUTEX::MUTEXmode);
                    local_mode = 0;
                    SOFTWARE_TIMERS::AfterManualMode_timer_active = true;
                    xTimerStart(SOFTWARE_TIMERS::AfterManualMode_timer, 0); // start timer to delay next possible automatic mode change after manual operation, 5 seconds 
                    Serial.println("Button unpressed - MODE 0 active");
                    break;
                }
            }    
            
            // ----------------------------------------------------> AUTOMATIC MODE <-----------------------------------------------
            // got signal to run automatic modes from hardware timer
            if (SOFTWARE_TIMERS::AfterManualMode_timer_active == false && local_mode == 0) // check if manual mode delay timer is not active
            {
                if (xQueueReceive(QUEUE::SSRQueue, &SSRModeValue, 0))
                {
                    Serial.printf("Received SSRModeValue: %d\n", SSRModeValue);
                    digitalWrite(PIN::Relay_pin, HIGH); // turn on relay
                    xTimerStart(SOFTWARE_TIMERS::EncoderListener_timer, pdMS_TO_TICKS(20)); // start encoder listener timer
                    switch(SSRModeValue)
                    {
                        case 1: //sunrise, open roller blinds
                            xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
                            GLOBALS::MODE = 10;
                            xSemaphoreGive(MUTEX::MUTEXmode);
                            vTaskResume(MOTOR::MotorA::task_motorA_controller_handle);
                            vTaskResume(MOTOR::MotorB::task_motorB_controller_handle);
                            local_mode = 10;
                            break;

                        case 2: //sunset, close roller blinds
                            xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
                            GLOBALS::MODE = 20;
                            xSemaphoreGive(MUTEX::MUTEXmode);
                            vTaskResume(MOTOR::MotorA::task_motorA_controller_handle);
                            vTaskResume(MOTOR::MotorB::task_motorB_controller_handle);
                            local_mode = 20;
                            break;
                    }
                }
            }
            
            // Check if in automatic mode and handle stopping at limits
            if ((local_mode == 10 && (ENCODER::EncoderA::at_up_limit() || ENCODER::EncoderB::at_up_limit())) 
            || (local_mode == 20 && (ENCODER::EncoderA::at_down_limit() || ENCODER::EncoderB::at_down_limit())) 
            || ((local_mode == 10 || local_mode == 20) && xTimerIsTimerActive(SOFTWARE_TIMERS::EncoderListener_timer) == pdFALSE))
            {
                xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
                GLOBALS::MODE = 0;
                xSemaphoreGive(MUTEX::MUTEXmode);
                local_mode = 0;
            }
             
            if ((ENCODER::EncoderA::at_up_limit() || ENCODER::EncoderB::at_up_limit()) && serial_latch == 0)
            {
                Serial.println("MAX reached");
                Serial.printf("Encoder A count: %d\n", ENCODER::EncoderA::get_count());
                Serial.printf("Encoder B count: %d\n", ENCODER::EncoderB::get_count());
                serial_latch = 1;                   
            }
            else if ((ENCODER::EncoderA::at_down_limit() || ENCODER::EncoderB::at_down_limit()) && serial_latch == 0)
            {
                Serial.println("MIN REACHED!");
                Serial.printf("Encoder A count: %d\n", ENCODER::EncoderA::get_count());
                Serial.printf("Encoder B count: %d\n", ENCODER::EncoderB::get_count());
                serial_latch = 1;
            }
            else if (!ENCODER::EncoderA::at_up_limit() && !ENCODER::EncoderA::at_down_limit() && !ENCODER::EncoderB::at_up_limit() && !ENCODER::EncoderB::at_down_limit() && serial_latch == 1)
            {
                serial_latch = 0;
            }
            
            // --------------------------------------------------> SLEEP MODE LOGIC <-----------------------------------------------
            if(GLOBALS::OTA_DONE && GLOBALS::SSR_DONE && SOFTWARE_TIMERS::AfterManualMode_timer_active == false && local_mode == 0)
            {
                digitalWrite(PIN::Relay_pin, LOW); // turn off relay
                Serial.println("Entering light sleep mode...");
                
                if (HW_TIMER::esp_sleep_enable_timer_wakeup_setup(GLOBALS::TIME_SET_FOR_TIMER))
                {
                    esp_light_sleep_start();
                    vTaskDelay(pdMS_TO_TICKS(50));
                }
                else
                {
                    Serial.println("Failed to set up light sleep timer.");
                    GLOBALS::SSR_DONE = 0;
                    xTaskNotifyGive(SSR::SSR::ssr_task_handle); // notify SSR task to run
                }
                if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER)  {
                    Serial.println("ESP32 woke up from timer wake-up.");
                    GLOBALS::SSR_DONE = 0; // indicate that SSR task needs to run again after wakeup
                    xTaskNotifyGive(SSR::SSR::ssr_task_handle); // notify SSR task to run
                }
                 // delay to allow serial messages to be sent before next loop
            }
            vTaskDelay(pdMS_TO_TICKS(20)); // little delay
        }
    }

    void main_task_init(unsigned portBASE_TYPE stack_size, UBaseType_t priority)
    {
        xTaskCreate(
            main_task,            // Task function
            "Main Task",          // Name of the task
            stack_size,           // Stack size in words
            NULL,                 // Task input parameter
            1,                    // Priority of the task
            &mainTaskHandle       // Task handle
        );
    }
}
