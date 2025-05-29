#include "tasks/light_and_led.h"
#include "global_variables.h"
#include <PubSubClient.h>

static const char* TAG = "LIGHT_LED";

Adafruit_NeoPixel pixels3(4, GPIO_NUM_8, NEO_GRB + NEO_KHZ800);

// Global variables for light control
static bool currentLightState = false;  // Current light state (ON/OFF)
static bool lastSensorState = false;    // Last state set by sensor (for change detection)

// Forward declaration for MQTT client access
extern PubSubClient mqttClient;
extern const char* TOPIC_ATTRIBUTES;

// Function to publish light state to MQTT (updates dashboard button)
void publishLightState(bool lightEnabled, const char* source) {
    if (mqttClient.connected()) {
        String attributePayload = "{\"lightEnabled\": " + String(lightEnabled ? "true" : "false") + "}";
        if (mqttClient.publish(TOPIC_ATTRIBUTES, attributePayload.c_str())) {
            ESP_LOGI(TAG, "Dashboard updated via MQTT (%s): Light=%s", 
                     source, lightEnabled ? "ON" : "OFF");
        } else {
            ESP_LOGE(TAG, "Failed to publish light state to MQTT");
        }
    }
}

// Manual light control functions (from dashboard button)
void setLightState(bool enabled) {
    currentLightState = enabled;
    
    if (enabled) {
        // Turn on all LEDs to white
        for (int i = 0; i < 4; i++) {
            pixels3.setPixelColor(i, pixels3.Color(255, 255, 255));
        }
        ESP_LOGI(TAG, "Dashboard button: Light turned ON");
    } else {
        // Turn off all LEDs
        for (int i = 0; i < 4; i++) {
            pixels3.setPixelColor(i, pixels3.Color(0, 0, 0));
        }
        ESP_LOGI(TAG, "Dashboard button: Light turned OFF");
    }
    pixels3.show();
    
    // IMPORTANT: Update sensor state to match manual control
    // This prevents sensor from immediately trying to change the light back
    lastSensorState = enabled;
    ESP_LOGI(TAG, "Manual control: Updated lastSensorState to %s", enabled ? "ON" : "OFF");
    
    // Publish state change to MQTT (update dashboard button)
    publishLightState(enabled, "DASHBOARD");
}

bool getLightState() {
    return currentLightState;
}

// Function to get current light mode for debugging
const char* getLightMode() {
    return "HYBRID"; // Both manual and auto control active
}

// Debug function to check all states
void debugLightStates() {
    ESP_LOGI(TAG, "=== LIGHT DEBUG ===");
    ESP_LOGI(TAG, "currentLightState: %s", currentLightState ? "true" : "false");
    ESP_LOGI(TAG, "lastSensorState: %s", lastSensorState ? "true" : "false");
    ESP_LOGI(TAG, "Current Mode: %s", getLightMode());
    ESP_LOGI(TAG, "Current State: %s", getLightState() ? "ON" : "OFF");
    int lightValue = analogRead(GPIO_NUM_2);
    ESP_LOGI(TAG, "Current Sensor: %d", lightValue);
    ESP_LOGI(TAG, "===================");
}

void TaskLightAndLED(void *pvParameters) {
  pinMode(GPIO_NUM_2, INPUT); // Initialize light sensor pin
  pixels3.begin();
  
  // Initialize variables
  currentLightState = false;
  lastSensorState = false;
  ESP_LOGI(TAG, "Light task started in HYBRID mode (manual + auto)");
  
  // Publish initial state to dashboard
  publishLightState(currentLightState, "INIT");
  
  while(1) {
    // Read light sensor value
    int lightValue = analogRead(GPIO_NUM_2);
    // Update global variable for MQTT publishing
    currentLightLevel = lightValue;
    
    ESP_LOGI(TAG, "Light Sensor: %d, Current State: %s", 
             lightValue, getLightState() ? "ON" : "OFF");
      // HYBRID MODE: Always check sensor, but allow dashboard override
    bool sensorWantsLightOn;
    
    // Auto logic based on sensor with hysteresis
    if (lightValue < 300) {
        sensorWantsLightOn = true;
        ESP_LOGI(TAG, "SENSOR: < 300, wants light ON");
    } else if (lightValue > 500) {
        sensorWantsLightOn = false;
        ESP_LOGI(TAG, "SENSOR: > 500, wants light OFF");
    } else {
        // Hysteresis zone: keep the last sensor decision
        sensorWantsLightOn = lastSensorState;
        ESP_LOGI(TAG, "SENSOR: Hysteresis zone (300-500), keeping previous state: %s", 
                 sensorWantsLightOn ? "ON" : "OFF");
    }
    
    // Update light if sensor state changed
    if (sensorWantsLightOn != lastSensorState) {
        ESP_LOGI(TAG, "SENSOR triggered state change: %s → %s", 
                 lastSensorState ? "ON" : "OFF", sensorWantsLightOn ? "ON" : "OFF");
        
        // Update current light state
        currentLightState = sensorWantsLightOn;
        
        if (currentLightState) {
            // Turn on all LEDs to white
            for (int i = 0; i < 4; i++) {
                pixels3.setPixelColor(i, pixels3.Color(255, 255, 255));
            }
            ESP_LOGI(TAG, "AUTO: Turning lights ON (sensor: %d)", lightValue);
        } else {
            // Turn off all LEDs
            for (int i = 0; i < 4; i++) {
                pixels3.setPixelColor(i, pixels3.Color(0, 0, 0));
            }
            ESP_LOGI(TAG, "AUTO: Turning lights OFF (sensor: %d)", lightValue);
        }
        pixels3.show();
        
        // Update sensor state and publish to dashboard
        lastSensorState = sensorWantsLightOn;
        publishLightState(currentLightState, "AUTO_SENSOR");
    } else {
        ESP_LOGI(TAG, "AUTO: No sensor state change, staying %s", 
                 currentLightState ? "ON" : "OFF");
    }
    
    // Note: Manual control via dashboard button (setLightState) can override anytime
    // and the sensor will continue monitoring for the next automatic change

    vTaskDelay(pdMS_TO_TICKS(10000)); // Check sensor every 2 seconds
  }
}