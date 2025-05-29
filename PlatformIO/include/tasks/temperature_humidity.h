#ifndef TEMPERATURE_HUMIDITY_H
#define TEMPERATURE_HUMIDITY_H

#include "global_variables.h"
#include <DHT20.h>
#include <lcd.h>

void TaskTemperature_Humidity(void *pvParameters);

// External reference to global temperature variable
extern volatile float currentTemperature;

#endif