#include "tasks/temperature_humidity.h"
#include "global_variables.h"

static const char* TAG = "TEMP_HUMIDITY";

DHT20 dht20;

void TaskTemperature_Humidity(void *pvParameters){
  dht20.begin();

  while(1) {    
    dht20.read();
    double temperature = dht20.getTemperature();
    double humidity = dht20.getHumidity();    // Update global sensor variables for MQTT and fan control
    currentTemperature = (float)temperature;
    currentHumidity = (float)humidity;

    // Use separate logs to reduce stack usage
    ESP_LOGI(TAG, "Temperature: %.1f °C", temperature);
    ESP_LOGI(TAG, "Humidity: %.1f %%", humidity);
   
    vTaskDelay(pdMS_TO_TICKS(5000)); // Use constant to avoid stack calculations
  }
}
