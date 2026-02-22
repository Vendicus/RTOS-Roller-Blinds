#include <Arduino.h>
#include <MotorB_class.h>
#include <global.h>
#include <Mutex_mode.h>

namespace MOTOR {

uint8_t MotorB::ena = 0;
uint8_t MotorB::in1 = 0;
uint8_t MotorB::in2 = 0;

uint16_t MotorB::pwm_cyc = 0;
bool MotorB::backward_state = false;

uint8_t MotorB::local_mode = 0;
uint32_t MotorB::buttonID = 0;

TaskHandle_t MotorB::task_motorB_controller_handle = NULL;

void MotorB::init(uint8_t enable_signal, uint8_t in_1, uint8_t in_2)
{
    ena = enable_signal;
    in1 = in_1;
    in2 = in_2;

    pwm_cyc = 0;
    backward_state = false;
    local_mode = 0;
    buttonID = 0;

    xTaskCreate(
        MotorB::task_motor_controller,                  // Task function
        "MotorBController",                             // Name of the task
        4096,                                           // Stack size in bytes
        NULL,                                           // Task input parameter
        1,                                              // Priority of the task
        &MOTOR::MotorB::task_motorB_controller_handle   // Task handle
    );
}

/** 
 * @brief Start the motor with optional soft start.
 *
 *     @param soft_start if true, the motor will gradually increase speed to the target PWM value.
 *     @param backward if true, the motor will run in the backward direction.
 *     @param pwm_max maximum PWM value to reach during soft start.
 *     @param pwm_min minimum PWM value to start from during soft start.
 *     @param pwm_increment_value value by which PWM is increased in each step during soft start.
 * 
 *     @return true if the motor has reached the target PWM value, false otherwise.
 */
bool MotorB::start(bool soft_start, bool backward, unsigned short int pwm_max, unsigned short int pwm_min, unsigned short int pwm_increment_value)
{
    backward_state = backward;
    if (soft_start)
    {
        (pwm_cyc < pwm_min) ? pwm_cyc = pwm_min : pwm_cyc;
        if ((pwm_cyc < 255) | (pwm_cyc <= pwm_max)) 
        {
            MotorB::pwm_increase(pwm_increment_value);
            return false;
        }
        return true;
    }
    else
    {
        pwm_cyc = 0;
        digitalWrite(ena, HIGH);
        digitalWrite(in1, backward ? LOW : HIGH);
        digitalWrite(in2, backward ? HIGH : LOW); 
        return true;
    }
}

/**
 * @brief Increases the PWM cycle by a specified increment value.
 * @param pwm_increment_value The value to increase the PWM cycle by. Default is 10. Check start function to change.
 */
void MotorB::pwm_increase(unsigned short int pwm_increment_value )
{
    ((pwm_cyc + pwm_increment_value) <= 255) ? pwm_cyc+=pwm_increment_value : pwm_cyc=255;
    analogWrite(ena, pwm_cyc);
    digitalWrite(in1, backward_state ? LOW : HIGH);
    digitalWrite(in2, backward_state ? HIGH : LOW);
}

/**
 * @brief Stops the motor with optional soft stop.
 * @param soft_stop If true, the motor will be stopped softly by reducing PWM to zero. If false, the motor will be stopped immediately.
 */
void MotorB::stop(bool soft_stop)
{
    MotorB::pwm_reset();
    (soft_stop) ? digitalWrite(ena, LOW) : digitalWrite(ena, HIGH);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
}

/**
 * @brief Gets the current PWM cycle value.
 * @return The current PWM cycle value.
 */
unsigned short int MotorB::get_pwm_cycle()
{
    return pwm_cyc;
}

/**
 * @brief Resets the PWM cycle to zero and stops the motor.
 */
void MotorB::pwm_reset()
{
    analogWrite(ena, 0);
    pwm_cyc = 0;
}

// ------> FreeRTOS TASKS <-------
/**
 * @brief Task function to control the motor based on the current mode.
 */
void MotorB::task_motor_controller(void* parameter)
{
    while (1)
    {
        xSemaphoreTake(MUTEX::MUTEXmode, portMAX_DELAY);
        (local_mode != GLOBALS::MODE && local_mode != 3) ? local_mode = GLOBALS::MODE : local_mode;       
        xSemaphoreGive(MUTEX::MUTEXmode);
        switch(local_mode)
        {
        
        // *********** initial state after launch of microcontroller and after work of roller blinds is done *************
        case 0: 
            MotorB::stop(true);  //soft stop  
            vTaskSuspend(NULL);        
            break;

        // *********** manual steering mode *************
        case 1: 
            xTaskNotifyWait(0, 0, &buttonID, 0);
            if (buttonID == 1) // UP button
            {
                MotorB::start(true, false, 255, 50, 5);
            }
            else if (buttonID == 2) // DOWN button
            {
                MotorB::start(true, true, 255, 50, 5);
            }
            break;
        
        // *********** emergency stop mode *************
        case 3:
            MotorB::stop(false); // hard stop
            vTaskSuspend(NULL);
            break;
        
        // *********** automatic modes (20,30) *************
        // * motor going UP with soft start *
        case 10:
            MotorB::start(true, false, 255, 50, 5);
            break;
        // * motor going DOWN with soft start *
        case 20:
            MotorB::start(true, true, 255, 50, 5);
            break;
        
        default:
            MotorB::stop(false);
            vTaskSuspend(NULL);
            break;
        }

        vTaskDelay(50 / portTICK_PERIOD_MS); // DELAY 50 MS
    }
}
}