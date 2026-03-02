#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <ESP32Encoder.h>

extern Preferences flash;

namespace ENCODER {
class EncoderA
{
  public:
    static void init();

    static void set_up_limit(int64_t initial_count);

    static void set_down_limit(int64_t initial_count);

    static int64_t get_count();

    static void reset_count();

    static bool at_up_limit();

    static bool at_down_limit();

    static int64_t get_up_limit();

    static int64_t get_down_limit();

    //TASKS
    //static TaskHandle_t task_encoderA_handle;
    //static void task_encoderA_reader(void* parameter);

  private:
    //encoder count variable
    static int64_t max_count, min_count, encoder_count;

    static uint32_t can_id;
};
}