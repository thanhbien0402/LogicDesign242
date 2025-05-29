#include "tasks/soil_moisture.h"
#include "global_variables.h"

static const char* TAG = "SOIL_MOISTURE";

void TaskSoilMoisture(void *pvParameters) {
  pinMode(GPIO_NUM_1, INPUT); // Initialize soil moisture sensor pin
  while(1) {
    // Read soil moisture sensor value
    int soilValue = analogRead(GPIO_NUM_1);
      // Update global variable for MQTT publishing
    currentSoilMoisture = soilValue;
    
    ESP_LOGI(TAG, "Soil Moisture: %d", soilValue);

    vTaskDelay(pdMS_TO_TICKS(10000)); // Delay for 1 second
  }
}
