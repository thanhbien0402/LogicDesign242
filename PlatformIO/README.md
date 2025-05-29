
# ESP32 Smart Garden IoT System

A comprehensive IoT solution for smart garden monitoring and control using ESP32, integrated with ThingsBoard.io for cloud monitoring and remote control.

## 🌟 Features

### Sensor Monitoring
- **Temperature & Humidity**: DHT20 sensor with I2C interface
- **Soil Moisture**: Analog sensor for soil moisture detection
- **Light Level**: Photoresistor for ambient light monitoring
- **Real-time Display**: 16x2 LCD for local sensor readings

### Automated Control
- **Smart Fan Control**: Temperature-based fan control with PWM speed regulation
- **Intelligent Lighting**: NeoPixel LEDs with automatic light-based control
- **Remote Control**: MQTT/RPC commands for manual override

### Connectivity
- **WiFi Manager**: Web-based WiFi setup with credential storage
- **MQTT Integration**: ThingsBoard.io cloud platform connectivity
- **Real-time Telemetry**: 30-second interval sensor data publishing
- **Remote Commands**: Fan and LED control via ThingsBoard dashboard

## 🏗️ System Architecture

### Modular Design
```
src/tasks/
├── temperature_humidity.cpp    # DHT20 sensor & LCD display
├── soil_moisture.cpp          # Soil moisture monitoring  
├── light_and_led.cpp         # Light sensor & NeoPixel control
├── fan_control.cpp           # Temperature-based fan control
├── wifi_manager.cpp          # WiFi setup & management
└── mqtt_manager.cpp          # ThingsBoard.io integration
```

### Hardware Configuration
- **Temperature/Humidity**: DHT20 on I2C (GPIO 11 SDA, GPIO 12 SCL)
- **LCD Display**: 16x2 I2C LCD (Address: 0x33)
- **Fan Control**: PWM on GPIO 6 (25kHz frequency)
- **Soil Sensor**: Analog input on GPIO 1
- **Light Sensor**: Analog input on GPIO 2
- **NeoPixel LEDs**: 4 LEDs for status indication

## 🚀 Quick Start

### 1. Hardware Setup
1. Connect DHT20 sensor to I2C pins (GPIO 11, 12)
2. Connect LCD display to I2C bus
3. Connect fan to GPIO 6 with PWM control
4. Connect soil moisture sensor to GPIO 1
5. Connect light sensor to GPIO 2
6. Connect NeoPixel LEDs for status indication

### 2. Software Installation
1. Install PlatformIO IDE extension for VS Code
2. Clone this repository
3. Build and upload the firmware:
   ```bash
   pio run --target upload
   ```

### 3. WiFi Configuration
1. On first boot, ESP32 creates WiFi access point "SmartGarden_Setup"
2. Connect to AP and navigate to http://192.168.4.1
3. Scan and select your WiFi network
4. Enter credentials and save

### 4. ThingsBoard Setup
1. Follow the detailed guide in [THINGSBOARD_SETUP.md](THINGSBOARD_SETUP.md)
2. Create device on demo.thingsboard.io
3. Copy device access token to `src/tasks/mqtt_manager.cpp`
4. Create dashboard with telemetry and control widgets

## 📊 Data & Control

### Telemetry Data (Published every 30s)
```json
{
  "temperature": 25.6,
  "humidity": 65.2, 
  "soilMoisture": 450,
  "lightLevel": 650,
  "wifi_rssi": -45,
  "uptime": 3600
}
```

### Remote Control Commands
- **Fan Control**: Toggle on/off, set speed (0-255)
- **LED Control**: Set RGB color, toggle on/off
- **Real-time Response**: Immediate command execution

## 🔧 Technical Specifications

### Libraries Used
- **ESP32 Framework**: Arduino framework for ESP32
- **WiFi Management**: Custom web-based configuration
- **MQTT Client**: PubSubClient for ThingsBoard integration
- **JSON Processing**: ArduinoJson for data serialization
- **DHT20**: Temperature/humidity sensor library
- **NeoPixel**: Adafruit NeoPixel for LED control

### Task Priorities
- WiFi Manager: Priority 3 (Highest)
- MQTT Manager: Priority 2
- Sensor Tasks: Priority 2
- Control Tasks: Priority 2

### Performance
- **Sensor Sampling**: 1-second intervals
- **MQTT Publishing**: 30-second intervals
- **Fan Response**: Immediate temperature-based control
- **Memory Usage**: Optimized for ESP32 constraints

## 🛠️ Development

### Build Commands
```bash
# Build project
pio run

# Upload firmware
pio run --target upload

# Monitor serial output
pio device monitor

# Build filesystem (if using SPIFFS)
pio run --target buildfs
pio run --target uploadfs
```

### Task Configuration
Each task runs as a FreeRTOS task with configurable stack size and priority. Tasks communicate via global volatile variables for real-time sensor data sharing.

## 📈 Monitoring & Debugging

### Serial Monitor Output
- Sensor readings every second
- WiFi connection status
- MQTT connection and data publishing
- Command execution confirmations

### ThingsBoard Dashboard
- Real-time telemetry visualization
- Historical data charts
- Remote control interface
- Device status monitoring

## 🔧 Troubleshooting

### Common Issues
1. **WiFi Connection**: Check credentials, signal strength
2. **MQTT Connection**: Verify ThingsBoard access token
3. **Sensor Readings**: Check I2C connections and power
4. **Fan Control**: Verify PWM pin configuration

### Debug Features
- Comprehensive serial logging
- LED status indicators
- Web-based WiFi diagnostics
- MQTT connection status reporting

## 📄 License

This project is part of the ACLAB-HCMUT IoT starter kit and follows the laboratory's usage guidelines.

## 🤝 Contributing

Developed for educational purposes at ACLAB-HCMUT. For improvements or issues, please contact the laboratory team.

---

<a href="https://github.com/ACLAB-HCMUT"><img src="https://raw.githubusercontent.com/ACLAB-HCMUT/Common/main/Assets/ACLAB_IMG_1.png" alt="ACLAB logo" title="ACLAB" align="right" height="100" /></a>
