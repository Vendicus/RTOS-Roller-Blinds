#include <Arduino.h>
#pragma once

namespace BUTTON_TASK
{
    extern TaskHandle_t buttonTaskHandle;
    extern uint8_t receivedButtonID;

    static uint8_t Pin_up;
    static uint8_t Pin_down;
    static uint8_t Pin_stop;

    void button_task(void* parameter);
    void button_task_init(const uint8_t pin_up, const uint8_t pin_down, const uint8_t pin_stop, unsigned portBASE_TYPE stack_size = 1024, UBaseType_t priority = 2);
}