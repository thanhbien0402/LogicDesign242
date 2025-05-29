# Smart Device Control System using RTOS on IoTS Platform

## Table of Contents
1. [Introduction](#introduction)
2. [Background](#background)
3. [System Design](#system-design)
    3.1. [Overall System Architecture](#overall-system-architecture)
    3.2. [Hardware Design](#hardware-design)
    3.3. [Software Architecture](#software-architecture)
    3.4. [Communication Protocols](#communication-protocols)
    3.5. [Control Logic](#control-logic)
    3.6. [Data Flow](#data-flow)
4. [System Implementation](#system-implementation)
    4.1. [WiFi Manager Task](#wifi-manager-task)
    4.2. [MQTT Manager Task](#mqtt-manager-task)
    4.3. [Sensor Tasks](#sensor-tasks)
    4.4. [Control Tasks](#control-tasks)
    4.5. [Local Display and Configuration](#local-display-and-configuration)
5. [Conclusion](#conclusion)
    5.1. [Achievements](#achievements)
    5.2. [Limitations](#limitations)
    5.3. [Future Work](#future-work)

---

## 1. Introduction

The "Smart Device Control System using RTOS on an IoT Platform" is an advanced project focused on creating an intelligent, automated environment management system. This particular implementation, an ESP32-based Smart Garden IoT System, serves as a comprehensive solution for monitoring and controlling a smart garden environment. The system leverages the power of the ESP32 microcontroller, a variety of environmental sensors, and automated control mechanisms, all integrated with a cloud-based IoT platform (ThingsBoard.io) for remote monitoring and management.

The core of the system is built around the ESP32, a versatile System on Chip (SoC) known for its dual-core processing capabilities, integrated Wi-Fi and Bluetooth, and ample GPIOs, making it ideal for complex IoT applications. The use of a Real-Time Operating System (RTOS), specifically FreeRTOS, is a key aspect of this project. FreeRTOS enables efficient multitasking, allowing the system to concurrently manage sensor data acquisition, actuator control, network communication, and user interactions in a responsive and reliable manner. This is crucial for a system that needs to react to environmental changes in real-time while simultaneously handling cloud communication and local display updates.

**Key Features of the System:**

*   **Comprehensive Environmental Monitoring**: The system continuously monitors critical environmental parameters including ambient temperature, humidity (via DHT20 sensor), soil moisture levels (via an analog soil moisture sensor), and ambient light intensity (via a Light Dependent Resistor - LDR). This data provides a holistic view of the garden's conditions.
*   **Automated Control Mechanisms**: Intelligent control logic is implemented for various actuators. This includes:
    *   **Smart Fan Control**: The fan is automatically activated or its speed adjusted based on predefined temperature thresholds, helping to regulate the microclimate.
    *   **Intelligent Lighting**: NeoPixel LEDs are controlled based on ambient light levels and potentially time-based schedules, ensuring optimal lighting conditions for plant growth.
*   **Remote Monitoring and Control**: Integration with the ThingsBoard.io IoT platform allows users to remotely monitor sensor data through customizable dashboards. Furthermore, users can send commands (RPC calls) from the dashboard to manually override automated controls or trigger specific actions (e.g., turn the fan on/off, change light settings).
*   **Hybrid Control Mode**: The system supports a hybrid control approach, seamlessly combining automated sensor-based control with the ability for manual override via the cloud dashboard. This provides flexibility for users to intervene when necessary.
*   **Real-time Local Display**: A 16x2 LCD character display provides immediate, on-site feedback of current sensor readings and system status, allowing for quick checks without needing to access the cloud platform.
*   **User-Friendly Web-based Setup**: For initial Wi-Fi configuration, the system implements a captive portal. When the ESP32 cannot connect to a known Wi-Fi network, it starts an Access Point (AP), allowing the user to connect to it, scan for available Wi-Fi networks, and enter credentials through a simple web interface. These credentials are then saved for future connections.

**System Objectives:**

The primary objectives of this project are:

1.  **Automation**: To significantly reduce the need for manual intervention in garden management by implementing intelligent, sensor-driven control of environmental factors.
2.  **Monitoring**: To provide robust real-time and historical data visualization capabilities, enabling users to track environmental trends and make informed decisions.
3.  **Accessibility**: To allow users to monitor and control their smart garden system remotely from any location with internet access, via the ThingsBoard dashboard.
4.  **Reliability**: To ensure consistent and robust operation of the system through effective error handling, automatic reconnection mechanisms for network services (Wi-Fi and MQTT), and stable RTOS-based task management.
5.  **Scalability and Modularity**: To design the system with a modular architecture, both in hardware and software, facilitating future expansion with additional sensors, actuators, or features.

This project demonstrates a practical application of IoT principles and RTOS capabilities to solve a real-world problem, creating a more efficient, manageable, and intelligent gardening experience. The integration of hardware, embedded software, and cloud services showcases a complete end-to-end IoT solution.

---

## 2. Background

*(This section is intentionally left blank for the user to fill in.)*

---

## 3. System Design

This section outlines the architectural design of the ESP32 Smart Garden IoT System, detailing its hardware components, software structure, communication protocols, and control logic.

### 3.1. Overall System Architecture

#### High-Level System Architecture

The system is composed of the ESP32-based embedded device and a cloud integration component, primarily ThingsBoard.io.

```
┌─────────────────────────────────────────────────────────────────┐
│                    ESP32 Smart Garden System                    │
├─────────────────────────────────────────────────────────────────┤
│  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐        │
│  │   Sensors     │  │  Controllers  │  │ Communication │        │
│  │               │  │               │  │               │        │
│  │• DHT20        │  │ • Fan (PWM)   │  │ • WiFi        │        │
│  │• Soil Moisture│  │ • NeoPixel LED│  │ • MQTT        │        │
│  │• Light (LDR)  │  │ • Status LED  │  │ • HTTP (Setup)│        │
│  │               │  │               │  │               │        │
│  └───────────────┘  └───────────────┘  └───────────────┘        │
│                                                                 │
│  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐        │
│  │  Processing   │  │   Storage     │  │    Display    │        │
│  │               │  │               │  │               │        │
│  │ • FreeRTOS    │  │ • Preferences │  │ • LCD 16x2    │        │
│  │ • Multi-task  │  │ • SPIFFS      │  │ • Serial Log  │        │
│  │ • Real-time   │  │ • WiFi Creds  │  │               │        │
│  │               │  │               │  │               │        │
│  └───────────────┘  └───────────────┘  └───────────────┘        │
└─────────────────────────────────────────────────────────────────┘
                                │ (MQTT over WiFi)
                                ▼
┌─────────────────────────────────────────────────────────────────┐
│                     Cloud Integration (ThingsBoard.io)          │
├─────────────────────────────────────────────────────────────────┤
│  ┌───────────────┐  ┌───────────────┐  ┌───────────────┐        │
│  │ ThingsBoard   │  │   Dashboard   │  │   Analytics   │        │
│  │ Platform      │  │ & Controls    │  │ & Data Store  │        │
│  │               │  │               │  │               │        │
│  │ • Device Mgmt │  │ • Real-time   │  │ • Historical  │        │
│  │ • Data Store  │  │ • RPC Commands│  │ • Trends      │        │
│  │ • RPC Server  │  │ • Alarms      │  │ • Reports     │        │
│  │               │  │               │  │               │        │
│  └───────────────┘  └───────────────┘  └───────────────┘        │
└─────────────────────────────────────────────────────────────────┘
```
**Explanation:**
*   **ESP32 Smart Garden System**: This is the core embedded device.
    *   **Sensors**: Collect environmental data (temperature, humidity, soil moisture, light).
    *   **Controllers/Actuators**: Manage physical responses (fan, NeoPixel LEDs, status LED).
    *   **Communication**: Handles network connectivity (WiFi for internet access, MQTT for cloud communication, HTTP for the captive portal).
    *   **Processing**: The ESP32 microcontroller running FreeRTOS manages all tasks, data processing, and decision-making.
    *   **Storage**: Non-volatile storage (Preferences library, SPIFFS) is used for storing WiFi credentials and potentially other configuration settings.
    *   **Display**: A local LCD provides real-time information.
*   **Cloud Integration (ThingsBoard.io)**:
    *   **ThingsBoard Platform**: Provides device management, a robust data store for telemetry, and an RPC server for command and control.
    *   **Dashboard & Controls**: Users interact with the system via web-based dashboards for visualizing data and sending control commands.
    *   **Analytics & Data Store**: ThingsBoard stores historical data, enabling trend analysis and report generation.

#### System Layer Architecture

The software on the ESP32 is structured in layers to promote modularity and separation of concerns.

```
┌─────────────────────────────────────────────────────────────────┐
│                      Application Layer                          │
│  (Manages specific system functionalities and user interaction) │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐│
│  │   Sensor    │ │   Control   │ │    MQTT     │ │    WiFi     ││
│  │  Tasks (e.g.│ │  Tasks (e.g.│ │   Manager   │ │   Manager   ││
│  │ DHT, Soil)  │ │ Fan, Light) │ │(ThingsBoard)│ │ (Connection)││
│  └─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘│
└─────────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────────┐
│                     Middleware Layer                            │
│  (Provides OS services, libraries, and communication protocols) │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐│
│  │  FreeRTOS   │ │ ArduinoJson │ │ PubSubClient│ │ ESPAsyncWeb ││
│  │   Kernel    │ │(Data Format)│ │ (MQTT Lib)  │ │Server (HTTP)││
│  └─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘│
└─────────────────────────────────────────────────────────────────┘
┌─────────────────────────────────────────────────────────────────┐
│                      Hardware Layer                             │
│  (Direct interface with ESP32 SoC and connected peripherals)    │
│  ┌─────────────┐ ┌─────────────┐ ┌─────────────┐ ┌─────────────┐│
│  │    ESP32    │ │   Sensors   │ │ Actuators   │ │  Interfaces ││
│  │  SoC & SDK  │ │ (I2C/ADC)   │ │  (PWM/GPIO) │ │ (UART/SPI)  ││
│  └─────────────┘ └─────────────┘ └─────────────┘ └─────────────┘│
└─────────────────────────────────────────────────────────────────┘
```
**Explanation:**
*   **Application Layer**: Contains the specific logic for the smart garden system. This includes tasks for reading sensors, controlling actuators, managing MQTT communication with ThingsBoard, and handling WiFi connectivity and the setup portal.
*   **Middleware Layer**: Consists of the Real-Time Operating System (FreeRTOS), libraries for data handling (ArduinoJson), communication (PubSubClient for MQTT, ESPAsyncWebServer for HTTP), and other ESP32-specific SDK components. FreeRTOS is fundamental here, providing the scheduling and synchronization primitives for the concurrent tasks in the application layer.
*   **Hardware Layer**: Represents the physical ESP32 microcontroller, its peripherals (GPIO, I2C, ADC, PWM, SPI), and the connected external hardware components like sensors and actuators. This layer is abstracted by the ESP-IDF SDK and Arduino core libraries.

### 3.2. Hardware Design

#### Microcontroller
The system is based on an ESP32 microcontroller, likely on a development board such as the "Yolo Uno" (as suggested by filenames in the workspace). The ESP32 provides the necessary processing power, memory, and peripheral interfaces.

#### Pin Configuration
The following table details the GPIO pin assignments for various components connected to the ESP32:

| Component                 | GPIO Pin(s)                  | Interface | Description                                     |
|---------------------------|------------------------------|-----------|-------------------------------------------------|
| DHT20 Temp/Humidity Sensor| GPIO 11 (SDA), GPIO 12 (SCL) | I2C       | Digital sensor for temperature and humidity     |
| LCD Display (16x2)        | GPIO 11 (SDA), GPIO 12 (SCL) | I2C       | Character display for local feedback (addr 0x33)|
| Soil Moisture Sensor      | GPIO 1                       | ADC       | Analog sensor for soil moisture content         |
| Light Sensor (LDR)        | GPIO 2                       | ADC       | Analog sensor for ambient light level           |
| NeoPixel LEDs             | GPIO 8                       | SPI (like)| Addressable RGB LED strip for status/lighting   |
| Fan Control               | GPIO 6                       | PWM       | PWM output for controlling fan speed                |
| Status LED                | GPIO 48                      | GPIO      | General system status indication LED            |

*Note: NeoPixel control is often a bit-banged protocol that behaves like SPI but is timing-critical, managed by a dedicated library.*

#### Hardware Block Diagram

```
                         ┌─────────────────┐
                         │      ESP32      │
                         │   (Yolo Uno)    │
                         └─────────┬───────┘
                                   │ (System Bus, GPIOs)
                    ┌──────────────┼──────────────┐
                    │(I2C)         │(ADC)         │(PWM/GPIO/SPI-like)
           ┌────────▼───┐  ┌───────▼────┐  ┌─────▼─────┐
           │    I2C     │  │    ADC     │  │ PWM/GPIO  │
           │   Bus      │  │  Inputs    │  │  Outputs  │
           └────┬───────┘  └─────┬──────┘  └─────┬─────┘
                │                │               │
       ┌────────┼────────┐       │               │
       │        │        │       │               │
┌──────▼──┐ ┌───▼───┐ ┌──▼───────┐ ┌──▼───────┐ ┌─▼─────────┐
│  DHT20  │ │  LCD  │ │ Soil     │ │ Light    │ │ Fan       │
│ Sensor  │ │ 16x2  │ │ Moisture │ │ Sensor   │ │ Control   │
└─────────┘ └───────┘ │ Sensor   │ │ (LDR)    │ │ (PWM)     │
                      └──────────┘ └──────────┘ └───────────┘
                                               ┌───────────┐
                                               │ NeoPixel  │
                                               │ LEDs      │
                                               └───────────┘
                                               ┌───────────┐
                                               │Status LED │
                                               │ (GPIO)    │
                                               └───────────┘
```
**Explanation:**
The ESP32 is the central hub.
*   **I2C Bus**: Shared by the DHT20 sensor and the LCD display.
*   **ADC Inputs**: Used for reading analog values from the soil moisture sensor and the LDR light sensor.
*   **PWM/GPIO Outputs**:
    *   A PWM signal controls the speed of the fan.
    *   A GPIO pin drives the NeoPixel LED strip (using a specific protocol).
    *   Another GPIO pin controls the simple status LED.

#### Power and Connectivity
*   **Power Supply**: The system is powered via a USB-C 5V input, with an onboard voltage regulator providing the 3.3V required by the ESP32 and most peripherals.
*   **WiFi**: Utilizes the ESP32's built-in 2.4GHz 802.11 b/g/n Wi-Fi module with an internal antenna for wireless network communication.
*   **Debugging/Programming**: UART communication (typically at 115200 baud) is used for programming the ESP32 and for serial logging/debugging output.
*   **Storage**: The ESP32 module includes 8MB of flash memory. This memory is partitioned for the application code, OTA updates (if implemented), and a SPIFFS (SPI Flash File System) or LittleFS filesystem for storing persistent data like WiFi credentials and configuration settings.

### 3.3. Software Architecture

The software architecture is built upon FreeRTOS, enabling a multi-tasking environment.

#### Task-Based Architecture
The system employs a preemptive multi-tasking approach using FreeRTOS. Different functionalities are encapsulated in separate tasks, each with its own priority and stack allocation. This allows for concurrent operation and modular design.

```
┌─────────────────────────────────────────────────────────────────┐
│                         Task Hierarchy                          │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌───────────────────┐     Priority 5 (Highest)                 │
│  │   WiFi Manager    │─────(Manages WiFi connection, AP mode,   │
│  │      Task         │      captive portal, credential storage) │
│  └───────────────────┘                                          │
│                                                                 │
│  ┌───────────────────┐     Priority 5                           │
│  │   MQTT Manager    │─────(Manages MQTT connection to          │
│  │      Task         │      ThingsBoard, handles RPC, publish)  │
│  └───────────────────┘                                          │
│                                                                 │
│  ┌───────────────────┐     Priority 4 (or as needed)            │
│  │ MQTT Publisher    │─────(Dedicated task for publishing data, │
│  │      Task         │      decoupled from MQTT manager)        │
│  └───────────────────┘                                          │
│                                                                 │
│  ┌───────────────────┐ ┌───────────────────┐Priority 3 (Typical)│
│  │   Sensor Tasks    │ │  Controller Tasks │                    │
│  │(Periodic Data Acq)│ │ (Actuator Control)│                    │
│  │ • Temp/Humidity   │ │ • Fan Control     │                    │
│  │ • Soil Moisture   │ │ • Light/LED Ctrl  │                    │
│  │ • Light Level     │ │ • LCD Display Task│                    │
│  └───────────────────┘ └───────────────────┘                    │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```
**Explanation of Task Priorities:**
*   **Highest Priority (e.g., 5)**: Network management tasks like `WiFi Manager` and `MQTT Manager` are often given higher priority to ensure responsive connectivity and handling of incoming commands.
*   **Mid-Level Priority (e.g., 4)**: Tasks like `MQTT Publisher` or critical control loops might run at this level.
*   **Lower Priority (e.g., 3 or below)**: Sensor reading tasks and less time-critical control tasks can run at lower priorities. The LCD update task might also be at this level.
Priorities need to be carefully assigned to prevent priority inversion and ensure that critical tasks are not starved of CPU time. Inter-task communication would typically be handled using FreeRTOS primitives like queues, semaphores, and event groups.

#### Memory Management
The ESP32 has distinct memory regions:

```
┌─────────────────────────────────────────────────────────────────┐
│                      ESP32 Memory Layout (Typical 8MB Flash)    │
├─────────────────────────────────────────────────────────────────┤
│  Flash Memory (8MB)                                             │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │ Bootloader │ Partition Table │ App (OTA0) │ App (OTA1) │ SPIFFS/LittleFS │ │
│  │  (~64KB)   │     (~4KB)      │  (~3MB)    │   (~3MB)   │ (~1.5-2MB)    │  │
│  └─────────────────────────────────────────────────────────────┘│
│     (Factory App if OTA not used fully)                         │
│                                                                 │
│  RAM Memory (~520KB total, but usable is less)                  │
│  ┌─────────────────────────────────────────────────────────────┐ │
│  │ DRAM (Data) │ IRAM (Instructions) │ RTC Slow RAM │ Peripherals│ │
│  │ (~320KB)    │ (~128KB)            │   (~16KB)    │ (DMA etc.) │ │
│  └─────────────────────────────────────────────────────────────┘ │
│  Heap: Dynamically allocated from DRAM.                         │
│  Stacks: Each FreeRTOS task has its own stack, allocated from RAM.│
└─────────────────────────────────────────────────────────────────┘
```
**Explanation:**
*   **Flash Memory**: Stores the bootloader, partition table, application firmware (potentially with OTA slots for updates), and a filesystem (SPIFFS or LittleFS) for persistent data storage (e.g., WiFi credentials, configuration settings).
*   **RAM Memory**:
    *   **DRAM**: Used for data, BSS, and the heap (dynamic memory allocation).
    *   **IRAM**: Used for executing performance-critical parts of the code.
    *   **RTC Slow RAM**: Can be used for data that needs to persist across deep sleep cycles (though deep sleep might not be a primary feature of this continuously powered system).
    *   Task stacks and FreeRTOS kernel objects also reside in RAM. Careful management of stack sizes for each task and heap usage is crucial to prevent runtime errors like stack overflows or out-of-memory conditions.

### 3.4. Communication Protocols

The system utilizes several communication protocols for different purposes:

*   **I2C (Inter-Integrated Circuit)**:
    *   **Usage**: For communication with the DHT20 temperature/humidity sensor and the 16x2 LCD display.
    *   **Characteristics**: A two-wire serial protocol (SDA and SCL) suitable for short-distance communication with multiple devices on the same bus, each having a unique address.
*   **ADC (Analog-to-Digital Converter)**:
    *   **Usage**: To read analog voltage levels from the soil moisture sensor and the Light Dependent Resistor (LDR).
    *   **Characteristics**: The ESP32's ADC converts these analog readings into digital values that the microcontroller can process.
*   **PWM (Pulse Width Modulation)**:
    *   **Usage**: To control the speed of the cooling fan.
    *   **Characteristics**: By varying the duty cycle of a digital pulse train, the average power delivered to the fan motor can be controlled, thus regulating its speed.
*   **GPIO (General Purpose Input/Output)**:
    *   **Usage**: For controlling the status LED (simple on/off) and for the bit-banged protocol used by the NeoPixel LED strip.
    *   **Characteristics**: Direct digital control of pin states.
*   **WiFi (IEEE 802.11 b/g/n)**:
    *   **Usage**: For connecting the ESP32 to the local wireless network, providing internet access.
    *   **Characteristics**: Standard wireless communication protocol operating in the 2.4GHz band.
*   **HTTP (Hypertext Transfer Protocol)**:
    *   **Usage**: An ESPAsyncWebServer library is used to create a web server on the ESP32. This server hosts a captive portal for initial WiFi setup (SSID and password entry) when the device is in Access Point mode.
    *   **Characteristics**: Standard client-server protocol for web communication.
*   **MQTT (Message Queuing Telemetry Transport)**:
    *   **Usage**: The primary protocol for IoT communication between the ESP32 and the ThingsBoard.io cloud platform. Used for publishing sensor telemetry, device attributes, and receiving RPC (Remote Procedure Call) commands.
    *   **Characteristics**: A lightweight, publish-subscribe network protocol ideal for constrained devices and low-bandwidth, high-latency, or unreliable networks.
*   **UART (Universal Asynchronous Receiver/Transmitter)**:
    *   **Usage**: For serial communication with a host computer, primarily for firmware programming/flashing and for outputting debug messages and logs.
    *   **Characteristics**: Standard asynchronous serial communication.

### 3.5. Control Logic

The system implements hybrid control logic, combining automated responses based on sensor readings with the capability for manual overrides from the ThingsBoard dashboard.

#### Automated Fan Control
*   **Trigger**: Ambient temperature readings from the DHT20 sensor.
*   **Logic**:
    *   A predefined temperature threshold (e.g., 30°C) is set.
    *   If the measured temperature exceeds this threshold, the fan is automatically turned ON. The speed might be a default value or could be proportional to how much the temperature exceeds the threshold (more advanced).
    *   If the temperature falls below the threshold (or a hysteresis lower bound), the fan is turned OFF.
*   **Diagrammatic Representation**:
    ```
    Temperature Reading (from DHT20)
             │
             ▼
    ┌─────────────────┐
    │ Temperature     │
    │ Threshold Check │
    └─────────────────┘
             │
    (Temp > Threshold_High?) ──YES──►  ┌─────────────────┐
             │                         │ Auto: Fan ON    │
             │                         │ (Set PWM Speed) │
             NO                        └─────────────────┘
             │                                   │
    (Temp < Threshold_Low?)                      │
             │                                   │
             ▼                                   ▼
    ┌─────────────────┐                 (Fan remains ON or adjusts)
    │ Auto: Fan OFF   │
    │ (Set PWM to 0)  │
    └─────────────────┘
    ```

#### Intelligent Lighting Control (NeoPixel LEDs)
*   **Trigger**: Ambient light level readings from the LDR sensor.
*   **Logic**:
    *   The system aims to maintain an optimal light level for plants.
    *   If the ambient light detected by the LDR falls below a certain threshold (indicating it's too dark), the NeoPixel LEDs are turned ON to supplement the light. The color and intensity might be configurable.
    *   If the ambient light is sufficient, the NeoPixel LEDs are turned OFF or dimmed to save energy.
*   **Additional Logic**: Could also incorporate time-of-day schedules, e.g., ensuring lights are off during a "night" period for the plants, regardless of LDR readings.

#### Hybrid Control Mode
*   **Mechanism**: Users can send RPC commands from the ThingsBoard dashboard to manually control devices (e.g., `setFanState: true/false`, `setFanSpeed: 50%`, `setLightState: on/off`).
*   **Behavior**:
    *   When a manual command is received, it typically overrides the current automated state.
    *   The system needs to track whether it's in "manual" mode for a particular device or if it should revert to "auto" mode after a certain time or a specific command.
    *   The state (manual/auto, current settings) should be synchronized back to ThingsBoard so the dashboard reflects the actual operational state.

### 3.6. Data Flow

The data flow describes how information moves through the system:

1.  **Sensor Data Acquisition**:
    *   Respective FreeRTOS tasks periodically read data from the DHT20 (temperature, humidity), soil moisture sensor (analog value), and LDR (analog value).
    *   Raw sensor values are processed (e.g., ADC values converted to percentages or meaningful units).

2.  **Local Processing and Control**:
    *   The processed sensor data is used by control tasks (e.g., fan control, light control) to make decisions based on the defined control logic.
    *   Actuators (fan, NeoPixel LEDs) are adjusted accordingly via PWM or GPIO signals.

3.  **Local Display**:
    *   Key sensor readings and system status information are formatted and sent to the 16x2 LCD display for immediate local feedback.

4.  **Data Packaging for Cloud**:
    *   Sensor data and device status (e.g., fan on/off, light on/off, current mode auto/manual) are packaged, typically into a JSON format (using ArduinoJson library).

5.  **MQTT Telemetry Publishing**:
    *   The `MQTT Manager` task publishes this JSON payload as telemetry to specific topics on the ThingsBoard.io MQTT broker.
    *   Device attributes (e.g., firmware version, IP address, configurable thresholds) might also be published to ThingsBoard.

6.  **Cloud Platform (ThingsBoard.io)**:
    *   ThingsBoard receives the MQTT messages.
    *   Telemetry data is stored in its database.
    *   Dashboards are updated in real-time to visualize the incoming data.
    *   Rule chains in ThingsBoard can process incoming data for alerts, notifications, or further actions.

7.  **Remote Command (RPC) Flow**:
    *   A user interacts with a control widget (e.g., a switch or slider) on the ThingsBoard dashboard.
    *   This action triggers an RPC request from ThingsBoard to the ESP32 device via MQTT (sent to a specific RPC subscription topic).
    *   The `MQTT Manager` task on the ESP32 receives the RPC request.
    *   The request is parsed (e.g., method name like `setFanSpeed` and parameters like `{"speed": 75}`).
    *   The corresponding function or action is invoked on the ESP32 (e.g., adjusting fan PWM).
    *   The ESP32 can send an RPC reply back to ThingsBoard to acknowledge the command or return a result.

8.  **Configuration Data (WiFi Credentials)**:
    *   During initial setup (Captive Portal):
        *   User connects to ESP32's AP.
        *   User submits WiFi SSID/password via a web form.
        *   The `WiFi Manager` task receives these credentials.
        *   Credentials are saved to persistent storage (Preferences/SPIFFS).
    *   On subsequent boots:
        *   `WiFi Manager` task reads saved credentials from storage.
        *   Attempts to connect to the configured WiFi network.

This comprehensive data flow ensures that the system can operate autonomously, provide local feedback, communicate effectively with the cloud, and respond to remote commands.

---

## 4. System Implementation

This section details the implementation of the software modules and tasks running on the ESP32, based on the FreeRTOS multi-tasking architecture. The source files are typically located in the `src/` and `include/` directories of the PlatformIO project structure.

### 4.1. WiFi Manager Task

*   **Header File**: `include/tasks/wifi_manager.h`
*   **Source File**: `src/tasks/wifi_manager.cpp`
*   **Priority**: High (e.g., 5), as network connectivity is crucial for other tasks like MQTT.
*   **Stack Size**: Typically 4096 bytes or more, due to the use of WiFi libraries and potentially the web server for the captive portal.

**Functionality**:
*   **Connection Management**: Handles connecting the ESP32 to a configured Wi-Fi network using saved credentials (SSID and password) retrieved from persistent storage (Preferences API).
*   **Automatic Reconnection**: Implements a mechanism to automatically attempt reconnection if the Wi-Fi connection is lost. This usually involves a retry strategy with delays.
*   **Captive Portal for Initial Setup**:
    *   If no valid Wi-Fi credentials are found or if connection fails repeatedly, the task switches the ESP32 to Access Point (AP) mode.
    *   It starts an `AsyncWebServer` to serve a simple web page.
    *   When a user connects to this AP (e.g., "ESP32-SmartGarden-Setup") and opens a browser, they are redirected to this page (captive portal).
    *   The page allows the user to scan for available Wi-Fi networks, select one, and enter its password.
*   **Credential Saving**: Upon submission, the received SSID and password are saved to the ESP32's persistent storage. The device then attempts to connect to the newly configured network.
*   **Status Indication**: Provides status updates (e.g., connecting, connected, IP address, AP mode active) via serial logs and potentially to other tasks for display on the LCD or via LEDs.

**State Machine (Simplified)**:
```
     ┌─────────────┐         ┌────────────────┐
     │   STARTUP   │─NoCreds►│ START_AP_MODE  ├─┐
     └──────┬──────┘         └────────┬───────┘ │
            │ Saved Creds             │         │ User submits
            ▼                         │         │ creds via web
     ┌─────────────┐         ┌────────▼───────┐ │
     │ CONNECTING  │◄─Failed─┤ HANDLE_WEB_REQ │◄┘
     │ (STA Mode)  │         │(Captive Portal)│
     └──────┬──────┘         └────────────────┘
            │ Success
            ▼
     ┌─────────────┐
     │  CONNECTED  │───Disconnect───► (Back to CONNECTING or START_AP_MODE)
     └─────────────┘
```

**Key Functions (Conceptual)**:
*   `void task_wifi_manager(void *pvParameters)`: The FreeRTOS task function containing the main loop and state logic.
*   `bool connectToSavedWiFi()`: Attempts to connect using credentials from Preferences.
*   `void startAccessPointMode()`: Configures and starts the ESP32 in AP mode and initializes the web server for the captive portal.
*   `void handlePortalClient()`: Handles incoming HTTP requests for the captive portal (e.g., serving HTML, processing form submissions).
*   `void saveWiFiCredentials(const char* ssid, const char* password)`: Saves credentials to Preferences.
*   `bool isWiFiConnected()`: Returns the current WiFi connection status.

### 4.2. MQTT Manager Task

*   **Header File**: `include/tasks/mqtt_manager.h`
*   **Source File**: `src/tasks/mqtt_manager.cpp`
*   **Priority**: High (e.g., 5 or 4), to ensure timely communication with the ThingsBoard platform.
*   **Stack Size**: Typically 4096 bytes or more, due to MQTT library usage and JSON processing.

**Functionality**:
*   **Connection Management**: Establishes and maintains a connection to the ThingsBoard.io MQTT broker. This requires the broker address, port, device access token (or other credentials).
*   **Automatic Reconnection**: If the MQTT connection drops, this task attempts to reconnect periodically.
*   **Telemetry Publishing**:
    *   Receives formatted data (typically JSON strings) from sensor tasks or a central data management module.
    *   Publishes this telemetry data to the appropriate ThingsBoard topic (e.g., `v1/devices/me/telemetry`).
    *   A dedicated `MQTT Publisher Task` might exist to decouple data preparation and queueing from the actual publishing act, which this `MQTT Manager` would then handle.
*   **Attribute Publishing**: Publishes device attributes (e.g., firmware version, IP address, current settings like temperature thresholds) to ThingsBoard (e.g., `v1/devices/me/attributes`).
*   **RPC (Remote Procedure Call) Command Handling**:
    *   Subscribes to RPC request topics from ThingsBoard (e.g., `v1/devices/me/rpc/request/+`).
    *   When an RPC message is received, it parses the JSON payload to identify the method name (e.g., `setFanState`) and parameters.
    *   It then dispatches the command to the relevant control task or function within the system.
    *   It may send an RPC reply back to ThingsBoard (to `v1/devices/me/rpc/response/<request_id>`) indicating success or failure.
*   **Shared Attribute Updates**: Subscribes to attribute update topics from ThingsBoard (e.g., `v1/devices/me/attributes`) to receive changes to shared attributes (e.g., new fan speed setpoint from dashboard) and apply them locally.

**RPC Command Handling Flow (Conceptual)**:
```
MQTT Message Received on RPC Topic (e.g., v1/devices/me/rpc/request/123)
         │
         ▼ (Parse JSON: {"method": "setFanSpeed", "params": {"speed": 50}})
┌──────────────────────────────────────┐
│          RPC Method Routing          │
├──────────────────────────────────────┤
│ If method == "setFanState":          │
│   Call fan_control_set_state(params) │
│ Else if method == "setFanSpeed":     │
│   Call fan_control_set_speed(params) │
│ Else if method == "setLightState":   │
│  Call light_control_set_state(params)│
│ ... etc.                             │
└──────────────────────────────────────┘
         │
         ▼ (Action performed by respective module)
┌──────────────────────────────────────┐
│   Optionally Send RPC Response       │
│(e.g., v1/devices/me/rpc/response/123)│
└──────────────────────────────────────┘
```

**Key Functions (Conceptual)**:
*   `void task_mqtt_manager(void *pvParameters)`: The FreeRTOS task function.
*   `void mqtt_connect()`: Connects/reconnects to the MQTT broker.
*   `void mqtt_callback(char* topic, byte* payload, unsigned int length)`: Callback function invoked by the MQTT library when a message is received. Handles RPC and attribute updates.
*   `void publishTelemetry(const char* jsonPayload)`: Publishes telemetry data.
*   `void publishAttributes(const char* jsonPayload)`: Publishes client-side attributes.
*   `void handleRpcRequest(const char* methodName, const JsonDocument& params)`: Processes a parsed RPC request.

### 4.3. Sensor Tasks

These tasks are responsible for periodically reading data from the various sensors. Each sensor or group of closely related sensors might have its own task for modularity and independent scheduling.

#### Temperature & Humidity Task (DHT20)
*   **Header File**: `include/tasks/temperature_humidity.h`
*   **Source File**: `src/tasks/temperature_humidity.cpp`
*   **Sensor**: DHT20 (I2C interface)
*   **Update Interval**: Configurable, e.g., every 10-30 seconds.
*   **Functionality**:
    *   Initializes the DHT20 sensor.
    *   Periodically reads temperature and humidity values.
    *   May perform basic validation or filtering of readings.
    *   Makes the data available to other tasks (e.g., via global variables with mutex protection, queues, or by sending to a central data aggregator task).
*   **Task Structure Snippet (Conceptual)**:
    ```cpp
    // In temperature_humidity.cpp
    #include "DHT20.h"
    // ... other includes, global variables for temp/humidity

    DHT20 dht20; // Assuming I2C pins are predefined or configured in main

    void task_temperature_humidity(void *pvParameters) {
        if (dht20.begin()) {
            // Sensor initialized
        } else {
            // Handle sensor init error
        }
        for (;;) {
            if (dht20.read()) {
                float temperature = dht20.getTemperature();
                float humidity = dht20.getHumidity();
                // Store/send data (e.g., update global vars with mutex, send to queue)
                // Log data
            } else {
                // Handle read error
            }
            vTaskDelay(pdMS_TO_TICKS(10000)); // Delay for 10 seconds
        }
    }
    ```

#### Soil Moisture Task
*   **Header File**: `include/tasks/soil_moisture.h`
*   **Source File**: `src/tasks/soil_moisture.cpp`
*   **Sensor Interface**: ADC (e.g., GPIO 1)
*   **Update Interval**: Configurable, e.g., every 10-60 seconds.
*   **Functionality**:
    *   Configures the ADC pin.
    *   Periodically reads the analog value from the soil moisture sensor.
    *   Converts the raw ADC value to a meaningful unit (e.g., percentage moisture). This might involve calibration (mapping raw values for "dry" and "wet" conditions).
    *   Makes the data available.

#### Light Sensor Task (LDR)
*   **Header File**: `include/tasks/light_and_led.h` (likely integrated with LED control)
*   **Source File**: `src/tasks/light_and_led.cpp`
*   **Sensor Interface**: ADC (e.g., GPIO 2)
*   **Update Interval**: Configurable, e.g., every 5-10 seconds, often tied to the light control logic.
*   **Functionality**:
    *   Configures the ADC pin for the LDR.
    *   Periodically reads the analog value representing light intensity.
    *   Converts the raw ADC value to a relevant scale (e.g., lux, percentage).
    *   This data is primarily used by the `Light Control Task`.

### 4.4. Control Tasks

These tasks implement the logic for controlling actuators based on sensor data and/or remote commands.

#### Fan Control Task
*   **Header File**: `include/tasks/fan_control.h`
*   **Source File**: `src/tasks/fan_control.cpp`
*   **Priority**: Medium (e.g., 4 or 3).
*   **Control Method**: PWM output (e.g., on GPIO 6) using ESP32's LEDC (LED Control) peripheral for PWM generation.
*   **Functionality**:
    *   Initializes the PWM channel for fan control.
    *   Receives temperature data (from the temperature sensor task or a shared data structure).
    *   Receives fan control commands (on/off, speed percentage) from the `MQTT Manager` (RPC) or a local UI.
    *   Implements the hybrid control logic:
        *   **Auto Mode**: Adjusts fan speed (PWM duty cycle) based on temperature thresholds.
        *   **Manual Mode**: Sets fan speed based on user commands, overriding auto mode.
    *   Manages state (current speed, auto/manual mode) and publishes this state back to ThingsBoard via the `MQTT Manager`.
*   **State Synchronization (Conceptual)**:
    ```cpp
    // In fan_control.cpp
    // Global or class members:
    // bool fan_manual_mode = false;
    // int fan_current_speed_percent = 0; // 0-100
    // float fan_auto_threshold_temp = 30.0;

    void setFanState(bool enabled, int speedPercent, const char* source) {
        if (strcmp(source, "DASHBOARD") == 0 || strcmp(source, "RPC") == 0) {
            fan_manual_mode = true; // Manual override
        }
        fan_current_speed_percent = enabled ? speedPercent : 0;
        // Apply PWM duty cycle based on fan_current_speed_percent
        // publishFanState(enabled, fan_current_speed_percent, fan_manual_mode ? "MANUAL" : "AUTO");
    }

    void task_fan_control(void *pvParameters) {
        // ... init PWM ...
        for (;;) {
            if (!fan_manual_mode) {
                // float current_temp = get_latest_temperature();
                // if (current_temp > fan_auto_threshold_temp) {
                //     setFanState(true, DEFAULT_AUTO_SPEED, "AUTO");
                // } else {
                //     setFanState(false, 0, "AUTO");
                // }
            }
            // Wait for next cycle or event (e.g., new command, new temp reading)
            vTaskDelay(pdMS_TO_TICKS(2000)); // Example delay
        }
    }
    ```

#### Light Control Task (NeoPixel LEDs)
*   **Header File**: `include/tasks/light_and_led.h` or `include/tasks/led_control.h`
*   **Source File**: `src/tasks/light_and_led.cpp` or `src/tasks/led_control.cpp`
*   **LEDs**: NeoPixel RGB LEDs (e.g., a strip of 4 LEDs on GPIO 8). Uses Adafruit_NeoPixel library.
*   **Functionality**:
    *   Initializes the NeoPixel library.
    *   Receives light level data from the LDR sensor task.
    *   Receives light control commands (on/off, color, brightness) from `MQTT Manager` or local UI.
    *   Implements hybrid control:
        *   **Auto Mode**: Turns LEDs on/off or adjusts brightness/color based on ambient light levels (e.g., turn on if LDR reading is below a threshold).
        *   **Manual Mode**: Sets LED state based on user commands.
    *   Manages LED state and publishes it to ThingsBoard.
    *   May also control the simple status LED (GPIO 48) to indicate system states (e.g., WiFi connecting, MQTT connected, error).

### 4.5. Local Display and Configuration

#### LCD Display Task
*   **Interface**: I2C (shared with DHT20, LCD address typically 0x27 or 0x3F, document says 0x33).
*   **Library**: A standard LiquidCrystal_I2C library.
*   **Functionality**:
    *   A dedicated task or functions within other tasks periodically update the 16x2 LCD.
    *   Displays key information such as:
        *   Current Temperature and Humidity.
        *   Soil Moisture level.
        *   Ambient Light level.
        *   WiFi connection status (IP address or "Connecting...").
        *   MQTT connection status.
        *   Current fan/light status.
    *   The display content might cycle through different screens if there's too much information for two lines.

#### Configuration Storage
*   **Mechanism**: ESP32 Preferences library (key-value store in NVS - Non-Volatile Storage) or SPIFFS/LittleFS for more complex configuration files.
*   **Usage**:
    *   Storing WiFi SSID and password.
    *   Potentially storing user-settable thresholds (e.g., temperature for fan, light level for LEDs) if these are configurable via the captive portal or ThingsBoard shared attributes.
    *   The `WiFi Manager` handles reading/writing WiFi credentials. Other modules would access their specific configurations.

This implementation structure, leveraging FreeRTOS tasks and various libraries, allows for a modular, responsive, and feature-rich smart garden system. Careful management of shared resources (like I2C bus, global variables) using mutexes or other synchronization primitives is essential for stability.

---

## 5. Conclusion

The "Smart Device Control System using RTOS on an IoTS Platform," exemplified by the ESP32 Smart Garden project, successfully demonstrates the integration of embedded systems, real-time operating systems, sensor networks, and cloud-based IoT platforms to create an intelligent and automated environmental management solution.

### 5.1. Achievements

*   **Robust RTOS-based Multitasking**: The utilization of FreeRTOS allowed for the development of a responsive and stable system capable of concurrently handling sensor data acquisition, complex control logic, network communications (Wi-Fi and MQTT), and local display updates without significant performance degradation. This modular task-based architecture is a key achievement.
*   **Comprehensive Environmental Sensing and Control**: The system effectively integrates multiple sensors (DHT20 for temperature/humidity, soil moisture sensor, LDR for light) and actuators (PWM fan, NeoPixel LEDs), providing a holistic approach to garden environment monitoring and automated regulation.
*   **Seamless Cloud Integration with ThingsBoard.io**: Successful integration with the ThingsBoard IoT platform enables powerful remote monitoring via customizable dashboards, real-time data visualization, historical data logging for trend analysis, and remote command and control capabilities through RPC calls.
*   **Hybrid Control System**: The implementation of a hybrid control mode, which combines automated sensor-driven responses with the ability for users to manually override settings via the cloud dashboard, offers both intelligence and flexibility in system operation.
*   **User-Friendly Configuration**: The captive portal for Wi-Fi setup significantly enhances the user experience, simplifying the initial configuration process without requiring hardcoded credentials or serial terminal access for basic setup.
*   **Real-time Local Feedback**: The inclusion of an LCD display provides immediate, on-site access to critical sensor readings and system status, which is invaluable for quick checks and local diagnostics.
*   **Modular and Scalable Design**: The software architecture, with its distinct layers and task separation, along with the hardware design, provides a foundation that is relatively easy to extend with new sensors, actuators, or functionalities in the future.
*   **Practical Application of IoT Concepts**: The project serves as an excellent practical demonstration of an end-to-end IoT solution, from sensor data collection at the edge to cloud processing and user interaction.

### 5.2. Limitations

While the system achieves its core objectives, certain limitations are inherent or could be areas for improvement:

*   **Sensor Accuracy and Reliability**: The accuracy of low-cost sensors like DHT20, analog soil moisture sensors (which can corrode), and LDRs can be limited and may drift over time. They may also be susceptible to environmental noise.
*   **Network Dependency**: The system heavily relies on a stable Wi-Fi connection for cloud communication and remote control. In case of Wi-Fi or internet outages, remote functionalities are lost, and historical data logging to the cloud is interrupted (unless local buffering is implemented).
*   **Security Concerns**: While functional, the security aspects (Wi-Fi WPA2, MQTT with device token) are standard. For more critical applications, enhanced security measures like MQTT over TLS, secure element for key storage, and network intrusion detection would be necessary. The captive portal itself could be a potential, albeit small, security risk if not carefully implemented.
*   **Power Consumption**: The current design assumes continuous USB power. For battery-powered operation, significant power optimization would be required, including the use of deep sleep modes and low-power components, which would change the real-time responsiveness characteristics.
*   **Scalability of Single ESP32**: While the ESP32 is powerful, a single unit might become a bottleneck if the number of sensors/actuators or the complexity of control algorithms increases drastically.
*   **ThingsBoard Tier Limitations**: If using a free or limited tier of ThingsBoard.io, there might be restrictions on data rate, storage volume, or the number of devices/RPC calls, which could be a limitation for larger deployments or long-term use.
*   **Physical Enclosure and Durability**: The design focuses on the electronics and software. For actual deployment in a garden, a weatherproof and durable enclosure for the ESP32, sensors, and wiring would be essential, which is not covered in this scope.
*   **Error Handling and Fault Tolerance**: While basic error handling (e.g., reconnection attempts) is implemented, a more robust system would include comprehensive fault detection, diagnostics, and recovery mechanisms for various failure scenarios (e.g., sensor failure, actuator malfunction).

### 5.3. Future Work

The current system provides a solid foundation for numerous potential enhancements and future developments:

*   **Advanced Sensing and Actuation**:
    *   Integrate more diverse sensors: pH sensors, CO2 sensors, water level sensors, EC (electrical conductivity) sensors for nutrient monitoring.
    *   Add more sophisticated actuators: Automated watering systems (solenoid valves, pumps), nutrient dispensers, automated window/vent openers.
*   **Smarter Control Algorithms**:
    *   Implement PID (Proportional-Integral-Derivative) controllers for more precise environmental control (e.g., maintaining temperature within a very narrow range).
    *   Explore machine learning (e.g., using TensorFlow Lite for Microcontrollers on the ESP32 or cloud-based ML) for predictive analytics, anomaly detection, or optimizing growing conditions based on historical data.
*   **Enhanced User Interface and Experience**:
    *   Develop a dedicated mobile application (iOS/Android) for a more polished and accessible remote monitoring and control experience.
    *   Improve the local LCD interface with more screens, graphical elements (if using a graphical LCD), or button inputs for local configuration.
*   **Power Optimization and Alternative Power Sources**:
    *   Implement deep sleep modes and other power-saving techniques for potential battery-powered operation.
    *   Explore solar power integration with battery backup for off-grid deployments.
*   **Increased Robustness and Reliability**:
    *   Implement Over-The-Air (OTA) firmware updates for easier maintenance and feature deployment without physical access.
    *   Add local data logging to an SD card as a buffer in case of network outages, with synchronization to the cloud when connectivity is restored.
    *   Develop more comprehensive self-diagnostic routines and alert mechanisms for system faults.
*   **Security Hardening**:
    *   Implement MQTT over TLS/SSL for encrypted communication with ThingsBoard.
    *   Utilize the ESP32's hardware security features (e.g., Flash Encryption, Secure Boot).
    *   Implement role-based access control on the ThingsBoard dashboard if multiple users are involved.
*   **Expanded Data Analytics and Reporting**:
    *   Leverage ThingsBoard's advanced rule engine and analytics features more extensively.
    *   Integrate with external data analysis platforms or services for more in-depth insights and custom report generation.
*   **Edge Computing Capabilities**:
    *   Perform more data processing, filtering, and decision-making locally on the ESP32 (edge computing) to reduce latency, minimize data transmission to the cloud, and enable continued autonomous operation during network disruptions.
*   **Modular Hardware Expansion**:
    *   Design a standardized bus or connector system for easily adding or swapping sensor/actuator modules (plug-and-play concept).

By addressing these limitations and exploring future enhancements, the Smart Device Control System can evolve into an even more powerful, versatile, and user-friendly solution for a wide range of environmental monitoring and automation applications.
