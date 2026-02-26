#include <Arduino.h>
#include <backup_task.h>
#include <Preferences.h>
#include <EncoderA_class.h>
#include <EncoderB_class.h>
#include <Wifi_config.h>

namespace BACKUP_TASK
{
    TaskHandle_t backup_task_handle = NULL;
    uint32_t notify_val {0};

    void backup_task(void* parameter)
    {
        while(1)
        {
            xTaskNotifyWait(0, 0, &notify_val, portMAX_DELAY);
            
            if (ENCODER::EncoderA::get_count() != flash.getInt("count_A",false))
            {
                flash.putInt("count_A", ENCODER::EncoderA::get_count()); // save encoder count to flash
            }
            if (ENCODER::EncoderB::get_count() != flash.getInt("count_B",false))
            {
                flash.putInt("count_B", ENCODER::EncoderB::get_count()); // save encoder count to flash
            }
        }
    }

    void backup_task_init(unsigned portBASE_TYPE stack_size, UBaseType_t priority)
    {
        xTaskCreate(
            backup_task,
            "Backup Task",
            stack_size,
            NULL,
            priority,
            &backup_task_handle
        );
    }
}