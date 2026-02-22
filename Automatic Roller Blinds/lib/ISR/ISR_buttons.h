#pragma once
#include <Arduino.h>

// ISR declarations for button handling
namespace ISR_BUTTONS {
    static uint8_t Button_up_id;
    static uint8_t Button_down_id;
    static uint8_t Button_stop_id;

    void IRAM_ATTR isr_button_up();
    void IRAM_ATTR isr_button_down();
    void IRAM_ATTR isr_button_stop();

    void attach_button_ISRs(const uint8_t pin_up, const uint8_t pin_down, const uint8_t pin_stop);
}