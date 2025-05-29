#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

// ==========================================
// COMMON INCLUDES FOR ALL TASKS
// ==========================================

// Core Arduino framework
#include <Arduino.h>

// ESP32 system includes
#include "esp_log.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "esp32-hal-log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Common libraries used across multiple tasks
#include <WiFi.h>
#include <ArduinoJson.h>

// ==========================================
// GLOBAL SENSOR VARIABLES
// ==========================================

// Global sensor variables for sharing between tasks
// These are defined in main.cpp and used across multiple tasks
extern volatile float currentTemperature;
extern volatile float currentHumidity;
extern volatile int currentSoilMoisture;
extern volatile int currentLightLevel;

#endif // GLOBAL_VARIABLES_H
