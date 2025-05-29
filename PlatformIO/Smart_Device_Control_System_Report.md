# Smart Device Control System using RTOS on IoTS Platform

## Table of Contents
1. [Introduction](#1-introduction)
    1.1. [Motivation](#11-motivation)
    1.2. [Project Objectives](#12-project-objectives)
    1.3. [Project Structure](#13-project-structure)
2. [Background Knowledge](#2-background-knowledge)
3. [System Design](#3-system-design)
    3.1. [System Requirements](#31-system-requirements)
        3.1.1. [Functional Requirements](#311-functional-requirements)
        3.1.2. [Non-functional Requirements](#312-non-functional-requirements)
    3.2. [System Architecture](#32-system-architecture)
        3.2.1. [High-Level System Architecture](#321-high-level-system-architecture)
        3.2.2. [System Layer Architecture](#322-system-layer-architecture)
    3.3. [Hardware Design](#33-hardware-design)
        3.3.1. [Microcontroller](#331-microcontroller)
        3.3.2. [Pin Configuration](#332-pin-configuration)
        3.3.3. [Hardware Block Diagram](#333-hardware-block-diagram)
        3.3.4. [Power and Connectivity](#334-power-and-connectivity)
    3.4. [Software Architecture](#34-software-architecture)
        3.4.1. [Task-Based Architecture](#341-task-based-architecture)
        3.4.2. [Memory Management](#342-memory-management)
    3.5. [Communication Protocols](#35-communication-protocols)
    3.6. [Control Logic and Data Flow](#36-control-logic-and-data-flow)
        3.6.1. [Control Logic](#361-control-logic)
        3.6.2. [Data Flow](#362-data-flow)
4. [System Implementation](#4-system-implementation)
    4.1. [WiFi Manager Task](#wifi-manager-task)
    4.2. [MQTT Manager Task](#mqtt-manager-task)
    4.3. [Sensor Tasks](#sensor-tasks)
    4.4. [Control Tasks](#control-tasks)
    4.5. [Local Display and Configuration](#local-display-and-configuration)
5. [Conclusion](#5-conclusion)
    5.1. [Achievements](#51-achievements)
    5.2. [Limitations](#52-limitations)
    5.3. [Future Work](#53-future-work)

---

## 1. Introduction

### 1.1 Motivation
*(This section is to be filled by the user, outlining the motivation behind the project.)*

The "Smart Device Control System using RTOS on an IoT Platform" is an advanced project focused on creating an intelligent, automated environment management system. This particular implementation, an ESP32-based Smart Garden IoT System, serves as a comprehensive solution for monitoring and controlling a smart garden environment. The system leverages the power of the ESP32 microcontroller, a variety of environmental sensors, and automated control mechanisms, all integrated with a cloud-based IoT platform (ThingsBoard.io) for remote monitoring and management.

The core of the system is built around the ESP32, a versatile System on Chip (SoC) known for its dual-core processing capabilities, integrated Wi-Fi and Bluetooth, and ample GPIOs, making it ideal for complex IoT applications. The use of a Real-Time Operating System (RTOS), specifically FreeRTOS, is a key aspect of this project. FreeRTOS enables efficient multitasking, allowing the system to concurrently manage sensor data acquisition, actuator control, network communication, and user interactions in a responsive and reliable manner. This is crucial for a system that needs to react to environmental changes in real-time while simultaneously handling cloud communication and local display updates.

### 1.2 Project Objectives

The primary objectives of this project, derived from the ESP32 Smart Garden IoT System implementation, are:

1.  **Automation**: To significantly reduce the need for manual intervention in garden management by implementing intelligent, sensor-driven control of environmental factors.
2.  **Monitoring**: To provide robust real-time and historical data visualization capabilities, enabling users to track environmental trends and make informed decisions.
3.  **Accessibility**: To allow users to monitor and control their smart garden system remotely from any location with internet access, via the ThingsBoard dashboard.
4.  **Reliability**: To ensure consistent and robust operation of the system through effective error handling, automatic reconnection mechanisms for network services (Wi-Fi and MQTT), and stable RTOS-based task management.
5.  **Scalability and Modularity**: To design the system with a modular architecture, both in hardware and software, facilitating future expansion with additional sensors, actuators, or features.

**Key Features of the System (as implemented in the Smart Garden context):**

*   **Comprehensive Environmental Monitoring**: The system continuously monitors critical environmental parameters including ambient temperature, humidity (via DHT20 sensor), soil moisture levels (via an analog soil moisture sensor), and ambient light intensity (via a Light Dependent Resistor - LDR).
*   **Automated Control Mechanisms**: Intelligent control logic is implemented for various actuators, including smart fan control based on temperature and intelligent lighting with NeoPixel LEDs based on sensor feedback.
*   **Remote Monitoring and Control**: Integration with the ThingsBoard.io IoT platform allows users to remotely monitor sensor data and send commands (RPC calls) to manually override automated controls.
*   **Hybrid Control Mode**: Supports a combination of automatic sensor-based control and manual override capabilities.
*   **Real-time Local Display**: A 16x2 LCD character display provides immediate, on-site feedback of current sensor readings and system status.
*   **User-Friendly Web-based Setup**: Implements a captive portal for initial Wi-Fi configuration.

This project demonstrates a practical application of IoT principles and RTOS capabilities to solve a real-world problem, creating a more efficient, manageable, and intelligent gardening experience. The integration of hardware, embedded software, and cloud services showcases a complete end-to-end IoT solution.

### 1.3 Project Structure

This report is structured as follows:

*   **Chapter 1: Introduction** outlines the project's motivation, objectives, and the structure of this document.
*   **Chapter 2: Background Knowledge** provides theoretical background relevant to the project (to be completed by the user).
*   **Chapter 3: System Design** details the system requirements, overall architecture, hardware and software design, communication protocols, and control logic with data flow.
*   **Chapter 4: System Implementation** describes the practical realization of the designed system, including task implementations and specific technical details.
*   **Chapter 5: Conclusion** summarizes the project's achievements, discusses its limitations, and suggests potential areas for future work.

---

## 2. Background Knowledge

*(This section is intentionally left blank for the user to fill in.)*

---

## 3. System Design

This section outlines the architectural design of the ESP32 Smart Garden IoT System, detailing its system requirements, hardware components, software structure, communication protocols, control logic, and data flow.

### 3.1 System Requirements

#### 3.1.1 Functional Requirements
Based on the implemented ESP32 Smart Garden IoT System, the functional requirements are:

*   **Environmental Monitoring**: The system shall monitor ambient temperature, humidity, soil moisture content, and light levels.
*   **Automated Actuator Control**:
    *   The system shall provide smart fan control based on temperature readings.
    *   The system shall implement intelligent lighting control using NeoPixel LEDs based on sensor feedback (e.g., light levels).
*   **Remote Access and Control**:
    *   The system shall allow remote monitoring of sensor data via a ThingsBoard.io dashboard.
    *   The system shall support remote control of actuators through MQTT/RPC commands from the ThingsBoard dashboard.
*   **Hybrid Control**: The system shall support a hybrid control mode, allowing a combination of automatic sensor-based control and manual override capabilities.
*   **Local Data Display**: The system shall provide a local LCD display for immediate viewing of sensor readings and system status.
*   **Network Configuration**: The system shall offer a web-based setup mechanism (captive portal) for configuring WiFi credentials.

#### 3.1.2 Non-functional Requirements
Derived from the system objectives and performance specifications of the ESP32 Smart Garden IoT System:

*   **Reliability**:
    *   The system shall ensure robust operation with effective error handling.
    *   The system shall implement automatic reconnection mechanisms for WiFi and MQTT services.
    *   Target uptime: High, aiming for 99%+ with minimal downtime.
*   **Performance**:
    *   Boot Time: Shall be less than 10 seconds (actual observed ~8 seconds).
    *   WiFi Connection Time: Shall be established in less than 30 seconds (actual observed ~15 seconds).
    *   MQTT Connection Time: Shall be established in less than 10 seconds (actual observed ~5 seconds).
    *   Sensor Update Rate: Data from sensors shall be updated at regular intervals (e.g., every 10 seconds for primary sensors).
    *   Command Response Time: Remote commands shall be processed with a response time of less than 2 seconds (actual observed ~1 second).
*   **Resource Usage**:
    *   RAM Usage: Shall be optimized, targeting usage below 200KB (actual observed ~180KB).
    *   Flash Usage: Application firmware and data shall occupy less than 2MB of flash storage (actual observed ~1.5MB).
*   **Scalability**: The system shall be designed with a modular architecture to facilitate easy expansion with new sensors, actuators, or features.
*   **Usability**:
    *   The WiFi setup process via captive portal should be user-friendly.
    *   The ThingsBoard dashboard should provide an intuitive interface for monitoring and control.
*   **Security**:
    *   WiFi credentials shall be stored securely using the Preferences library.
    *   Communication with the MQTT broker shall use device access token authentication.
*   **Maintainability**: The software should be well-structured (e.g., task-based) and documented to allow for easier debugging and updates.

### 3.2 System Architecture

#### 3.2.1 High-Level System Architecture

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

#### 3.2.2 System Layer Architecture

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

### 3.3 Hardware Design

#### 3.3.1 Microcontroller
The system is based on an ESP32 microcontroller, likely on a development board such as the "Yolo Uno" (as suggested by filenames in the workspace). The ESP32 provides the necessary processing power, memory, and peripheral interfaces.

#### 3.3.2 Pin Configuration
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

#### 3.3.3 Hardware Block Diagram

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
           └────┬───────┘  └──────────┬─┘  └──────┬────┘
                │                     │           │
       ┌────────┼────────┐            │           │
       │        │        │            │           │
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

#### 3.3.4 Power and Connectivity
*   **Power Supply**: The system is powered via a USB-C 5V input, with an onboard voltage regulator providing the 3.3V required by the ESP32 and most peripherals.
*   **WiFi**: Utilizes the ESP32's built-in 2.4GHz 802.11 b/g/n Wi-Fi module with an internal antenna for wireless network communication.
*   **Debugging/Programming**: UART communication (typically at 115200 baud) is used for programming the ESP32 and for serial logging/debugging output.
*   **Storage**: The ESP32 module includes 8MB of flash memory. This memory is partitioned for the application code, OTA updates (if implemented), and a SPIFFS (SPI Flash File System) or LittleFS filesystem for storing persistent data like WiFi credentials and configuration settings.

### 3.4 Software Architecture

#### 3.4.1 Task-Based Architecture
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

#### 3.4.2 Memory Management
The ESP32 has distinct memory regions:

```
┌─────────────────────────────────────────────────────────────────┐
│                      ESP32 Memory Layout                        │
├─────────────────────────────────────────────────────────────────┤
│  Flash Memory (8MB)                                             │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │ Bootloader │ Partition Table │ App (OTA0) │ App (OTA1) │ SPIFFS/LittleFS │ │
│  │  (~64KB)   │     (~4KB)      │  (~3MB)    │   (~3MB)   │ (~1.5-2MB)    │  │
│  └─────────────────────────────────────────────────────────────┘│
│     (Factory App if OTA not used fully)                         │
│                                                                 │
│  RAM Memory (~520KB total, but usable is less)                  │
│  ┌─────────────────────────────────────────────────────────────┐│
│  │ DRAM (Data) │ IRAM (Instructions) │ RTC Slow RAM │ Peripherals│ │
│  │ (~320KB)    │ (~128KB)            │   (~16KB)    │ (DMA etc.)│ │
│  └─────────────────────────────────────────────────────────────┘│
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

### 3.5 Communication Protocols

The system utilizes a variety of communication protocols for internal (on-device) and external (network/cloud) interactions:

*   **I2C (Inter-Integrated Circuit)**:
    *   **Usage**: For communication with the DHT20 temperature/humidity sensor and the 16x2 LCD display.
    *   **Characteristics**: A two-wire serial protocol (SDA - GPIO 11, SCL - GPIO 12) suitable for short-distance communication with multiple devices on the same bus, each having a unique address.
*   **ADC (Analog-to-Digital Converter)**:
    *   **Usage**: To read analog voltage levels from the soil moisture sensor (GPIO 1) and the Light Dependent Resistor (LDR) (GPIO 2).
    *   **Characteristics**: The ESP32's ADC converts these analog readings into digital values that the microcontroller can process.
*   **PWM (Pulse Width Modulation)**:
    *   **Usage**: To control the speed of the cooling fan (GPIO 6).
    *   **Characteristics**: By varying the duty cycle of a digital pulse train (25kHz frequency as per implementation), the average power delivered to the fan motor can be controlled.
*   **GPIO (General Purpose Input/Output)**:
    *   **Usage**: For controlling the status LED (GPIO 48) with simple on/off states, and for the bit-banged, timing-critical protocol used by the NeoPixel LED strip (GPIO 8).
*   **WiFi (IEEE 802.11 b/g/n)**:
    *   **Usage**: For connecting the ESP32 to the local wireless network, providing internet access essential for cloud communication and time synchronization.
    *   **Characteristics**: Standard wireless communication protocol operating in the 2.4GHz band, managed by the WiFi Manager task.
*   **HTTP (Hypertext Transfer Protocol)**:
    *   **Usage**: An `ESPAsyncWebServer` library is used to create a web server on the ESP32. This server hosts a captive portal for initial WiFi setup (SSID and password entry) when the device cannot connect to a saved network and enters Access Point mode.
    *   **Characteristics**: Standard client-server protocol for web communication.
    *   **Web Server Routes** (from `src/tasks/wifi_manager.cpp`):
        ```
        HTTP Endpoints:
        ├── GET  /                    (Serves the main WiFi setup page)
        ├── GET  /scan               (Triggers a scan for available WiFi networks)
        ├── POST /save               (Receives and saves new WiFi credentials)
        ├── GET  /status             (Provides connection status information)
        └── GET  /reset              (Clears saved WiFi credentials)
        ```
    *   **Setup Web Interface** (Simplified HTML structure):
        ```html
        <!DOCTYPE html>
        <html>
        <head>
            <title>ESP32 WiFi Setup</title>
            <!-- Styles and scripts for captive portal -->
        </head>
        <body>
            <h2>ESP32 WiFi Configuration</h2>
            <form action="/save" method="POST">
                <label>Network:</label>
                <select name="ssid" id="ssidSelect"><!-- Networks from /scan --></select>
                <label>Password:</label>
                <input type="password" name="password" required>
                <button type="submit">Connect</button>
            </form>
        </body>
        </html>
        ```
*   **MQTT (Message Queuing Telemetry Transport)**:
    *   **Usage**: The primary protocol for IoT communication between the ESP32 and the ThingsBoard.io cloud platform. Managed by the MQTT Manager task.
    *   **Characteristics**: A lightweight, publish-subscribe network protocol ideal for constrained devices.
    *   **Functions**:
        *   Publishing sensor telemetry data (e.g., temperature, humidity).
        *   Publishing device attributes (e.g., fan state, light state).
        *   Subscribing to and receiving RPC (Remote Procedure Call) commands from the ThingsBoard dashboard.
        *   Publishing responses to RPC commands.
    *   **Broker**: `demo.thingsboard.io` (as per documentation)
    *   **Port**: 1883 (non-encrypted)
    *   **Authentication**: Device-specific access token (e.g., `aqeffw396e9r45c0w8th`).
    *   **Topic Structure**:
        ```
        ThingsBoard MQTT Topics:
        ├── v1/devices/me/telemetry          (Publish sensor data)
        ├── v1/devices/me/attributes         (Publish device state/attributes)
        ├── v1/devices/me/rpc/request/+      (Subscribe to RPC commands from server)
        └── v1/devices/me/rpc/response/{id}  (Publish responses to specific RPC command)
        ```
    *   **Telemetry Data Format** (Example JSON payload):
        ```json
        {
          "temperature": 25.6,
          "humidity": 65.2,
          "soilMoisture": 450,
          "lightLevel": 650,
          "fanEnabled": true,
          "fanSpeed": 47.1, // Percentage
          "fanPWM": 120,    // Raw PWM value
          "lightEnabled": false
        }
        ```
*   **UART (Universal Asynchronous Receiver/Transmitter)**:
    *   **Usage**: For serial communication with a host computer (typically via USB-to-UART bridge).
    *   **Functions**: Firmware programming/flashing, outputting debug messages (`ESP_LOGI`, `ESP_LOGE`, etc.), and general system logging.
    *   **Baud Rate**: Commonly 115200.

### 3.6 Control Logic and Data Flow

#### 3.6.1 Control Logic
The system implements a "Hybrid Control Mode," allowing for both automatic sensor-driven actions and manual overrides from the cloud dashboard. State synchronization ensures consistency.

**1. Hybrid Control Architecture Principles:**
*   **Automatic Control**: System components (fan, lights) react to sensor inputs based on predefined logic.
*   **Manual Override**: Users can send commands via the ThingsBoard dashboard to directly control actuators, temporarily overriding automatic logic.
*   **State Synchronization**: The system ensures that the manual state is acknowledged and, where appropriate, informs the automatic logic to prevent conflicts or immediate reversions. For instance, a manual fan activation might set a flag (`manualOverride`) that the automatic control logic respects for a period or until a new significant sensor change.

**2. Fan Control Logic (`src/tasks/fan_control.cpp`)**
*   **Control Method**: PWM (Pulse Width Modulation) at 25kHz frequency via ESP32's LEDC peripheral.
*   **Automatic Temperature-Based Control**:
    *   If `currentTemperature > TEMP_FAN_ON` (e.g., 30°C), the fan is automatically turned ON to a default speed.
    *   If `currentTemperature` is below this threshold, the fan is automatically turned OFF.
    *   The `lastAutoFanState` variable tracks the state desired by the automatic logic.
*   **Manual Control**:
    *   RPC commands like `setFan` (true/false) or `setFanSpeed` (percentage) can override the automatic state.
    *   When a manual command is received, `manualOverride` flag is set.
    *   The `setFanState()` function updates `currentFanSpeed`, writes to PWM, and critically, updates `lastAutoFanState` to reflect the new manual state, preventing immediate reversion by the auto logic.
*   **Diagrammatic Representation**:
    ```
    Temperature Reading (from DHT20 via Global Var)
             │
             ▼
    ┌─────────────────┐
    │ Temperature     │     > TEMP_FAN_ON (e.g. 30°C)   ┌─────────────────┐
    │ Threshold Check │────────────────────────────────►│ Auto: Fan ON    │
    │ (in fan_control)│                                 │ (Default Speed) │
    └─────────────────┘                                 └─────────────────┘
             │                                                     │
             │ <= TEMP_FAN_ON                                      │
             ▼                                                     │
    ┌─────────────────┐                                            │
    │ Auto: Fan OFF   │                                            │
    └─────────────────┘                                            │
             │                                                     │
             └─────────────────────┬───────────────────────────────┘
                                   │ (Reconciliation with Manual Override)
                                   ▼
                          ┌─────────────────┐
                          │   Current Fan   │
                          │ State (PWM Out) │
                          └─────────────────┘
                                   ▲
                                   │ (Manual Command via MQTT)
                          ┌─────────────────┐
                          │ Manual Override │◄── ThingsBoard RPC
                          │ (setFan,        │    (e.g. "setFan")
                          │  setFanSpeed)   │
                          └─────────────────┘
    ```
*   **State Synchronization Snippet** (Conceptual from `SYSTEM_DESIGN_AND_IMPLEMENTATION.md`):
    ```cpp
    // In fan_control.cpp (illustrative)
    void setFanState(bool enabled, float speedPercent /* from RPC or auto */, const char* source) {
        if (enabled) {
            currentFanPwm = mapSpeedToPwm(speedPercent); // Convert % to PWM value
        } else {
            currentFanPwm = SPEED_OFF_PWM;
        }
        ledcWrite(PWM_CHANNEL, currentFanPwm);
        currentFanState = enabled; // Update global/static state

        if (strcmp(source, "DASHBOARD") == 0) {
            manualOverrideActive = true;
            // Potentially sync lastAutoFanState if manual action should reset auto baseline
            lastAutoFanState = enabled; 
        } else if (strcmp(source, "AUTO_TEMP") == 0) {
            manualOverrideActive = false; // Auto control is now dominant
            lastAutoFanState = enabled;
        }
        // publishFanState(currentFanState, speedPercent, source); // To ThingsBoard
    }
    ```

**3. Light Control Logic (`src/tasks/light_and_led.cpp`)**
*   **LEDs**: 4x NeoPixel RGB LEDs (controlled via GPIO 8).
*   **Control Method**: Hybrid automatic (sensor-based) + manual.
*   **Sensor-Based Control with Hysteresis**:
    *   If `currentLightLevel < LIGHT_THRESHOLD_ON` (e.g., 300 lux), LEDs are turned ON.
    *   If `currentLightLevel > LIGHT_THRESHOLD_OFF` (e.g., 500 lux), LEDs are turned OFF.
    *   If `currentLightLevel` is between `LIGHT_THRESHOLD_ON` and `LIGHT_THRESHOLD_OFF` (hysteresis zone), the LEDs maintain their previous state (`lastSensorLightState`). This prevents rapid toggling.
*   **Manual Control**:
    *   RPC commands like `setLight` (true/false) can override the automatic state.
    *   Similar to fan control, a `manualLightOverride` flag and state synchronization would be used.
*   **Diagrammatic Representation**:
    ```
    Light Sensor Reading (LDR via Global Var)
             │
             ▼
    ┌─────────────────────────────────────────┐
    │           Hysteresis Logic              │
    │        (in light_and_led task)          │
    ├─────────────────────────────────────────┤
    │ • < LIGHT_THRESHOLD_ON (e.g. 300 lux) → Sensor Wants Light ON      │
    │ • > LIGHT_THRESHOLD_OFF (e.g. 500 lux)→ Sensor Wants Light OFF     │
    │ • Else (Hysteresis Zone)            → Maintain lastSensorLightState│
    └─────────────────────────────────────────┘
             │
             ▼
    ┌─────────────────┐
    │ State Change    │    Yes    ┌─────────────────┐
    │ Detected?       │──────────►│ Update NeoPixels│
    │(sensorWantsLight│           │ Publish State   │
    │!= lastSensor...)│           └─────────────────┘
    └─────────────────┘
             │ No Change
             ▼
    ┌─────────────────┐
    │ Maintain Current│
    │ NeoPixel State  │
    └─────────────────┘
             │ (Reconciliation with Manual Override)
             ▼
    ┌─────────────────┐
    │ Current Light   │
    │State (NeoPixels)│
    └─────────────────┘
             ▲
             │ (Manual Command via MQTT)
    ┌─────────────────┐
    │ Manual Override │◄── ThingsBoard RPC
    │(e.g. "setLight")│    (e.g. "setLight")
    └─────────────────┘
    ```
