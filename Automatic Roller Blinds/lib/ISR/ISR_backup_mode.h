#pragma once
#include <Arduino.h>

namespace ISR_BACKUP
{
    void isr_backup_mode();
    void attach_backup_isr(const uint8_t pin);
}
