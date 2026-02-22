#pragma once
#include <Arduino.h>
#include <Preferences.h>

extern Preferences flash;

namespace ENCODER {
class EncoderA
{
  public:
    static void init(uint8_t encoderA_canal_a);

    static void set_up_limit(int32_t initial_count);

    static void set_down_limit(int32_t initial_count);

    static int32_t get_count();

    static void reset_count();

    static bool at_up_limit();

    static bool at_down_limit();

    static int32_t get_up_limit();

    static int32_t get_down_limit();

    //TASKS
    static TaskHandle_t task_encoderA_handle;
    static void task_encoderA_reader(void* parameter);

  private:
    //encoder count variable
    static int32_t max_count, min_count, encoder_count;

    static uint32_t can_id;
    static uint8_t canal_a, canal_b;
};
}