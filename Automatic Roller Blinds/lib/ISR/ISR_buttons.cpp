<<<<<<< HEAD
#include <Arduino.h>
#include <ISR_buttons.h>
#include <Button_Queue.h>
#include <global.h>

namespace ISR_BUTTONS 
{
    void IRAM_ATTR isr_button_up() {
        // Handle button up press with button up id == 1
        Button_up_id = 1;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(QUEUE::ButtonQueue, &Button_up_id, &xHigherPriorityTaskWoken);

        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void IRAM_ATTR isr_button_down() {
        // Handle button down press with button down id == 2
        Button_down_id = 2;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(QUEUE::ButtonQueue, &Button_down_id, &xHigherPriorityTaskWoken);

        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void IRAM_ATTR isr_button_stop() {
        // Handle button stop press with button stop id == 3
        Button_stop_id = 3;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(QUEUE::ButtonQueue, &Button_stop_id, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void attach_button_ISRs(const uint8_t pin_up, const uint8_t pin_down, const uint8_t pin_stop) {
        attachInterrupt(digitalPinToInterrupt(pin_up), isr_button_up, CHANGE); //signal CHANGE to detect both press and release
        attachInterrupt(digitalPinToInterrupt(pin_down), isr_button_down, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pin_stop), isr_button_stop, CHANGE);
    }


}
=======
#include <Arduino.h>
#include <ISR_buttons.h>
#include <Button_Queue.h>
#include <global.h>

namespace ISR_BUTTONS 
{
    void IRAM_ATTR isr_button_up() {
        // Handle button up press with button up id == 1
        Button_up_id = 1;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(QUEUE::ButtonQueue, &Button_up_id, &xHigherPriorityTaskWoken);

        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void IRAM_ATTR isr_button_down() {
        // Handle button down press with button down id == 2
        Button_down_id = 2;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(QUEUE::ButtonQueue, &Button_down_id, &xHigherPriorityTaskWoken);

        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void IRAM_ATTR isr_button_stop() {
        // Handle button stop press with button stop id == 3
        Button_stop_id = 3;
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xQueueSendFromISR(QUEUE::ButtonQueue, &Button_stop_id, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void attach_button_ISRs(const uint8_t pin_up, const uint8_t pin_down, const uint8_t pin_stop) {
        attachInterrupt(digitalPinToInterrupt(pin_up), isr_button_up, CHANGE); //signal CHANGE to detect both press and release
        attachInterrupt(digitalPinToInterrupt(pin_down), isr_button_down, CHANGE);
        attachInterrupt(digitalPinToInterrupt(pin_stop), isr_button_stop, CHANGE);
    }


}
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
