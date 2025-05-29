#ifndef FAN_CONTROL_H
#define FAN_CONTROL_H

#include "global_variables.h"

// Fan control task
void TaskFanControl(void *pvParameters);

// Manual fan control functions (for dashboard button)
void setFanState(bool enabled);
void setFanSpeed(int pwmValue);
bool getFanState();
int getCurrentFanSpeed();

// Debug function
void debugFanStates();

// Global temperature variable for sharing between tasks
extern volatile float currentTemperature;

// Fan control constants
extern const int FAN_PIN;
extern const int PWM_CHANNEL;
extern const int PWM_FREQUENCY;
extern const int PWM_RESOLUTION;

// Temperature threshold and speed
extern const float TEMP_FAN_ON;
extern const int SPEED_OFF;
extern const int SPEED_DEFAULT;

#endif
