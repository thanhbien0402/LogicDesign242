#include "tasks/wifi_manager.h"

static const char* TAG = "WIFI_MANAGER";

WebServer server(80);
Preferences preferences;

const char* AP_SSID = "ESP32-Setup";
const char* AP_PASSWORD = "12345678";

void TaskWiFiManager(void *pvParameters) {
    ESP_LOGI(TAG, "WiFi Manager Task Started");
    
    preferences.begin("wifi", false);
      if (connectToSavedWiFi()) {
        ESP_LOGI(TAG, "Connected to saved WiFi");
        while(1) {
            if (WiFi.status() != WL_CONNECTED) {
                ESP_LOGW(TAG, "WiFi disconnected, trying to reconnect...");
                if (!connectToSavedWiFi()) {
                    startAccessPoint();
                }
            }
            vTaskDelay(pdMS_TO_TICKS(10000));
        }
    } else {
        ESP_LOGI(TAG, "Starting setup mode");
        startAccessPoint();
        
        while(1) {
            server.handleClient();
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}

bool connectToSavedWiFi() {
    String savedSSID = preferences.getString("ssid", "");
    String savedPassword = preferences.getString("password", "");
      if (savedSSID.length() == 0) {
        ESP_LOGW(TAG, "No saved WiFi credentials");
        return false;
    }
    
    ESP_LOGI(TAG, "Connecting to: %s", savedSSID.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(savedSSID.c_str(), savedPassword.c_str());
      int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        ESP_LOGI(TAG, "Connecting... attempt %d", attempts + 1);
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        ESP_LOGI(TAG, "WiFi connected! IP: %s", WiFi.localIP().toString().c_str());
        return true;
    } else {
        ESP_LOGE(TAG, "Failed to connect to WiFi");
        return false;
    }
}

void startAccessPoint() {
    ESP_LOGI(TAG, "Starting AP mode");
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    
    ESP_LOGI(TAG, "AP IP: %s", WiFi.softAPIP().toString().c_str());
      server.on("/", handleRoot);
    server.on("/scan", handleScan);
    server.on("/connect", HTTP_POST, handleConnect);
    server.on("/reset", [](){
        clearWiFiCredentials();
        String html = "<!DOCTYPE html><html><head><title>Reset Complete</title></head><body>";
        html += "<div class='container'>";
        html += "<h2>WiFi Settings Reset</h2>";
        html += "<p>All saved WiFi credentials have been cleared.</p>";
        html += "<p>Restarting device...</p>";
        html += "<script>setTimeout(function(){fetch('/restart');}, 2000);</script>";
        html += "</div></body></html>";
        server.send(200, "text/html", html);
    });
    server.onNotFound(handleNotFound);
      server.begin();
    ESP_LOGI(TAG, "Web server started");
}

void handleRoot() {
    String html = "<!DOCTYPE html><html><head>";
    html += "<title>ESP32 WiFi Setup</title>";
    html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<style>";
    html += "body{font-family:Arial,sans-serif;margin:0;padding:20px;background:#f0f0f0;}";
    html += ".container{max-width:400px;margin:0 auto;background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);}";
    html += "h1{color:#333;text-align:center;margin-bottom:30px;}";
    html += "input,button{width:100%;padding:12px;margin:8px 0;border:1px solid #ddd;border-radius:5px;box-sizing:border-box;}";
    html += "button{background:#4CAF50;color:white;border:none;cursor:pointer;font-size:16px;}";
    html += "button:hover{background:#45a049;}";
    html += ".scan-btn{background:#2196F3;}";
    html += ".scan-btn:hover{background:#1976D2;}";
    html += ".network{background:#f9f9f9;padding:10px;margin:5px 0;border-radius:5px;cursor:pointer;border:1px solid #eee;}";
    html += ".network:hover{background:#e9e9e9;}";
    html += ".signal{float:right;color:#666;}";
    html += "</style></head><body>";
    html += "<div class='container'>";
    html += "<h1>WiFi Setup</h1>";
    html += "<div id='networks'>";
    html += "<button class='scan-btn' onclick='scanNetworks()'>Scan for Networks</button>";
    html += "</div>";    html += "<form action='/connect' method='POST'>";
    html += "<input type='text' name='ssid' id='ssid' placeholder='WiFi Network Name' required>";
    html += "<input type='password' name='password' placeholder='WiFi Password'>";
    html += "<button type='submit'>Connect to WiFi</button>";
    html += "</form>";
    html += "<div style='margin-top:20px;text-align:center;'>";
    html += "<button onclick='resetSettings()' style='background:#f44336;'>Reset WiFi Settings</button>";
    html += "</div>";    html += "<script>";
    html += "function scanNetworks(){";
    html += "document.getElementById('networks').innerHTML='<p>Scanning...</p>';";
    html += "fetch('/scan').then(r=>r.text()).then(d=>document.getElementById('networks').innerHTML=d);";
    html += "}";
    html += "function selectNetwork(ssid){document.getElementById('ssid').value=ssid;}";
    html += "function resetSettings(){if(confirm('Reset all WiFi settings?')){fetch('/reset').then(()=>location.reload());}}";
    html += "</script>";
    html += "</div></body></html>";
    
    server.send(200, "text/html", html);
}

void handleScan() {
    String networks = "<h3>Available Networks:</h3>";
    int n = WiFi.scanNetworks();
    
    if (n == 0) {
        networks += "<p>No networks found</p>";
    } else {
        for (int i = 0; i < n; i++) {
            String ssid = WiFi.SSID(i);
            int rssi = WiFi.RSSI(i);
            String encryption = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured";
            
            networks += "<div class='network' onclick='selectNetwork(\"" + ssid + "\")'>";
            networks += "<strong>" + ssid + "</strong>";
            networks += "<span class='signal'>" + String(rssi) + " dBm (" + encryption + ")</span>";
            networks += "</div>";
        }
    }
    
    networks += "<button class='scan-btn' onclick='scanNetworks()'>Scan Again</button>";
    
    server.send(200, "text/html", networks);
}

void handleConnect() {
    String ssid = server.arg("ssid");
    String password = server.arg("password");
    
    if (ssid.length() == 0) {
        String html = "<!DOCTYPE html><html><head><title>Error</title></head><body>";
        html += "<div class='container'>";
        html += "<h2 style='color:red;'>Error: SSID cannot be empty</h2>";
        html += "<p><a href='/'>Go Back</a></p>";
        html += "</div></body></html>";
        server.send(400, "text/html", html);
        return;
    }
    
    ESP_LOGI(TAG, "Attempting to connect to: %s", ssid.c_str());
    
    // Show connecting page
    String html = "<!DOCTYPE html><html><head><title>Connecting</title>";
    html += "<meta http-equiv='refresh' content='10;url=/status'>";
    html += "</head><body>";
    html += "<div class='container'>";
    html += "<h2>Connecting to WiFi...</h2>";
    html += "<p>Network: <strong>" + ssid + "</strong></p>";
    html += "<p>Please wait while we connect to the network.</p>";
    html += "<p>This page will refresh automatically in 10 seconds.</p>";
    html += "</div></body></html>";
    
    server.send(200, "text/html", html);
    
    // Try to connect
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
      int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        ESP_LOGI(TAG, "Connection attempt %d...", attempts + 1);
        attempts++;
    }
      if (WiFi.status() == WL_CONNECTED) {
        // Save credentials
        preferences.putString("ssid", ssid);
        preferences.putString("password", password);
        
        ESP_LOGI(TAG, "WiFi connected and saved! IP Address: %s", WiFi.localIP().toString().c_str());
        
        // Set up status endpoint for success
        server.on("/status", [](){
            String html = "<!DOCTYPE html><html><head><title>Success</title></head><body>";
            html += "<div class='container'>";
            html += "<h2 style='color:green;'>Connected Successfully!</h2>";
            html += "<p>Network: <strong>" + WiFi.SSID() + "</strong></p>";
            html += "<p>IP Address: <strong>" + WiFi.localIP().toString() + "</strong></p>";
            html += "<p>The device will restart in 5 seconds to apply settings.</p>";
            html += "<script>setTimeout(function(){fetch('/restart');}, 5000);</script>";
            html += "</div></body></html>";
            server.send(200, "text/html", html);
        });
        
        server.on("/restart", [](){
            server.send(200, "text/plain", "Restarting...");
            delay(1000);
            ESP.restart();
        });
          } else {
        ESP_LOGE(TAG, "Failed to connect to WiFi");
        
        // Set up status endpoint for failure
        server.on("/status", [](){
            String html = "<!DOCTYPE html><html><head><title>Connection Failed</title></head><body>";
            html += "<div class='container'>";
            html += "<h2 style='color:red;'>Connection Failed</h2>";
            html += "<p>Could not connect to the WiFi network.</p>";
            html += "<p>Please check the password and try again.</p>";
            html += "<p><a href='/'>Try Again</a></p>";
            html += "</div></body></html>";
            server.send(200, "text/html", html);
        });
    }
}

void handleNotFound() {
    String html = "<!DOCTYPE html><html><head><title>Page Not Found</title></head><body>";
    html += "<div class='container'>";
    html += "<h2>Page Not Found</h2>";
    html += "<p>The requested page could not be found.</p>";
    html += "<p><a href='/'>Go to WiFi Setup</a></p>";
    html += "</div></body></html>";
    
    server.send(404, "text/html", html);
}

void clearWiFiCredentials() {
    preferences.putString("ssid", "");
    preferences.putString("password", "");
    ESP_LOGI(TAG, "WiFi credentials cleared");
}
