#pragma once
#include <Arduino.h>

// ISR declarations for encoder canal handling
namespace ISR_ENCODERS {

    void IRAM_ATTR isr_encA_canA();
    void IRAM_ATTR isr_encB_canA();
    
    void attach_encoders_ISRs(const uint8_t encoderA_canal_a, const uint8_t encoderB_canal_a);
}