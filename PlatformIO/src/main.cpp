#include "global_variables.h"
#include "tasks/led_control.h"
#include "tasks/temperature_humidity.h"
#include "tasks/soil_moisture.h"
#include "tasks/light_and_led.h"
#include "tasks/wifi_manager.h"
#include "tasks/fan_control.h"
#include "tasks/mqtt_manager.h"

static const char* TAG = "MAIN";

// Global sensor variables for MQTT publishing
volatile float currentTemperature = 0.0;
volatile float currentHumidity = 0.0;
volatile int currentSoilMoisture = 0;
volatile int currentLightLevel = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(GPIO_NUM_11, GPIO_NUM_12);
  
  // Create WiFi task with higher priority
  xTaskCreate(TaskWiFiManager, "WiFi Manager", 4096, NULL, 5, NULL);
  
  // Create MQTT task (depends on WiFi)
  // xTaskCreate(TaskMQTTManager, "MQTT Manager", 4096, NULL, 5, NULL);
  xTaskCreate(mqttTask, "MQTT Manager", 4096, NULL, 5, NULL);
  xTaskCreate(publisherTask, "MQTT Publisher", 4096, NULL, 5, NULL);
  // Create other tasks with increased stack sizes for ESP_LOG compatibility
  xTaskCreate(TaskLEDControl, "LED Control", 4096, NULL, 4, NULL);
  xTaskCreate(TaskTemperature_Humidity, "Temp Humidity", 4096, NULL, 4, NULL);
  xTaskCreate(TaskSoilMoisture, "Soil Moisture", 4096, NULL, 4, NULL);
  xTaskCreate(TaskLightAndLED, "Light and LED Control", 4096, NULL, 4, NULL);
  xTaskCreate(TaskFanControl, "Fan Control", 4096, NULL, 4, NULL);
  
  ESP_LOGI(TAG, "All tasks created successfully");
}

void loop() {
  
}