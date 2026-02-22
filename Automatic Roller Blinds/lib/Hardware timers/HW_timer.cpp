#include <Arduino.h>
#include <HW_timer.h> 
#include <SSR_task.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Mutex_mode.h>
#include <global.h>

namespace HW_TIMER
{   

    time_t time_now = 0;
    time_t diff_time = 0;

    bool esp_sleep_enable_timer_wakeup_setup(uint64_t time_s)
    {
        time_now = time(nullptr); 
        diff_time = time_now - GLOBALS::TIME_SET;
        if (time_s < diff_time)
        {
            return false;
        }
        Serial.printf("Setting up ESP32 light sleep wake-up timer for %llu seconds from now.\n", (time_s - diff_time));
        esp_sleep_enable_timer_wakeup((time_s - diff_time)* 1000000ULL); // Convert seconds to microseconds
        return true;
    }

    // ********************** earlier version used one hardware timer as below **********************

    //hw_timer_t* HW_timer_2 = nullptr;

    // void IRAM_ATTR isr_timer_2()
    // {
    //     timerAlarmDisable(HW_timer_2);
    //     BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //     xTaskNotifyFromISR(SSR::SSR::ssr_task_handle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
    //     portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    // }

    // void hw_timer_2_activate(uint64_t time)
    // {
    //     uint64_t calc = time * 1000000ULL; 
    //     timerAlarmDisable(HW_timer_2); // reset timer counter
    //     timerWrite(HW_timer_2, 0);
    //     timerAlarmWrite(HW_timer_2, calc, false);
    //     timerAlarmEnable(HW_timer_2);
    // }

    // void attach_timers(uint8_t prescaler)
    // {
    //     HW_timer_2 = timerBegin(1, prescaler, true); // begin with hw timer 1
    //     timerAttachInterrupt(HW_timer_2, &isr_timer_2, true);
    // }
}