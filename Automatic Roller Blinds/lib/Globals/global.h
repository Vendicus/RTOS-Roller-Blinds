<<<<<<< HEAD
#pragma once
#include <Arduino.h>

// Global definitions and declarations
namespace GLOBALS{
    extern uint8_t MODE; // machine state operating mode
    extern uint8_t OTA_DONE;    // Flag indicating OTA update completion
    extern uint8_t SSR_DONE;    // Flag indicating SSR update completion
    extern time_t TIME_SET;     // current time
    extern time_t TIME_SET_FOR_TIMER; // Time period for hardware timer wakeup
}
=======
#pragma once
#include <Arduino.h>

// Global definitions and declarations
namespace GLOBALS{
    extern uint8_t MODE; // machine state operating mode
    extern uint8_t OTA_DONE;    // Flag indicating OTA update completion
    extern uint8_t SSR_DONE;    // Flag indicating SSR update completion
    extern time_t TIME_SET;     // current time
    extern time_t TIME_SET_FOR_TIMER; // Time period for hardware timer wakeup
}
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
