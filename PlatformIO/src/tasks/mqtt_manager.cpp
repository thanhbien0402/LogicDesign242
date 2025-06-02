#include "tasks/mqtt_manager.h"
#include "global_variables.h"
#include "tasks/fan_control.h"
#include "tasks/light_and_led.h"
#include "lcd.h"

static const char* TAG = "MQTT_MANAGER";

// ThingsBoard.io MQTT configuration
const char* MQTT_SERVER = "demo.thingsboard.io";
const int MQTT_PORT = 1883;
const char* MQTT_CLIENT_ID = "ESP32";
const char* MQTT_USERNAME = "aqeffw396e9r45c0w8th";  // Replace with your device access token
const char* MQTT_PASSWORD = "";  // ThingsBoard doesn't use password for device tokens

// ThingsBoard MQTT topics
const char* TOPIC_TELEMETRY = "v1/devices/me/telemetry";
const char* TOPIC_ATTRIBUTES = "v1/devices/me/attributes";
const char* TOPIC_RPC_REQUEST = "v1/devices/me/rpc/request/+";
const char* TOPIC_RPC_RESPONSE = "v1/devices/me/rpc/response/";

LiquidCrystal_I2C lcd(33, 16, 2);
// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void mqttCallback(char* topic, byte* payload, unsigned int length) {
    ESP_LOGI(TAG, "MQTT message received on topic: %s", topic);

    String message = "";
    for (unsigned int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    ESP_LOGI(TAG, "Message: %s", message.c_str());

    // Handle RPC requests
    String topicStr = String(topic);
    
    // Parse JSON message
    DynamicJsonDocument doc(512);
    DeserializationError error = deserializeJson(doc, message);
    
    if (error) {
        ESP_LOGE(TAG, "Failed to parse JSON: %s", error.c_str());
        return;
    }
      // Check if this is an RPC request
    if (topicStr.startsWith("v1/devices/me/rpc/request/")) {
        String method = doc["method"];
        
        // Extract request ID from topic
        String requestId = topicStr.substring(strlen("v1/devices/me/rpc/request/"));
        String responseTopic = "v1/devices/me/rpc/response/" + requestId;
        
        // Access fan control constants
        extern const int PWM_CHANNEL;
        extern const int SPEED_DEFAULT;
        extern const int SPEED_OFF;
          if (method == "setFanSpeed") {
            // Get speed parameter (0-100%)
            float speedPercent = doc["params"];
            
            // Convert percentage to PWM value (0-255)
            int pwmValue = (int)((speedPercent / 100.0) * 255);
            pwmValue = constrain(pwmValue, 0, 255);
            
            // Use new fan control function for synchronization
            setFanSpeed(pwmValue);
            
            ESP_LOGI(TAG, "Fan speed set to %.2f%% (PWM: %d)", speedPercent, pwmValue);
            
            // Send RPC response
            String response = "{\"success\": true, \"speedPercent\": " + String(speedPercent) + ", \"pwmValue\": " + String(pwmValue) + "}";
            
            if (mqttClient.publish(responseTopic.c_str(), response.c_str())) {
                ESP_LOGI(TAG, "Response sent successfully");
            } else {
                ESP_LOGE(TAG, "Failed to send response");
            }
        }
        else if (method == "setFan") {
            // Get boolean parameter for fan on/off
            bool fanEnabled = doc["params"];
            
            // Use new fan control function for synchronization
            setFanState(fanEnabled);
            
            ESP_LOGI(TAG, "Fan turned %s", fanEnabled ? "ON" : "OFF");
            
            // Send RPC response
            int currentPwm = getCurrentFanSpeed();
            float speedPercent = (currentPwm / 255.0) * 100.0;
            String response = "{\"success\": true, \"fanEnabled\": " + String(fanEnabled ? "true" : "false") + ", \"speedPercent\": " + String(speedPercent) + "}";
            if (mqttClient.publish(responseTopic.c_str(), response.c_str())) {
                ESP_LOGI(TAG, "Response sent successfully");
            } else {
                ESP_LOGE(TAG, "Failed to send response");
            }
        }
        else if (method == "setLight") {
            // Get boolean parameter for light on/off
            bool lightEnabled = doc["params"];
            
            // Call light control function
            setLightState(lightEnabled);
            
            ESP_LOGI(TAG, "Light turned %s", lightEnabled ? "ON" : "OFF");
            
            // Update attributes on server
            String attributePayload = "{\"lightEnabled\": " + String(lightEnabled ? "true" : "false") + "}";
            mqttClient.publish(TOPIC_ATTRIBUTES, attributePayload.c_str());
            ESP_LOGI(TAG, "Updated server state: lightEnabled=%s", lightEnabled ? "true" : "false");
              // Send RPC response
            String response = "{\"success\": true, \"lightEnabled\": " + String(lightEnabled ? "true" : "false") + "}";
            if (mqttClient.publish(responseTopic.c_str(), response.c_str())) {
                ESP_LOGI(TAG, "Light response sent successfully");
            } else {
                ESP_LOGE(TAG, "Failed to send light response");
            }
        }        else if (method == "debugLight") {
            // Debug function to check all light states
            debugLightStates();
            
            // Get all current states for response
            bool currentState = getLightState();
            int lightValue = analogRead(GPIO_NUM_2);
            
            ESP_LOGI(TAG, "Debug light states requested");
            
            // Send RPC response with debug info
            String response = "{\"success\": true, \"mode\": \"hybrid\", \"lightState\": " + 
                            String(currentState ? "true" : "false") + 
                            ", \"sensorValue\": " + String(lightValue) + "}";
            
            if (mqttClient.publish(responseTopic.c_str(), response.c_str())) {
                ESP_LOGI(TAG, "Debug light response sent successfully");
            } else {
                ESP_LOGE(TAG, "Failed to send debug light response");
            }
        }
        else if (method == "getLightState") {
            // Get current light state for dashboard query
            bool currentState = getLightState();
            int lightValue = analogRead(GPIO_NUM_2);
            
            ESP_LOGI(TAG, "Light state query: %s, sensor: %d", 
                     currentState ? "ON" : "OFF", lightValue);
            
            // Send RPC response with current light info
            String response = "{\"success\": true, \"lightEnabled\": " + 
                            String(currentState ? "true" : "false") + 
                            ", \"sensorValue\": " + String(lightValue) + 
                            ", \"mode\": \"hybrid\"}";
            
            if (mqttClient.publish(responseTopic.c_str(), response.c_str())) {
                ESP_LOGI(TAG, "Light state query response sent successfully");
            } else {
                ESP_LOGE(TAG, "Failed to send light state query response");
            }
        }else if (method == "debugFan") {
            // Debug function to check all fan states
            debugFanStates();
            
            // Get all current states for response
            bool currentState = getFanState();
            int currentSpeed = getCurrentFanSpeed();
            float speedPercent = (currentSpeed / 255.0) * 100.0;
            
            ESP_LOGI(TAG, "Debug fan states requested");
            
            // Send RPC response with debug info
            String response = "{\"success\": true, \"mode\": \"hybrid\", \"fanState\": " + 
                            String(currentState ? "true" : "false") + 
                            ", \"fanSpeed\": " + String(speedPercent) + 
                            ", \"temperature\": " + String(currentTemperature) + "}";
            
            if (mqttClient.publish(responseTopic.c_str(), response.c_str())) {
                ESP_LOGI(TAG, "Debug fan response sent successfully");
            } else {
                ESP_LOGE(TAG, "Failed to send debug fan response");
            }
        }
        else if (method == "getFanSpeed") {
            // Get current fan speed for dashboard query
            bool currentState = getFanState();
            int currentSpeed = getCurrentFanSpeed();
            float speedPercent = (currentSpeed / 255.0) * 100.0;
            
            ESP_LOGI(TAG, "Fan speed query: %s, %.1f%% (%d PWM)", 
                     currentState ? "ON" : "OFF", speedPercent, currentSpeed);
            
            // Send RPC response with current fan info
            String response = "{\"success\": true, \"fanEnabled\": " + 
                            String(currentState ? "true" : "false") + 
                            ", \"fanSpeed\": " + String(speedPercent) + 
                            ", \"pwmValue\": " + String(currentSpeed) + "}";
            
            if (mqttClient.publish(responseTopic.c_str(), response.c_str())) {
                ESP_LOGI(TAG, "Fan speed query response sent successfully");
            } else {
                ESP_LOGE(TAG, "Failed to send fan speed query response");
            }
        }
        else {
            ESP_LOGW(TAG, "Unknown method: %s", method.c_str());
            
            // Send error response
            String response = "{\"success\": false, \"error\": \"Unknown method: " + method + "\"}";
            mqttClient.publish(responseTopic.c_str(), response.c_str());
        }
    }
}

void mqttTask(void *pvParameters) {
    while (!WiFi.isConnected()) {
        ESP_LOGW(TAG, "Waiting for WiFi connection...");
        vTaskDelay(pdMS_TO_TICKS(1000));  // Wait for WiFi to connect
    }
    
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
    mqttClient.setCallback(mqttCallback);
    ESP_LOGI(TAG, "MQTT Client initialized");

    while (1) {
        if (!mqttClient.connected()) {
            ESP_LOGI(TAG, "Connecting to MQTT server...");
            while (!mqttClient.connected()) {
                if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
                    ESP_LOGI(TAG, "MQTT connected successfully");
                    
                    // Subscribe to RPC request topic
                    mqttClient.subscribe(TOPIC_RPC_REQUEST);
                    ESP_LOGI(TAG, "Subscribed to topic: %s", TOPIC_RPC_REQUEST);

                } else {
                    ESP_LOGE(TAG, "Failed to connect to MQTT server, retrying in 5 seconds...");
                    ESP_LOGE(TAG, "MQTT state: %d", mqttClient.state());
                    vTaskDelay(pdMS_TO_TICKS(5000));  // Wait before retrying
                }
            }
        }
        
        mqttClient.loop();  // Process incoming messages
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void publisherTask(void *pvParameters) {
    lcd.begin();
    lcd.backlight();
    while (1) {        if (mqttClient.connected()) {
            char payload[512];  // Increased buffer size for additional data
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Temp: ");
            lcd.print(currentTemperature, 2);  // Changed from 1 to 2 decimal places
            lcd.print(" *C");
            lcd.setCursor(0, 1);
            lcd.print("Humi: ");    
            lcd.print(currentHumidity, 2);     // Changed from 1 to 2 decimal places
            lcd.print(" %");            // Get current fan state using new functions
            bool fanEnabled = getFanState();
            int currentPWM = getCurrentFanSpeed();
            float currentFanSpeed = (currentPWM / 255.0) * 100.0;
            
            // Get current light state
            bool lightEnabled = getLightState();
            
            snprintf(payload, sizeof(payload), 
                "{\"temperature\":%.2f,\"humidity\":%.2f,\"soilMoisture\":%d,\"lightLevel\":%d,\"fanEnabled\":%s,\"fanSpeed\":%.2f,\"fanPWM\":%d,\"lightEnabled\":%s}",
                currentTemperature, currentHumidity, currentSoilMoisture, currentLightLevel,
                fanEnabled ? "true" : "false", currentFanSpeed, currentPWM, lightEnabled ? "true" : "false");
            
            ESP_LOGI(TAG, "Publishing telemetry data: %s", payload);

            if (mqttClient.publish(TOPIC_TELEMETRY, payload)) {
                ESP_LOGI(TAG, "Published successfully");
            } else {
                ESP_LOGE(TAG, "Failed to publish telemetry data");
            }
        } else {
            ESP_LOGW(TAG, "MQTT client not connected. Attempting to reconnect...");
        }

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

// Main MQTT Manager Task that creates and manages MQTT subtasks
void TaskMQTTManager(void *pvParameters) {
    ESP_LOGI(TAG, "MQTT Manager Task Started");
    
    // Create MQTT connection task
    xTaskCreate(mqttTask, "MQTT Client", 4096, NULL, 5, NULL);
    
    // Create publisher task
    xTaskCreate(publisherTask, "MQTT Publisher", 4096, NULL, 4, NULL);
    
    // This task monitors the MQTT connection and tasks
    while(1) {
        // Monitor MQTT connection status and log periodically
        if (mqttClient.connected()) {
            ESP_LOGI(TAG, "MQTT Status: Connected");
        } else {
            ESP_LOGW(TAG, "MQTT Status: Disconnected");
        }
        
        vTaskDelay(pdMS_TO_TICKS(30000)); // Check every 30 seconds
    }
}