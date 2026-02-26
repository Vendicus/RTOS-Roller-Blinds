<<<<<<< HEAD
#include <EncoderA_class.h>
#include <Arduino.h>
#include <Preferences.h>

Preferences flash;

namespace ENCODER {

int32_t EncoderA::max_count {0};
int32_t EncoderA::min_count {0};
int32_t EncoderA::encoder_count {0};
uint32_t EncoderA::can_id {0};
uint8_t EncoderA::canal_a {0};
uint8_t EncoderA::canal_b {0};

TaskHandle_t EncoderA::task_encoderA_handle = NULL;

void EncoderA::init(uint8_t encoderA_canal_a)
{
    encoder_count = 0;
    max_count = 200;
    min_count = -10;

    // read last values from flash
    encoder_count = flash.getInt("count", 0);
    max_count = flash.getInt("max", 200);
    Serial.printf("MAX value read: %d\n", max_count);
    Serial.printf("ENCODER A val read :%d\n", encoder_count);

    canal_a = encoderA_canal_a;
    //canal_b = encoderA_canal_b;

    xTaskCreate(
        EncoderA::task_encoderA_reader,                 // Task function
        "EncoderAReader",                               // Name of the task
        1024,                                           // Stack size in bytes
        NULL,                                           // Task input parameter
        2,                                              // Priority of the task
        &ENCODER::EncoderA::task_encoderA_handle        // Task handle
    );
}

/**
 * @return current encoder count.
 */
int32_t EncoderA::get_count()
{
    return encoder_count;
}

/**
 * @brief Reset the encoder count to zero and save to flash new value.
 */
void EncoderA::reset_count()
{
    encoder_count = 0;
    if (ENCODER::EncoderA::get_count() != flash.getInt("count",false))
    {
        flash.putInt("count", ENCODER::EncoderA::get_count()); // save encoder count to flash
    }
}

/**
 * @brief Set the upper limit for the encoder count and save to flash new value.
 * @param initial_count the upper limit value to set.
 */
void EncoderA::set_up_limit(int32_t initial_count)
{
    max_count = initial_count;
    // save in FLASH after val change
    if (EncoderA::get_up_limit() != flash.getInt("max",false))
    {
        flash.putInt("max", EncoderA::get_up_limit());
    }
}

/**
 * @return upper limit value of encoder.
 */
int32_t EncoderA::get_up_limit()
{
    return max_count;
}

/**
 * @return lower limit value of encoder.
 */
int32_t EncoderA::get_down_limit()
{
    return min_count;
}

/**
 * @brief Set the lower limit for the encoder count.
 * @param initial_count the lower limit value to set.
 */
void EncoderA::set_down_limit(int32_t initial_count)
{
    min_count = initial_count;
}

/**
 * @return true if the encoder count has reached or exceeded the upper limit, false otherwise.
 */
bool EncoderA::at_up_limit()
{   
    return encoder_count >= max_count;
}

/**
 * @return true if the encoder count has reached or fallen below the lower limit, false otherwise.
 */
bool EncoderA::at_down_limit()
{
    return encoder_count <= min_count;
}

/**
 * @brief Task function to read the encoder signals and update the encoder count.
 */
void EncoderA::task_encoderA_reader(void* parameter)
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
=======
#include <EncoderA_class.h>
#include <Arduino.h>
#include <Preferences.h>

Preferences flash;

namespace ENCODER {

int32_t EncoderA::max_count {0};
int32_t EncoderA::min_count {0};
int32_t EncoderA::encoder_count {0};
uint32_t EncoderA::can_id {0};
uint8_t EncoderA::canal_a {0};
uint8_t EncoderA::canal_b {0};

TaskHandle_t EncoderA::task_encoderA_handle = NULL;

void EncoderA::init(uint8_t encoderA_canal_a)
{
    encoder_count = 0;
    max_count = 200;
    min_count = -10;

    // read last values from flash
    encoder_count = flash.getInt("count", 0);
    max_count = flash.getInt("max", 200);
    Serial.printf("MAX value read: %d\n", max_count);
    Serial.printf("ENCODER A val read :%d\n", encoder_count);

    canal_a = encoderA_canal_a;
    //canal_b = encoderA_canal_b;

    xTaskCreate(
        EncoderA::task_encoderA_reader,                 // Task function
        "EncoderAReader",                               // Name of the task
        1024,                                           // Stack size in bytes
        NULL,                                           // Task input parameter
        2,                                              // Priority of the task
        &ENCODER::EncoderA::task_encoderA_handle        // Task handle
    );
}

/**
 * @return current encoder count.
 */
int32_t EncoderA::get_count()
{
    return encoder_count;
}

/**
 * @brief Reset the encoder count to zero and save to flash new value.
 */
void EncoderA::reset_count()
{
    encoder_count = 0;
    if (ENCODER::EncoderA::get_count() != flash.getInt("count",false))
    {
        flash.putInt("count", ENCODER::EncoderA::get_count()); // save encoder count to flash
    }
}

/**
 * @brief Set the upper limit for the encoder count and save to flash new value.
 * @param initial_count the upper limit value to set.
 */
void EncoderA::set_up_limit(int32_t initial_count)
{
    max_count = initial_count;
    // save in FLASH after val change
    if (EncoderA::get_up_limit() != flash.getInt("max",false))
    {
        flash.putInt("max", EncoderA::get_up_limit());
    }
}

/**
 * @return upper limit value of encoder.
 */
int32_t EncoderA::get_up_limit()
{
    return max_count;
}

/**
 * @return lower limit value of encoder.
 */
int32_t EncoderA::get_down_limit()
{
    return min_count;
}

/**
 * @brief Set the lower limit for the encoder count.
 * @param initial_count the lower limit value to set.
 */
void EncoderA::set_down_limit(int32_t initial_count)
{
    min_count = initial_count;
}

/**
 * @return true if the encoder count has reached or exceeded the upper limit, false otherwise.
 */
bool EncoderA::at_up_limit()
{   
    return encoder_count >= max_count;
}

/**
 * @return true if the encoder count has reached or fallen below the lower limit, false otherwise.
 */
bool EncoderA::at_down_limit()
{
    return encoder_count <= min_count;
}

/**
 * @brief Task function to read the encoder signals and update the encoder count.
 */
void EncoderA::task_encoderA_reader(void* parameter)
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
>>>>>>> d16706ff4e9ebbc9ce0bdd258acfc55d981d48ea
}