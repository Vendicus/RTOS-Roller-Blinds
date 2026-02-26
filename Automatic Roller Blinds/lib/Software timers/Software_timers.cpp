<<<<<<< HEAD
#include <Arduino.h>
#include <Software_timers.h>
#include <Pins.h>
#include <Wifi_config.h>
#include <WiFi.h>
#include <global.h>

namespace SOFTWARE_TIMERS 
{ 
    // Software timer handles definitions
    TimerHandle_t AfterManualMode_timer = NULL;
    TimerHandle_t EmergencyMode_timer = NULL;
    TimerHandle_t OTAMode_timer = NULL;
    TimerHandle_t EncoderListener_timer = NULL;

    bool AfterManualMode_timer_active = false;

    void software_timers_init(uint32_t AfterManualMode_timer_ms_period, uint32_t EmergencyMode_timer_ms_period, uint64_t OTAMode_timer_ms_period, uint32_t EncoderListener_timer_ms_period)
    {
        // Create software timers here, initial those at void setup() in main.ino
        
        // Timer to handle actions after manual mode and slow down operation change if needed
        AfterManualMode_timer = xTimerCreate(
            "AfterManualModeTimer",                                // Timer name
            pdMS_TO_TICKS(AfterManualMode_timer_ms_period),        // Timer period in ticks (5 seconds default)
            pdFALSE,                                               // Auto-reload (pdFALSE = one-shot)
            (void*)0,                                              // Timer ID
            AfterManualMode_timer_callback                         // Callback function
        );

        if (AfterManualMode_timer == NULL)
        {
            Serial.println("AfterManualMode Timer could not be created");
        }
        else
        {
            Serial.println("AfterManualMode Timer created successfully");
        }

        // Timer to handle emergency stop mode actions (like LED blinking)
        EmergencyMode_timer = xTimerCreate(
            "EmergencyModeTimer",                           // Timer name
            pdMS_TO_TICKS(EmergencyMode_timer_ms_period),   // Timer period in ticks (500 ms default)
            pdTRUE,                                         // Auto-reload (pdTRUE = periodic)
            (void*)1,                                       // Timer ID
            EmergencyMode_timer_callback                    // Callback function
        );

        if (EmergencyMode_timer == NULL)
        {
            Serial.println("EmergencyMode Timer could not be created");
        }
        else
        {
            Serial.println("EmergencyMode Timer created successfully");
        }

        // Timer to handle OTA mode timeout
        OTAMode_timer = xTimerCreate(
            "OTAModeTimer",                           // Timer name
            pdMS_TO_TICKS(OTAMode_timer_ms_period),   // Timer period in ticks (5 minutes default)
            pdFALSE,                                  // Auto-reload (pdFALSE = one-shot)
            (void*)2,                                 // Timer ID
            OTAMode_timer_callback                    // Callback function
        );

        if (OTAMode_timer == NULL)
        {
            Serial.println("OTAMode Timer could not be created");
        }
        else
        {
            Serial.println("OTAMode Timer created successfully");
        }

        // Timer to handle Encoder Listener actions, safety check in automatic mode
        EncoderListener_timer = xTimerCreate(
            "EncoderListenerTimer",                                           // Timer name
            pdMS_TO_TICKS(EncoderListener_timer_ms_period),                   // Timer period in ticks (example 100 ms)
            pdFALSE,                                                          // Auto-reload (pdFALSE = one-shot)
            (void*)3,                                                         // Timer ID
            EncoderListener_timer_callback                                    // Callback function
        );

        if (EncoderListener_timer == NULL)
        {
            Serial.println("EncoderListener Timer could not be created");
        }
        else
        {
            Serial.println("EncoderListener Timer created successfully");
        }

    }

    /*
    Timer used for halting changes in machine state after manual mode has been activated. 
    Example - user used buttons to steer motors directly, but at the same time the automatic mode is going to change the state.
    Timer will block such changes for a certain period of time, launching from last user interaction. 
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    void AfterManualMode_timer_callback(TimerHandle_t xTimer){
        AfterManualMode_timer_active = false;
    }
    
    bool led_state = false;
    /*
    Timer used for emergency stop mode handling.
    Used for signaling with D2 led PIN blinking and other emergency stop related actions.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    void EmergencyMode_timer_callback(TimerHandle_t xTimer)
    {
        led_state = !led_state;
        digitalWrite(PIN::LED_PIN, led_state ? HIGH : LOW);
    }

    /*
    Timer used for automatic wifi OTA check during start. It will suspend OTA task after schedulded time from start of ESP. 
    Its main purpose is to prevent large amount of electricity usage during wifi mode on and problems with backup power source during that time.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    void OTAMode_timer_callback(TimerHandle_t xTimer)
    {
        // Suspend OTA task after timer expires
        if (WIFI::otaTaskHandle != NULL)
        {
            vTaskSuspend(WIFI::otaTaskHandle);
            WIFI::wifi_end(); // Disable WiFi to save power
            GLOBALS::OTA_DONE = 1; // Indicate that OTA period is done
            Serial.println("Suspending OTA task due to OTAMode timer expiry");
        }
        else
        {
            Serial.println("OTA Task Handle is NULL, cannot suspend OTA task");
        }
    }

    /*
    Timer used for monitoring time of automatic mode operation of motors with encoders. If automatic mode is active for too long,
    it may indicate a problem with encoders or motors (like being stuck). This timer callback will handle such situations, signaling an error and shutting down automatic motor operation.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    void EncoderListener_timer_callback(TimerHandle_t xTimer)
    {
        Serial.println("ERROR! Motor Encoders not responding or there is a problem with motors! shutting down automatic motor operation!");
    }
=======
#include <Arduino.h>
#include <Software_timers.h>
#include <Pins.h>
#include <Wifi_config.h>
#include <WiFi.h>
#include <global.h>

namespace SOFTWARE_TIMERS 
{ 
    // Software timer handles definitions
    TimerHandle_t AfterManualMode_timer = NULL;
    TimerHandle_t EmergencyMode_timer = NULL;
    TimerHandle_t OTAMode_timer = NULL;
    TimerHandle_t EncoderListener_timer = NULL;

    bool AfterManualMode_timer_active = false;

    void software_timers_init(uint32_t AfterManualMode_timer_ms_period, uint32_t EmergencyMode_timer_ms_period, uint64_t OTAMode_timer_ms_period, uint32_t EncoderListener_timer_ms_period)
    {
        // Create software timers here, initial those at void setup() in main.ino
        
        // Timer to handle actions after manual mode and slow down operation change if needed
        AfterManualMode_timer = xTimerCreate(
            "AfterManualModeTimer",                                // Timer name
            pdMS_TO_TICKS(AfterManualMode_timer_ms_period),        // Timer period in ticks (5 seconds default)
            pdFALSE,                                               // Auto-reload (pdFALSE = one-shot)
            (void*)0,                                              // Timer ID
            AfterManualMode_timer_callback                         // Callback function
        );

        if (AfterManualMode_timer == NULL)
        {
            Serial.println("AfterManualMode Timer could not be created");
        }
        else
        {
            Serial.println("AfterManualMode Timer created successfully");
        }

        // Timer to handle emergency stop mode actions (like LED blinking)
        EmergencyMode_timer = xTimerCreate(
            "EmergencyModeTimer",                           // Timer name
            pdMS_TO_TICKS(EmergencyMode_timer_ms_period),   // Timer period in ticks (500 ms default)
            pdTRUE,                                         // Auto-reload (pdTRUE = periodic)
            (void*)1,                                       // Timer ID
            EmergencyMode_timer_callback                    // Callback function
        );

        if (EmergencyMode_timer == NULL)
        {
            Serial.println("EmergencyMode Timer could not be created");
        }
        else
        {
            Serial.println("EmergencyMode Timer created successfully");
        }

        // Timer to handle OTA mode timeout
        OTAMode_timer = xTimerCreate(
            "OTAModeTimer",                           // Timer name
            pdMS_TO_TICKS(OTAMode_timer_ms_period),   // Timer period in ticks (5 minutes default)
            pdFALSE,                                  // Auto-reload (pdFALSE = one-shot)
            (void*)2,                                 // Timer ID
            OTAMode_timer_callback                    // Callback function
        );

        if (OTAMode_timer == NULL)
        {
            Serial.println("OTAMode Timer could not be created");
        }
        else
        {
            Serial.println("OTAMode Timer created successfully");
        }

        // Timer to handle Encoder Listener actions, safety check in automatic mode
        EncoderListener_timer = xTimerCreate(
            "EncoderListenerTimer",                                           // Timer name
            pdMS_TO_TICKS(EncoderListener_timer_ms_period),                   // Timer period in ticks (example 100 ms)
            pdFALSE,                                                          // Auto-reload (pdFALSE = one-shot)
            (void*)3,                                                         // Timer ID
            EncoderListener_timer_callback                                    // Callback function
        );

        if (EncoderListener_timer == NULL)
        {
            Serial.println("EncoderListener Timer could not be created");
        }
        else
        {
            Serial.println("EncoderListener Timer created successfully");
        }

    }

    /*
    Timer used for halting changes in machine state after manual mode has been activated. 
    Example - user used buttons to steer motors directly, but at the same time the automatic mode is going to change the state.
    Timer will block such changes for a certain period of time, launching from last user interaction. 
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    void AfterManualMode_timer_callback(TimerHandle_t xTimer){
        AfterManualMode_timer_active = false;
    }
    
    bool led_state = false;
    /*
    Timer used for emergency stop mode handling.
    Used for signaling with D2 led PIN blinking and other emergency stop related actions.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    void EmergencyMode_timer_callback(TimerHandle_t xTimer)
    {
        led_state = !led_state;
        digitalWrite(PIN::LED_PIN, led_state ? HIGH : LOW);
    }

    /*
    Timer used for automatic wifi OTA check during start. It will suspend OTA task after schedulded time from start of ESP. 
    Its main purpose is to prevent large amount of electricity usage during wifi mode on and problems with backup power source during that time.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    void OTAMode_timer_callback(TimerHandle_t xTimer)
    {
        // Suspend OTA task after timer expires
        if (WIFI::otaTaskHandle != NULL)
        {
            vTaskSuspend(WIFI::otaTaskHandle);
            WIFI::wifi_end(); // Disable WiFi to save power
            GLOBALS::OTA_DONE = 1; // Indicate that OTA period is done
            Serial.println("Suspending OTA task due to OTAMode timer expiry");
        }
        else
        {
            Serial.println("OTA Task Handle is NULL, cannot suspend OTA task");
        }
    }

    /*
    Timer used for monitoring time of automatic mode operation of motors with encoders. If automatic mode is active for too long,
    it may indicate a problem with encoders or motors (like being stuck). This timer callback will handle such situations, signaling an error and shutting down automatic motor operation.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    void EncoderListener_timer_callback(TimerHandle_t xTimer)
    {
        Serial.println("ERROR! Motor Encoders not responding or there is a problem with motors! shutting down automatic motor operation!");
    }
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
}