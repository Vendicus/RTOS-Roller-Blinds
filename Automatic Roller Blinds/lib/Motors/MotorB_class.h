#pragma once
#include <Arduino.h>

namespace MOTOR {

class MotorB {
  public:
    static void init(uint8_t enable_signal, uint8_t in_1, uint8_t in_2);

    static bool start(bool soft_start = false, bool backward = false, unsigned short int pwm_max = 255, unsigned short int pwm_min = 0, unsigned short int pwm_increment_value = 10);

    static void stop(bool soft_stop);
    
    static unsigned short int get_pwm_cycle();

    static void pwm_increase(unsigned short int pwm_increment_value = 10);

    static void pwm_reset();

    // TASKS
    static TaskHandle_t task_motorB_controller_handle;
    static void task_motor_controller(void* parameter);

  private:
    // motor controlller L298N pins
    static uint8_t ena, in1, in2;

    // motor pwm and direction state
    static uint16_t pwm_cyc;
    static bool backward_state;
    
    // local copy of global mode variable
    static uint8_t local_mode;

    // for manual mode operation
    static uint32_t buttonID;
};
}