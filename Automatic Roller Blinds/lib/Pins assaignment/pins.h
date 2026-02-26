#pragma once
#include <Arduino.h>

// Global Pin assignments for ESP32 microcontroller DEVKIT V1
namespace PIN{
    // Button pins
    extern const uint8_t Button_up;       // GPIO pin for UP button - S1
    extern const uint8_t Button_down;     // GPIO pin for DOWN button - S2
    extern const uint8_t Button_Em_stop;  // GPIO pin for EMERGENCY STOP button - SE

    // Motor A pins
    extern const uint8_t MotorA_enable;   // GPIO pin for Motor A ENABLE - L298N ENA PIN
    extern const uint8_t MotorA_in1;      // GPIO pin for Motor A IN1 - L298N IN1 PIN
    extern const uint8_t MotorA_in2;      // GPIO pin for Motor A IN2 - L298N IN2 PIN

    // Motor B pins
    extern const uint8_t MotorB_enable;   // GPIO pin for Motor B ENABLE - L298N ENB PIN
    extern const uint8_t MotorB_in1;      // GPIO pin for Motor B IN1 - L298N IN3 PIN
    extern const uint8_t MotorB_in2;      // GPIO pin for Motor B IN2 - L298N IN4 PIN

    // LED pin for safety mode
    extern const uint8_t LED_PIN;        // GPIO pin for LED indication - D2
    extern const uint8_t Relay_pin;      // GPIO pin for relay control - D12

    // Encoders pins
    extern const uint8_t EncoderA_canalA;   // GPIO pin for Encoder A canal A
    //extern const uint8_t EncoderA_canalB;   // GPIO pin for Encoder A canal B
    extern const uint8_t EncoderB_canalA;   // GPIO pin for Encoder B canal A
    //extern const uint8_t EncoderB_canalB;   // GPIO pin for Encoder B canal B

    // pin used for backup monitor
    extern const uint8_t Voltage_monitor_pin;   // GPIO pin used as signal for power lose  

    void init_pins();
}