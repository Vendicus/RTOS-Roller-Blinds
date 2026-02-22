#include <Arduino.h>
#include <main_task.h>
#include <button_task.h>
#include <MotorA_class.h>
#include <MotorB_class.h>
#include <Software_timers.h>
#include <ISR_buttons.h>
#include <Button_Queue.h>
#include <Mutex_mode.h>
#include <global.h>
#include <pins.h>
#include <EncoderA_class.h>
#include <EncoderB_class.h>
#include <ISR_encoders.h>
#include <SSR_Queue.h>
#include <backup_task.h>
#include <ISR_backup_mode.h>
#include <SSR_task.h>
#include <HW_timer.h>
#include <Preferences.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Wifi_config.h>
#include <time.h>

void setup() 
{
  // run once
  Serial.begin(115200);
  vTaskDelay(pdMS_TO_TICKS(2000));
  Serial.println("Automatic Roller Blinds - System Initializing...");

  //PREFENCES BEGIN
  flash.begin("backup", false);

  // PINS MODE INITIALIZATION
  PIN::init_pins();
  
  // ISRS and HW TIMERS INITIALIZATION
  ISR_BUTTONS::attach_button_ISRs(PIN::Button_up, PIN::Button_down, PIN::Button_Em_stop); 
  ISR_ENCODERS::attach_encoders_ISRs(PIN::EncoderA_canalA, PIN::EncoderB_canalA);
  ISR_BACKUP::attach_backup_isr(PIN::Voltage_monitor_pin);
  //HW_TIMER::attach_timers(80);

  // QUEUES, MUTEXES, TIMERS INITIALIZATION
  SOFTWARE_TIMERS::software_timers_init();
  MUTEX::MUTEXmode_init();
  QUEUE::ButtonQueue_init();
  QUEUE::SRRQueue_init();

  // OTA INITIALIZATION
  WIFI::wifi_start();
  WIFI::ota_start();

  vTaskDelay(pdMS_TO_TICKS(100));

  // TASKS INITIALIZATION
  BUTTON_TASK::button_task_init(PIN::Button_up, PIN::Button_down, PIN::Button_Em_stop); 
  BACKUP_TASK::backup_task_init(4096, 4);
  MOTOR::MotorA::init(PIN::MotorA_enable, PIN::MotorA_in1, PIN::MotorA_in2);
  MOTOR::MotorB::init(PIN::MotorB_enable, PIN::MotorB_in1, PIN::MotorB_in2);
  ENCODER::EncoderA::init(PIN::EncoderA_canalA);
  ENCODER::EncoderB::init(PIN::EncoderB_canalA);
  SSR::SSR::SSR_task_init(8192, 2);
  MAIN_TASK::main_task_init();
  WIFI::ota_task_init();
}

void loop() {}

