#pragma once
#include <Arduino.h>

namespace BACKUP_TASK
{
    extern TaskHandle_t backup_task_handle;
    extern uint32_t notify_val;
    //extern time_t backup_interval_s;

    void backup_task(void *parameter);
    void backup_task_init(unsigned portBASE_TYPE stack_size, UBaseType_t priority);
}