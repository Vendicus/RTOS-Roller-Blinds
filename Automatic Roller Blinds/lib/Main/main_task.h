#pragma once
#include <Arduino.h>

namespace MAIN_TASK
{
    extern TaskHandle_t mainTaskHandle;
    extern uint32_t ManualModeValue;
    extern uint8_t SSRModeValue;
    
    extern uint8_t local_mode;
    extern uint8_t serial_latch;
    extern uint8_t counter;

    void main_task(void* parameter);
    void main_task_init(unsigned portBASE_TYPE stack_size = 2048, UBaseType_t priority = 1);
}