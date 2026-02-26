<<<<<<< HEAD
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SSR_task.h>
#include <ArduinoJson.h>
#include <time.h>
#include <HW_timer.h>
#include <SSR_Queue.h>
#include <Wifi_config.h>
#include <global.h>


namespace SSR
{
    TaskHandle_t SSR::ssr_task_handle = NULL;
    String SSR::payload {""};
    uint8_t SSR::Mode {0}; 
    uint32_t SSR::signal {0};
    tm SSR::t {0};
    
    time_t SSR::sun_string_to_time(const String &s)
    {
        //format YYYY-MM-DD HH:MM:SS
        sscanf(s.c_str(), "%d-%d-%d %d:%d:%d",
        &t.tm_year, 
        &t.tm_mon,
        &t.tm_mday,
        &t.tm_hour,
        &t.tm_min,
        &t.tm_sec);

        t.tm_year -= 1900;
        t.tm_mon -= 1;

        time_t out = mktime(&t);
        return out;
    }

    String SSR::iso_string {""};

    time_t SSR::iso_string_to_time(const String &s)
    {
        iso_string = s.c_str();
        iso_string.replace("T"," ");
        iso_string = iso_string.substring(0, 19);
        
        time_t out = SSR::sun_string_to_time(iso_string);
        return out;
    }

    time_t SSR::rtc_time {0};
    timeval SSR::current_time {0};

    bool SSR::set_RTC_time()
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("WiFi connection failed, cannot set RTC time.");
            WIFI::wifi_start();
            return false;
        } 

        // actual time download 
        HTTPClient http;
        http.begin("https://aisenseapi.com/services/v1/datetime");
        int httpCode = http.GET();
        if(httpCode == 200) // code - OK
        {
            payload = http.getString();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            
            String rtc_string = doc["datetime"];

            Serial.print("Date time set up: ");
            Serial.println(rtc_string);

            rtc_time = iso_string_to_time(rtc_string);
            current_time = { .tv_sec = rtc_time };
            settimeofday(&current_time, NULL);
        }
        else
        {
            Serial.println("There was error during datetime request.");
            return false;
        }
        http.end();
        WIFI::wifi_end();
        return true;
    }
    
    time_t SSR::now {0};

    void SSR::get_RTC_time()
    {
        now = time(NULL);
        if (now != (time_t)(-1))
        {
            Serial.printf("The current time is %s(%jd seconds since the Epoch)\n",
               asctime(gmtime(&now)), // time as date : YYYY-MM-DD HH:MM:SS
               (intmax_t)now);  // seconds from 1970
        }
        else
        {
            Serial.println("Error during reading RTC time, check if time is properly set.");
        }
    }

    SSR::Sun_t SSR::suntime {0};

    bool SSR::sunrise_sunset_set_time()
    {

        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("WiFi connection failed, cannot set sunrise and sunset time.");
            WIFI::wifi_start();
            return false;
        }

        HTTPClient http;
        http.begin("https://api.sunrise-sunset.org/json?lat=50.029468&lng=22.013083&formatted=0");
        // lattitude of your area -> lat= 
        // longtitude of your area -> lng= 

        int httpCode = http.GET();
        if(httpCode == 200) // code -> OK
        {
            payload = http.getString();

            DynamicJsonDocument doc(2048);
            deserializeJson(doc, payload);

            String sunrise = doc["results"]["sunrise"];
            String sunset = doc["results"]["sunset"];

            suntime.sunrise = iso_string_to_time(sunrise);
            suntime.sunset = iso_string_to_time(sunset);
            
            Serial.printf("Sunrise for today: %s, ",
                asctime(gmtime(&suntime.sunrise)));
            Serial.printf("Sunset : %s \n",
                asctime(gmtime(&suntime.sunset)));
        }
        else
        {
            Serial.println("There was error during sunrise and sunset request.");
            return false;
        }
        http.end();
        WIFI::wifi_end();
        return true;
    }

    time_t SSR::diff_sunrise {0};
    time_t SSR::diff_sunset {0};
    tm SSR::date {0};
    uint64_t SSR::time_to_next_check {0};

    bool SSR::set_hw_timer()
    {
        now = time(NULL); 
        localtime_r(&now, &date);
        
        if(sunrise_sunset_set_time())
        {
            diff_sunrise = suntime.sunrise - now; 
            diff_sunset = suntime.sunset - now;
            
            Serial.printf("Time (sec) for next sunrise: %s, for sunset %s",
                (String)(diff_sunrise),
                (String)(diff_sunset)
            );
            Serial.println();

            GLOBALS::TIME_SET = now;

            if (diff_sunrise > 0)
            {
                Serial.printf("Setting hardware timer for sunrise in %s seconds.\n", (String)(diff_sunrise));
                if (diff_sunrise > 1800) // 30 min earlier launch
                {
                    GLOBALS::TIME_SET_FOR_TIMER = diff_sunrise - 1800;
                    //HW_TIMER::hw_timer_2_activate((time_t)(diff_sunrise - 1800));
                }
                else
                {
                    GLOBALS::TIME_SET_FOR_TIMER = diff_sunrise;
                    //HW_TIMER::hw_timer_2_activate(diff_sunrise);
                }
                Mode = 1; // sunrise set activate
            }
            else if (diff_sunset > -720)
            {
                Serial.printf("Setting hardware timer for sunset in %s seconds.\n", (String)(diff_sunset + 720));
                GLOBALS::TIME_SET_FOR_TIMER = diff_sunset + 720;
                //HW_TIMER::hw_timer_2_activate((time_t)(diff_sunset + 720)); // set launch later for 720 seconds
                Mode = 2; // sunset set activate
            }
            else
            {
                time_to_next_check = (uint64_t)25 * 3600 - (uint64_t)date.tm_hour * 3600;
                Serial.printf("Both sunrise and sunset times for today have passed. Setting hardware timer for next checking in %lld seconds.\n", time_to_next_check);
                if (time_to_next_check <= 0) {
                    // next day
                    Serial.println("error minus day adjustment");
                    time_to_next_check += 24 * 3600;
                    GLOBALS::TIME_SET_FOR_TIMER = time_to_next_check + 24 * 3600;
                }
                else
                {
                    GLOBALS::TIME_SET_FOR_TIMER = time_to_next_check;
                    //HW_TIMER::hw_timer_2_activate(time_to_next_check);
                }
                Mode = 0; // unsettled time, wait for (26h - now) time to get new sunrise and sunset times
            }
            return true;
        }
        else
        {
            Serial.println("Sunrise and sunset time not set, cannot set hardware timer.");
            return false;
        }   
    }

    // TASKS

    void SSR::SSR_task(void* parameters)
    {   
        vTaskDelay(pdMS_TO_TICKS(5000)); // wait for 5 sec to let WiFi and OTA tasks finish their setup
        while(!set_RTC_time())
        {
            Serial.println("RTC time NOT set, next try for 5 sec.");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        get_RTC_time();
        while(!set_hw_timer())
        {
            Serial.println("Hardware timer NOT set, next try for 5 sec.");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        GLOBALS::SSR_DONE = 1; // indicate that SSR task has finished initial setup

        while (1)
        {
            xTaskNotifyWait(0, 0, &signal, portMAX_DELAY);
            if (Mode == 0)
            {
                while(!set_RTC_time())
                {
                    Serial.println("RTC time NOT set, next try for 5 sec.");
                    vTaskDelay(pdMS_TO_TICKS(5000));
                }
                get_RTC_time();
                while(!set_hw_timer())
                {
                    Serial.println("Hardware timer NOT set, next try for 5 sec.");
                    vTaskDelay(pdMS_TO_TICKS(5000));
                }
            }
            else 
            {
                xQueueSend(QUEUE::SSRQueue, &Mode, 0);
                vTaskDelay(pdMS_TO_TICKS(5000));
                while(!set_hw_timer())
                {
                    Serial.println("Hardware timer NOT set, next try for 5 sec.");
                    vTaskDelay(pdMS_TO_TICKS(5000));
                }
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
            GLOBALS::SSR_DONE = 1; // indicate that SSR task has finished
        }
    }

    void SSR::SSR_task_init(unsigned portBASE_TYPE stack_size, UBaseType_t priority)
    {
        xTaskCreate
        (
            SSR::SSR_task,
            "SunsetandSunriseReadingTask",
            stack_size,
            NULL,
            priority,
            &SSR::ssr_task_handle
        );
    }
=======
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SSR_task.h>
#include <ArduinoJson.h>
#include <time.h>
#include <HW_timer.h>
#include <SSR_Queue.h>
#include <Wifi_config.h>
#include <global.h>


namespace SSR
{
    TaskHandle_t SSR::ssr_task_handle = NULL;
    String SSR::payload {""};
    uint8_t SSR::Mode {0}; 
    uint32_t SSR::signal {0};
    tm SSR::t {0};
    
    time_t SSR::sun_string_to_time(const String &s)
    {
        //format YYYY-MM-DD HH:MM:SS
        sscanf(s.c_str(), "%d-%d-%d %d:%d:%d",
        &t.tm_year, 
        &t.tm_mon,
        &t.tm_mday,
        &t.tm_hour,
        &t.tm_min,
        &t.tm_sec);

        t.tm_year -= 1900;
        t.tm_mon -= 1;

        time_t out = mktime(&t);
        return out;
    }

    String SSR::iso_string {""};

    time_t SSR::iso_string_to_time(const String &s)
    {
        iso_string = s.c_str();
        iso_string.replace("T"," ");
        iso_string = iso_string.substring(0, 19);
        
        time_t out = SSR::sun_string_to_time(iso_string);
        return out;
    }

    time_t SSR::rtc_time {0};
    timeval SSR::current_time {0};

    bool SSR::set_RTC_time()
    {
        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("WiFi connection failed, cannot set RTC time.");
            WIFI::wifi_start();
            return false;
        } 

        // actual time download 
        HTTPClient http;
        http.begin("https://aisenseapi.com/services/v1/datetime");
        int httpCode = http.GET();
        if(httpCode == 200) // code - OK
        {
            payload = http.getString();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            
            String rtc_string = doc["datetime"];

            Serial.print("Date time set up: ");
            Serial.println(rtc_string);

            rtc_time = iso_string_to_time(rtc_string);
            current_time = { .tv_sec = rtc_time };
            settimeofday(&current_time, NULL);
        }
        else
        {
            Serial.println("There was error during datetime request.");
            return false;
        }
        http.end();
        WIFI::wifi_end();
        return true;
    }
    
    time_t SSR::now {0};

    void SSR::get_RTC_time()
    {
        now = time(NULL);
        if (now != (time_t)(-1))
        {
            Serial.printf("The current time is %s(%jd seconds since the Epoch)\n",
               asctime(gmtime(&now)), // time as date : YYYY-MM-DD HH:MM:SS
               (intmax_t)now);  // seconds from 1970
        }
        else
        {
            Serial.println("Error during reading RTC time, check if time is properly set.");
        }
    }

    SSR::Sun_t SSR::suntime {0};

    bool SSR::sunrise_sunset_set_time()
    {

        if (WiFi.status() != WL_CONNECTED)
        {
            Serial.println("WiFi connection failed, cannot set sunrise and sunset time.");
            WIFI::wifi_start();
            return false;
        }

        HTTPClient http;
        http.begin("https://api.sunrise-sunset.org/json?lat=50.029468&lng=22.013083&formatted=0");
        // lattitude of your area -> lat= 
        // longtitude of your area -> lng= 

        int httpCode = http.GET();
        if(httpCode == 200) // code -> OK
        {
            payload = http.getString();

            DynamicJsonDocument doc(2048);
            deserializeJson(doc, payload);

            String sunrise = doc["results"]["sunrise"];
            String sunset = doc["results"]["sunset"];

            suntime.sunrise = iso_string_to_time(sunrise);
            suntime.sunset = iso_string_to_time(sunset);
            
            Serial.printf("Sunrise for today: %s, ",
                asctime(gmtime(&suntime.sunrise)));
            Serial.printf("Sunset : %s \n",
                asctime(gmtime(&suntime.sunset)));
        }
        else
        {
            Serial.println("There was error during sunrise and sunset request.");
            return false;
        }
        http.end();
        WIFI::wifi_end();
        return true;
    }

    time_t SSR::diff_sunrise {0};
    time_t SSR::diff_sunset {0};
    tm SSR::date {0};
    uint64_t SSR::time_to_next_check {0};

    bool SSR::set_hw_timer()
    {
        now = time(NULL); 
        localtime_r(&now, &date);
        
        if(sunrise_sunset_set_time())
        {
            diff_sunrise = suntime.sunrise - now; 
            diff_sunset = suntime.sunset - now;
            
            Serial.printf("Time (sec) for next sunrise: %s, for sunset %s",
                (String)(diff_sunrise),
                (String)(diff_sunset)
            );
            Serial.println();

            GLOBALS::TIME_SET = now;

            if (diff_sunrise > 0)
            {
                Serial.printf("Setting hardware timer for sunrise in %s seconds.\n", (String)(diff_sunrise));
                if (diff_sunrise > 1800) // 30 min earlier launch
                {
                    GLOBALS::TIME_SET_FOR_TIMER = diff_sunrise - 1800;
                    //HW_TIMER::hw_timer_2_activate((time_t)(diff_sunrise - 1800));
                }
                else
                {
                    GLOBALS::TIME_SET_FOR_TIMER = diff_sunrise;
                    //HW_TIMER::hw_timer_2_activate(diff_sunrise);
                }
                Mode = 1; // sunrise set activate
            }
            else if (diff_sunset > -720)
            {
                Serial.printf("Setting hardware timer for sunset in %s seconds.\n", (String)(diff_sunset + 720));
                GLOBALS::TIME_SET_FOR_TIMER = diff_sunset + 720;
                //HW_TIMER::hw_timer_2_activate((time_t)(diff_sunset + 720)); // set launch later for 720 seconds
                Mode = 2; // sunset set activate
            }
            else
            {
                time_to_next_check = (uint64_t)25 * 3600 - (uint64_t)date.tm_hour * 3600;
                Serial.printf("Both sunrise and sunset times for today have passed. Setting hardware timer for next checking in %lld seconds.\n", time_to_next_check);
                if (time_to_next_check <= 0) {
                    // next day
                    Serial.println("error minus day adjustment");
                    time_to_next_check += 24 * 3600;
                    GLOBALS::TIME_SET_FOR_TIMER = time_to_next_check + 24 * 3600;
                }
                else
                {
                    GLOBALS::TIME_SET_FOR_TIMER = time_to_next_check;
                    //HW_TIMER::hw_timer_2_activate(time_to_next_check);
                }
                Mode = 0; // unsettled time, wait for (26h - now) time to get new sunrise and sunset times
            }
            return true;
        }
        else
        {
            Serial.println("Sunrise and sunset time not set, cannot set hardware timer.");
            return false;
        }   
    }

    // TASKS

    void SSR::SSR_task(void* parameters)
    {   
        vTaskDelay(pdMS_TO_TICKS(5000)); // wait for 5 sec to let WiFi and OTA tasks finish their setup
        while(!set_RTC_time())
        {
            Serial.println("RTC time NOT set, next try for 5 sec.");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        get_RTC_time();
        while(!set_hw_timer())
        {
            Serial.println("Hardware timer NOT set, next try for 5 sec.");
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        GLOBALS::SSR_DONE = 1; // indicate that SSR task has finished initial setup

        while (1)
        {
            xTaskNotifyWait(0, 0, &signal, portMAX_DELAY);
            if (Mode == 0)
            {
                while(!set_RTC_time())
                {
                    Serial.println("RTC time NOT set, next try for 5 sec.");
                    vTaskDelay(pdMS_TO_TICKS(5000));
                }
                get_RTC_time();
                while(!set_hw_timer())
                {
                    Serial.println("Hardware timer NOT set, next try for 5 sec.");
                    vTaskDelay(pdMS_TO_TICKS(5000));
                }
            }
            else 
            {
                xQueueSend(QUEUE::SSRQueue, &Mode, 0);
                vTaskDelay(pdMS_TO_TICKS(5000));
                while(!set_hw_timer())
                {
                    Serial.println("Hardware timer NOT set, next try for 5 sec.");
                    vTaskDelay(pdMS_TO_TICKS(5000));
                }
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
            GLOBALS::SSR_DONE = 1; // indicate that SSR task has finished
        }
    }

    void SSR::SSR_task_init(unsigned portBASE_TYPE stack_size, UBaseType_t priority)
    {
        xTaskCreate
        (
            SSR::SSR_task,
            "SunsetandSunriseReadingTask",
            stack_size,
            NULL,
            priority,
            &SSR::ssr_task_handle
        );
    }
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
}