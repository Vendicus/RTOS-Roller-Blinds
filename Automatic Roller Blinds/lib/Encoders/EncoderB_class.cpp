#include <EncoderB_class.h>
#include <Arduino.h>
#include <Preferences.h>

namespace ENCODER {

int32_t EncoderB::max_count {0};
int32_t EncoderB::min_count {0};
int32_t EncoderB::encoder_count {0};
uint32_t EncoderB::can_id {0};
uint8_t EncoderB::canal_a {0};
uint8_t EncoderB::canal_b {0};

TaskHandle_t EncoderB::task_encoderB_handle = NULL;

void EncoderB::init(uint8_t encoderB_canal_a)
{
    encoder_count = 0;
    max_count = 200;
    min_count = -10;

    // read last values from flash
    encoder_count = flash.getInt("countB", 0);
    max_count = flash.getInt("maxB", 200);
    Serial.printf("MAX value read: %d\n", max_count);
    Serial.printf("ENCODER B val read :%d\n", encoder_count);

    canal_a = encoderB_canal_a;
    //canal_b = encoderB_canal_b;

    xTaskCreate(
        EncoderB::task_encoderB_reader,                 // Task function
        "EncoderBReader",                               // Name of the task
        1024,                                           // Stack size in bytes
        NULL,                                           // Task input parameter
        2,                                              // Priority of the task
        &ENCODER::EncoderB::task_encoderB_handle        // Task handle
    );
}

/**
 * @return current encoder count.
 */
int32_t EncoderB::get_count()
{
    return encoder_count;
}

/**
 * @brief Reset the encoder count to zero and save to flash new value.
 */
void EncoderB::reset_count()
{
    encoder_count = 0;
    if (ENCODER::EncoderB::get_count() != flash.getInt("countB",false))
    {
        flash.putInt("countB", ENCODER::EncoderB::get_count()); // save encoder count to flash
    }
}

/**
 * @brief Set the upper limit for the encoder count and save to flash new value.
 * @param initial_count the upper limit value to set.
 */
void EncoderB::set_up_limit(int32_t initial_count)
{
    max_count = initial_count;
    // save in FLASH after val change
    if (EncoderB::get_up_limit() != flash.getInt("maxB",false))
    {
        flash.putInt("maxB", EncoderB::get_up_limit());
    }
}

/**
 * @return upper limit value of encoder.
 */
int32_t EncoderB::get_up_limit()
{
    return max_count;
}

/**
 * @return lower limit value of encoder.
 */
int32_t EncoderB::get_down_limit()
{
    return min_count;
}

/**
 * @brief Set the lower limit for the encoder count.
 * @param initial_count the lower limit value to set.
 */
void EncoderB::set_down_limit(int32_t initial_count)
{
    min_count = initial_count;
}

/**
 * @return true if the encoder count has reached or exceeded the upper limit, false otherwise.
 */
bool EncoderB::at_up_limit()
{   
    return encoder_count >= max_count;
}

/**
 * @return true if the encoder count has reached or fallen below the lower limit, false otherwise.
 */
bool EncoderB::at_down_limit()
{
    return encoder_count <= min_count;
}

/**
 * @brief Task function to read the encoder signals and update the encoder count.
 */
void EncoderB::task_encoderB_reader(void* parameter)
{
    while (1)
    {
        xTaskNotifyWait(0, 0, &can_id, portMAX_DELAY);
        if (digitalRead(canal_b) == LOW)
        {
            encoder_count++;
        }
        else
        {
            encoder_count--;
        }
    }
}
}
