#include "tasks/led_control.h"

static const char* TAG = "LED_CONTROL";

void TaskLEDControl(void *pvParameters) {
  pinMode(GPIO_NUM_48, OUTPUT); // Initialize LED pin
  ESP_LOGI(TAG, "LED Control Task Started");
  int ledState = 0;
  while(1) {
    
    if (ledState == 0) {
      digitalWrite(GPIO_NUM_48, HIGH); // Turn ON LED
      // ESP_LOGI(TAG, "LED ON");
    } else {
      digitalWrite(GPIO_NUM_48, LOW); // Turn OFF LED
      // ESP_LOGI(TAG, "LED OFF");
    }
    ledState = 1 - ledState;
    vTaskDelay(pdMS_TO_TICKS(1000)); // Toggle every 1 second
  }
}