#include <global.h>

namespace GLOBALS
{
    uint8_t MODE {0}; // machine state operating mode
    uint8_t sygnal {0};    // ID for UP button ISR
    uint8_t OTA_DONE {0};    // Flag indicating OTA update completion
    uint8_t SSR_DONE {0};    // Flag indicating SSR update completion
    time_t TIME_SET {0};     // Time when sunrise/sunset was set or wakeup period set
    time_t TIME_SET_FOR_TIMER {0}; // Time period for hardware timer wakeup
} // namespace name
