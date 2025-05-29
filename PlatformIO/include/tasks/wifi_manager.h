#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "global_variables.h"
#include <WebServer.h>
#include <Preferences.h>

void TaskWiFiManager(void *pvParameters);

// WiFi management functions
bool connectToSavedWiFi();
void startAccessPoint();
void clearWiFiCredentials();

// Web server handlers
void handleRoot();
void handleScan();
void handleConnect();
void handleNotFound();

#endif
