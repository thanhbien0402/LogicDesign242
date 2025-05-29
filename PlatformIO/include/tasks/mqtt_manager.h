#ifndef MQTT_MANAGER_H
#define MQTT_MANAGER_H

#include "global_variables.h"
#include <PubSubClient.h>

// MQTT task function
void TaskMQTTManager(void *pvParameters);

// MQTT connection and callback functions
void connectToMQTT();
void mqttCallback(char* topic, byte* payload, unsigned int length);
void publishSensorData();
void subscribeToTopics();
void mqttTask(void *pvParameters);
void publisherTask(void *pvParameters);

// MQTT status
extern bool mqttConnected;

#endif
