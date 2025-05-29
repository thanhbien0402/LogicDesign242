#include "tasks/fan_control.h"
#include "global_variables.h"
#include <PubSubClient.h>

static const char* TAG = "FAN_CONTROL";

const int FAN_PIN = GPIO_NUM_6;
const int PWM_CHANNEL = 0;        // PWM channel for fan control
const int PWM_FREQUENCY = 25000;  // 25kHz PWM frequency
const int PWM_RESOLUTION = 8;     // 8-bit resolution (0-255)

// Simple temperature threshold for fan on/off
const float TEMP_FAN_ON = 30.0;     // Above this: fan on at default speed

// PWM duty cycle values (0-255)
const int SPEED_OFF = 0;
const int SPEED_DEFAULT = 120;  // Default slow speed (~47%)

// Global variables for fan control synchronization
static int currentFanSpeed = SPEED_OFF;      // Current fan PWM value
static bool lastAutoFanState = false;       // Last state set by auto control (for change detection)
static bool manualOverride = false;         // Flag to indicate manual control was used

// Forward declaration for MQTT client access
extern PubSubClient mqttClient;
extern const char* TOPIC_ATTRIBUTES;

// Function to publish fan state to MQTT (updates dashboard button)
void publishFanState(bool fanEnabled, float speedPercent, const char* source) {
    if (mqttClient.connected()) {
        String attributePayload = "{\"fanEnabled\": " + String(fanEnabled ? "true" : "false") + 
                                ", \"fanSpeed\": " + String(speedPercent) + "}";
        if (mqttClient.publish(TOPIC_ATTRIBUTES, attributePayload.c_str())) {
            ESP_LOGI(TAG, "Dashboard updated via MQTT (%s): Fan=%s, Speed=%.1f%%", 
                     source, fanEnabled ? "ON" : "OFF", speedPercent);
        } else {
            ESP_LOGE(TAG, "Failed to publish fan state to MQTT");
        }
    }
}

// Manual fan control functions (from dashboard button)
void setFanState(bool enabled) {
    int pwmValue;
    
    if (enabled) {
        pwmValue = SPEED_DEFAULT;
        ESP_LOGI(TAG, "Dashboard button: Fan turned ON at default speed");
    } else {
        pwmValue = SPEED_OFF;
        ESP_LOGI(TAG, "Dashboard button: Fan turned OFF");
    }
    
    // Set fan speed
    ledcWrite(PWM_CHANNEL, pwmValue);
    currentFanSpeed = pwmValue;
    
    // IMPORTANT: Set manual override and update auto state to prevent conflicts
    manualOverride = true;
    lastAutoFanState = enabled;
    ESP_LOGI(TAG, "Manual control: Updated lastAutoFanState to %s, manual override active", 
             enabled ? "ON" : "OFF");
    
    // Publish state change to MQTT (update dashboard button)
    float speedPercent = (pwmValue / 255.0) * 100.0;
    publishFanState(enabled, speedPercent, "DASHBOARD");
}

void setFanSpeed(int pwmValue) {
    pwmValue = constrain(pwmValue, 0, 255);
    
    // Set fan speed
    ledcWrite(PWM_CHANNEL, pwmValue);
    currentFanSpeed = pwmValue;
    
    bool fanEnabled = (pwmValue > 0);
    float speedPercent = (pwmValue / 255.0) * 100.0;
    
    // IMPORTANT: Set manual override and update auto state
    manualOverride = true;
    lastAutoFanState = fanEnabled;
    ESP_LOGI(TAG, "Manual speed control: PWM=%d (%.1f%%), manual override active", 
             pwmValue, speedPercent);
    
    // Publish state change to MQTT (update dashboard button)
    publishFanState(fanEnabled, speedPercent, "DASHBOARD");
}

bool getFanState() {
    return (currentFanSpeed > 0);
}

int getCurrentFanSpeed() {
    return currentFanSpeed;
}

// Debug function to check all states
void debugFanStates() {
    ESP_LOGI(TAG, "=== FAN DEBUG ===");
    ESP_LOGI(TAG, "currentFanSpeed: %d PWM", currentFanSpeed);
    ESP_LOGI(TAG, "lastAutoFanState: %s", lastAutoFanState ? "true" : "false");
    ESP_LOGI(TAG, "manualOverride: %s", manualOverride ? "true" : "false");
    ESP_LOGI(TAG, "Current State: %s", getFanState() ? "ON" : "OFF");
    ESP_LOGI(TAG, "Current Temperature: %.1f°C", currentTemperature);
    ESP_LOGI(TAG, "=================");
}

void TaskFanControl(void *pvParameters) {
    ESP_LOGI(TAG, "Fan Control Task Started");
    
    // Configure PWM for fan control
    ledcSetup(PWM_CHANNEL, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(FAN_PIN, PWM_CHANNEL);
    
    // Initialize variables
    currentFanSpeed = SPEED_OFF;
    lastAutoFanState = false;
    manualOverride = false;
    
    // Start with fan off
    ledcWrite(PWM_CHANNEL, SPEED_OFF);
    ESP_LOGI(TAG, "Fan task started in HYBRID mode (manual + auto)");
    
    // Publish initial state to dashboard
    publishFanState(false, 0.0, "INIT");
      
    while(1) {
        ESP_LOGI(TAG, "Temperature: %.1f°C, Current Fan: %s, Speed: %d PWM", 
                 currentTemperature, getFanState() ? "ON" : "OFF", currentFanSpeed);
        
        // HYBRID MODE: Always check temperature, but allow dashboard override
        bool autoWantsFanOn;
        
        // Auto logic based on temperature
        if (currentTemperature > TEMP_FAN_ON) {
            autoWantsFanOn = true;
            ESP_LOGI(TAG, "AUTO: > %.1f°C, wants fan ON", TEMP_FAN_ON);
        } else {
            autoWantsFanOn = false;
            ESP_LOGI(TAG, "AUTO: < %.1f°C, wants fan OFF", TEMP_FAN_ON);
        }
        
        // Update fan if auto state changed OR if enough time passed since manual override
        if (autoWantsFanOn != lastAutoFanState) {
            ESP_LOGI(TAG, "AUTO triggered state change: %s → %s", 
                     lastAutoFanState ? "ON" : "OFF", autoWantsFanOn ? "ON" : "OFF");
            
            // Update current fan state
            int newPwmValue = autoWantsFanOn ? SPEED_DEFAULT : SPEED_OFF;
            ledcWrite(PWM_CHANNEL, newPwmValue);
            currentFanSpeed = newPwmValue;
            
            if (autoWantsFanOn) {
                int percentage = (SPEED_DEFAULT * 100) / 255;
                ESP_LOGI(TAG, "AUTO: Turning fan ON at %d%% (temp: %.1f°C)", percentage, currentTemperature);
            } else {
                ESP_LOGI(TAG, "AUTO: Turning fan OFF (temp: %.1f°C)", currentTemperature);
            }
            
            // Update auto state and publish to dashboard
            lastAutoFanState = autoWantsFanOn;
            manualOverride = false; // Clear manual override after auto action
            float speedPercent = (newPwmValue / 255.0) * 100.0;
            publishFanState(autoWantsFanOn, speedPercent, "AUTO_TEMP");
        } else {
            ESP_LOGI(TAG, "AUTO: No temperature state change, staying %s", 
                     getFanState() ? "ON" : "OFF");
        }
        
        // Note: Manual control via dashboard button (setFanState/setFanSpeed) can override anytime
        // and the temperature sensor will continue monitoring for the next automatic change
        
        vTaskDelay(pdMS_TO_TICKS(5000)); // Check temperature every 5 seconds
    }
}
