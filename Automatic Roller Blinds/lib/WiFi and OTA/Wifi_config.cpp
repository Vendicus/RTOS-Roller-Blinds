#include <Arduino.h>
#include <WiFi.h>
#include <Wifi_config.h>
#include <Mutex_mode.h>
#include <EncoderA_class.h>
#include <Preferences.h>
#include <Software_timers.h>
#include <esp_wifi.h>

namespace WIFI{
    unsigned long wifi_connect_time {0};
    TaskHandle_t otaTaskHandle = NULL;

    bool wifi_start() 
    {
        xSemaphoreTake(MUTEX::MUTEXwifi, portMAX_DELAY);
        //esp_err_t results = esp_wifi_init(WIFI_INIT_CONFIG_DEFAULT);
        WiFi.mode(WIFI_STA);
        WiFi.begin("*******", "********"); // Replace with your WiFi credentials
        esp_err_t results = esp_wifi_start();

        Serial.println("WiFi start result: " + String(results));
        wifi_connect_time = millis();
        while (WiFi.status() != WL_CONNECTED)
        {
            if (millis() - wifi_connect_time > 10000) {
                Serial.println("WiFi timeout");
                results = esp_wifi_stop();
                //WiFi.disconnect(true);
                WiFi.mode(WIFI_OFF);
                xSemaphoreGive(MUTEX::MUTEXwifi);
                return false;
            }
            vTaskDelay(pdMS_TO_TICKS(100));
        }
        xSemaphoreGive(MUTEX::MUTEXwifi);
        return true;
    }

    void wifi_end()
    {
        xSemaphoreTake(MUTEX::MUTEXwifi, portMAX_DELAY);
        esp_err_t results = esp_wifi_stop();
        //WiFi.disconnect(true, true);
        WiFi.mode(WIFI_OFF);
        xSemaphoreGive(MUTEX::MUTEXwifi);
        Serial.println("WiFi stopped with result: " + String(results));
    }

    void ota_start()
    {
        ArduinoOTA.setHostname("RollerBlindsController");
        ArduinoOTA.setPassword("admin");

        ArduinoOTA.onStart([]() {
            // Before starting OTA, save encoder count to flash
            if (ENCODER::EncoderA::get_count() != flash.getInt("count",false))
            {
                flash.putInt("count", ENCODER::EncoderA::get_count());
            }

            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
        });
        ArduinoOTA.onEnd([]() {
            Serial.println("\nEnd");
        });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });
        ArduinoOTA.onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });

        ArduinoOTA.begin();
        Serial.println("OTA Ready");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
    }

    void ota_task_init(unsigned portBASE_TYPE stack_size, UBaseType_t priority)
    {
        xTaskCreate(
            ota_task,            // Task function
            "OTA_Task",          // Name of the task
            stack_size,          // Stack size in words
            NULL,                // Task input parameter
            priority,            // Priority of the task
            &otaTaskHandle       // Task handle
        );
    }

    void ota_task(void* parameter)
    {
        xTimerStart(SOFTWARE_TIMERS::OTAMode_timer, 0); // Start OTAMode timer
        while(true)
        {
            if (WiFi.status() == WL_CONNECTED)
            {
                ArduinoOTA.handle();
            }
            else
            {
                wifi_start();
            }   
            vTaskDelay(pdMS_TO_TICKS(200)); 
        }
    }

}