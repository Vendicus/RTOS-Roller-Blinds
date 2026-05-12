#pragma once
#include <Arduino.h>
#include <time.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


namespace SSR {
    class SSR
    {
        public:
            struct Sun_t
            {
                time_t sunrise;
                time_t sunset;
            };

            static bool set_RTC_time();
            static void get_RTC_time();
            static time_t iso_string_to_time(const char* s);
            static bool sunrise_sunset_set_time();
            static bool set_hw_timer();
            static bool set_hw_debug_timer();
        
            // TASKS
            static TaskHandle_t ssr_task_handle;
            static void SSR_task(void *parameter);
            static void SSR_task_init(unsigned portBASE_TYPE stack_size, UBaseType_t priority);

        private:
            // global vars used with sunrise and sunset time reading and processing
            static HTTPClient http;
            static String payload;
            static JsonDocument api_documentation;
            static DeserializationError error;
        
            // local methods vars
            static uint8_t Mode;
            static uint32_t signal;
            static String iso_string;

            // time vars
            static struct tm t;
            static struct tm date;
            static time_t rtc_time;
            static timeval current_time; 
            static time_t now, diff_sunrise, diff_sunset;
            static Sun_t suntime;
            static uint64_t time_to_next_check;
    };
}