<<<<<<< HEAD
#include <Arduino.h>
#include <ISR_encoders.h>
#include <global.h>
#include <EncoderA_class.h>
#include <EncoderB_class.h>

namespace ISR_ENCODERS
{
    void IRAM_ATTR isr_encA_canA() {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(ENCODER::EncoderA::task_encoderA_handle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void IRAM_ATTR isr_encB_canA() {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(ENCODER::EncoderB::task_encoderB_handle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }
     
    void attach_encoders_ISRs(const uint8_t encoderA_canal_a, const uint8_t encoderB_canal_a) {
        attachInterrupt(digitalPinToInterrupt(encoderA_canal_a), isr_encA_canA, FALLING); 
        attachInterrupt(digitalPinToInterrupt(encoderB_canal_a), isr_encB_canA, FALLING);
    }
=======
#include <Arduino.h>
#include <ISR_encoders.h>
#include <global.h>
#include <EncoderA_class.h>
#include <EncoderB_class.h>

namespace ISR_ENCODERS
{
    void IRAM_ATTR isr_encA_canA() {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(ENCODER::EncoderA::task_encoderA_handle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }

    void IRAM_ATTR isr_encB_canA() {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xTaskNotifyFromISR(ENCODER::EncoderB::task_encoderB_handle, 1, eSetValueWithOverwrite, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    }
     
    void attach_encoders_ISRs(const uint8_t encoderA_canal_a, const uint8_t encoderB_canal_a) {
        attachInterrupt(digitalPinToInterrupt(encoderA_canal_a), isr_encA_canA, FALLING); 
        attachInterrupt(digitalPinToInterrupt(encoderB_canal_a), isr_encB_canA, FALLING);
    }
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
}