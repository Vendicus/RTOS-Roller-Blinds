#pragma once
#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

namespace SOFTWARE_TIMERS 
{   
    /*
    Timer used for halting changes in machine state after manual mode has been activated. 
    Example - user used buttons to steer motors directly, but at the same time the automatic mode is going to change the state.
    Timer will block such changes for a certain period of time, launching from last user interaction. 
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    extern TimerHandle_t AfterManualMode_timer;
    /*
    Timer used for emergency stop mode handling.
    Used for signaling with D2 led PIN blinking and other emergency stop related actions.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    extern TimerHandle_t EmergencyMode_timer;
    /*
    Timer used for automatic wifi OTA check during start. It will suspend OTA task after schedulded time from start of ESP. 
    Its main purpose is to prevent large amount of electricity usage during wifi mode on and problems with backup power source during that time.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    extern TimerHandle_t OTAMode_timer;
    /*
    Timer used for monitoring time of automatic mode operation of motors with encoders. If automatic mode is active for too long,
    it may indicate a problem with encoders or motors (like being stuck). This timer callback will handle such situations, signaling an error and shutting down automatic motor operation.
    To change the period, re-initialize the timer with different period value in software_timers_init().
    */
    extern TimerHandle_t EncoderListener_timer;

    extern bool led_state; // led state var for emergency mode blinking, used in EmergencyMode_timer_callback
    extern bool AfterManualMode_timer_active; // flag indicating if AfterManualMode timer is active
    //180000
    void software_timers_init(uint32_t AfterManualMode_timer_ms_period = 5000, uint32_t EmergencyMode_timer_ms_period = 500, uint64_t OTAMode_timer_ms_period = 80000, uint32_t EncoderListener_timer_ms_period = 15000);
    void AfterManualMode_timer_callback(TimerHandle_t xTimer);
    void EmergencyMode_timer_callback(TimerHandle_t xTimer);
    void OTAMode_timer_callback(TimerHandle_t xTimer);
    void EncoderListener_timer_callback(TimerHandle_t xTimer);
}
