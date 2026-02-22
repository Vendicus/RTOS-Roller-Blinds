#include <Arduino.h>
#include <Pins.h>
#include "driver/rtc_io.h"

namespace PIN{
    #define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // Macro for individual GPIO bitmask
    
    // Button pins
    const uint8_t Button_up {15};       // GPIO pin for UP button
    const uint8_t Button_down {5};      // GPIO pin for DOWN button
    const uint8_t Button_Em_stop {4};   // GPIO pin for EMERGENCY STOP button

    const uint8_t Relay_pin {12};       //GPIO pin for relay control

    // Motor A pins
    const uint8_t MotorA_enable {14};   // GPIO pin for Motor A ENABLE
    const uint8_t MotorA_in1 {27};      // GPIO pin for Motor A IN1
    const uint8_t MotorA_in2 {26};      // GPIO pin for Motor A IN2

    // Motor B pins
    const uint8_t MotorB_enable {25};   // GPIO pin for Motor B ENABLE
    const uint8_t MotorB_in1 {33};      // GPIO pin for Motor B IN1
    const uint8_t MotorB_in2 {32};      // GPIO pin for Motor B IN2

    // Encoders pins
    const uint8_t EncoderA_canalA {18};     // GPIO pin for Encoder A canal A
    const uint8_t EncoderB_canalA {19};     // GPIO pin for Encoder B canal A 
    //const uint8_t EncoderA_canalB {22};   // GPIO pin for Encoder A canal B
    //const uint8_t EncoderB_canalB {23};     // GPIO pin for Encoder B canal B

    // Backup pin
    const uint8_t Voltage_monitor_pin{22};   // GPIO pin used as signal for power lose 

    // LED pin for safety mode
    const uint8_t LED_PIN {2};          // GPIO pin for LED indication - D2
    
    void init_pins()
    {
        // Initialize button pins
        pinMode(Button_up, INPUT_PULLUP);
        pinMode(Button_down, INPUT_PULLUP);
        pinMode(Button_Em_stop, INPUT_PULLUP);

        // Initialize Motors pins
        pinMode(MotorA_enable, OUTPUT);
        pinMode(MotorA_in1, OUTPUT);
        pinMode(MotorA_in2, OUTPUT);
        pinMode(MotorB_enable, OUTPUT);
        pinMode(MotorB_in1, OUTPUT);
        pinMode(MotorB_in2, OUTPUT);

        // initialize LED pin for emergency mode indication
        pinMode(LED_PIN, OUTPUT);
        pinMode(Relay_pin, OUTPUT);

        // Initialize Encoders pins
        pinMode(EncoderA_canalA, INPUT_PULLUP);
        //pinMode(EncoderA_canalB, INPUT_PULLUP);
        pinMode(EncoderB_canalA, INPUT_PULLUP);
        //pinMode(EncoderB_canalB, INPUT_PULLUP);

        // Initialize Backup monitor pin
        pinMode(Voltage_monitor_pin, INPUT_PULLUP); //use resistor to 3.3V (10 KOhm)

        // Configure wakeup sources for light sleep
        esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);
        //esp_sleep_enable_ext0_wakeup(static_cast<gpio_num_t>(Voltage_monitor_pin), 0); // Wake up on LOW signal on Voltage_monitor_pin
    }
}