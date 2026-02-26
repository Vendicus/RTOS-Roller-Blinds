<<<<<<< HEAD
#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

namespace HW_TIMER{
    extern bool esp_sleep_enable_timer_wakeup_setup(uint64_t time_s);

    extern time_t time_now; 
    extern time_t diff_time;  

    // extern hw_timer_t* HW_timer_2;

    // extern void hw_timer_2_activate(uint64_t time);

    // void isr_timer_2();

    // void attach_timers(uint8_t prescaler);
}
=======
#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

namespace HW_TIMER{
    extern bool esp_sleep_enable_timer_wakeup_setup(uint64_t time_s);

    extern time_t time_now; 
    extern time_t diff_time;  

    // extern hw_timer_t* HW_timer_2;

    // extern void hw_timer_2_activate(uint64_t time);

    // void isr_timer_2();

    // void attach_timers(uint8_t prescaler);
}
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
