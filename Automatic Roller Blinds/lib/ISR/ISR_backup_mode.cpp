#include <Arduino.h>
#include <ISR_backup_mode.h>
#include <backup_task.h>

namespace ISR_BACKUP
{
    void IRAM_ATTR isr_backup_mode() {     
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(BACKUP_TASK::backup_task_handle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void attach_backup_isr(const uint8_t pin)
    {
        attachInterrupt(digitalPinToInterrupt(pin), isr_backup_mode, RISING); //signal falling to detect low value 
    }

}