#ifndef LIGHT_AND_LED_H
#define LIGHT_AND_LED_H

#include "global_variables.h"
#include <Adafruit_NeoPixel.h>

void TaskLightAndLED(void *pvParameters);

// External reference to NeoPixel object for MQTT control
extern Adafruit_NeoPixel pixels3;

// Light control functions
void setLightState(bool enabled);
bool getLightState();
void publishLightState(bool lightEnabled, const char* source);
const char* getLightMode();
void debugLightStates(); // Debug function

#endif
