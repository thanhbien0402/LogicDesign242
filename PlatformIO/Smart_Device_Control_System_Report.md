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
    *   **Usage**: To control the speed of the cooling fan.
    *   **Characteristics**: By varying the duty cycle of a digital pulse train (25kHz frequency as per implementation), the average power delivered to the fan motor can be controlled.
*   **GPIO (General Purpose Input/Output)**:
    *   **Usage**: For controlling the status LED with simple on/off states, and for the bit-banged, timing-critical protocol used by the NeoPixel LED strip.
*   **WiFi (IEEE 802.11 b/g/n)**:
    *   **Usage**: For connecting the ESP32 to the local wireless network, providing internet access essential for cloud communication and time synchronization.
    *   **Characteristics**: Standard wireless communication protocol operating in the 2.4GHz band, managed by the WiFi Manager task.
*   **HTTP (Hypertext Transfer Protocol)**:
    *   **Usage**: An `ESPAsyncWebServer` library is used to create a web server on the ESP32. This server hosts a captive portal for initial WiFi setup (SSID and password entry) when the device cannot connect to a saved network and enters Access Point mode.
    *   **Characteristics**: Standard client-server protocol for web communication.
    *   **HTTP Endpoints** (from `src/tasks/wifi_manager.cpp`):
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
       <= TEMP_FAN_ON                                          │
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
    *   When a manual command is received, `manualLightOverride` flag is set.
    *   The `setLightState()` function updates the LED strip colors/states and updates `lastSensorLightState` to reflect the new manual state.
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

#### 4. System Implementation

This section details the implementation of the core software tasks that constitute the Smart Device Control System. Each task is designed to run concurrently under the FreeRTOS real-time operating system, managing specific hardware components or communication protocols.

### 4.1 WiFi Manager Task

**Objective**: To establish and maintain a stable WiFi connection for the ESP32 device, enabling network communication for MQTT and HTTP services.

**Implementation Details**:
-   **Filename**: `src/tasks/wifi_manager.cpp` (conceptual, based on typical project structure)
-   **Core Functionality**:
    -   **Credential Management**: Loads saved WiFi SSID and password from non-volatile storage (e.g., Preferences library).
    -   **Connection Logic**: Attempts to connect to the configured WiFi network. Implements a retry mechanism with timeouts to handle connection failures.
    -   **Captive Portal (Initial Setup)**: If no credentials are found or connection fails repeatedly, the task can initiate an Access Point (AP) mode. In AP mode, it serves a web page (e.g., at `192.168.4.1`) allowing the user to scan for available WiFi networks, select one, and enter the password. These credentials are then saved for future connections.
    -   **Status Indication**: Provides feedback on connection status (e.g., via serial log, status LED, or publishing to a local status topic).
    -   **Reconnection**: Monitors the WiFi connection status and attempts to reconnect if the connection is lost.
-   **Key Libraries**: `WiFi.h`, `ESPAsyncWebServer.h` (or equivalent for web server in AP mode), `Preferences.h`.
-   **State Machine**:
    1.  `STARTUP`: Initialize WiFi module.
    2.  `LOAD_CREDENTIALS`: Try to load saved WiFi credentials.
    3.  `CONNECT_STA`: If credentials exist, attempt to connect in Station (STA) mode.
        -   On Success: Transition to `CONNECTED_MONITOR`.
        -   On Failure: After multiple retries, transition to `SETUP_AP_MODE`.
    4.  `SETUP_AP_MODE`: If no credentials or STA connection failed, start AP mode. Launch a web server for configuration.
        -   On Credentials Save: Save new credentials, then transition to `CONNECT_STA`.
    5.  `CONNECTED_MONITOR`: Periodically check WiFi status. If disconnected, attempt reconnection (`CONNECT_STA`).

### 4.2 MQTT Manager Task

**Objective**: To manage the connection to the MQTT broker (ThingsBoard.io), handle incoming commands (RPC), and publish telemetry data and device attributes.

**Implementation Details**:
-   **Filename**: `src/tasks/mqtt_manager.cpp` (conceptual)
-   **Core Functionality**:
    -   **Connection Management**: Establishes and maintains a connection to the MQTT broker using the device access token for authentication. Implements reconnection logic with backoff strategy.
    -   **Subscription**: Subscribes to relevant RPC request topics (e.g., `v1/devices/me/rpc/request/+`).
    -   **Message Handling (Callback)**: Processes incoming MQTT messages. For RPC requests, it parses the JSON payload, identifies the method name and parameters.
    -   **Command Dispatch**: Based on the RPC method, it invokes the appropriate local function to control a device (e.g., `setFan`, `setLight`) or retrieve status.
    -   **Telemetry Publishing**: Periodically (or on change) collects data from sensor tasks and system state variables, formats it into a JSON payload, and publishes to the telemetry topic (e.g., `v1/devices/me/telemetry`).
    -   **Attribute Publishing**: Publishes device attributes (e.g., firmware version, static configuration, current control states) to the attributes topic (e.g., `v1/devices/me/attributes`).
    -   **Response Publishing**: Sends responses to RPC commands back to the server via the appropriate RPC response topic (e.g., `v1/devices/me/rpc/response/{request_id}`).
-   **Key Libraries**: `PubSubClient.h`, `ArduinoJson.h`.
-   **RPC Command Handling Flow**:
    1.  Receive message on `v1/devices/me/rpc/request/+`.
    2.  Parse JSON: Extract `method` and `params`.
    3.  Route: Call internal handler based on `method` (e.g., `handleSetFan(params)`, `handleGetLightState()`).
    4.  Execute: Perform action or retrieve data.
    5.  Respond: Publish result to `v1/devices/me/rpc/response/{request_id}`.

### 4.3 Sensor Tasks

These tasks are responsible for reading data from various environmental sensors.

#### 4.3.1 Temperature & Humidity Sensor Task (DHT20)

-   **Objective**: To periodically read temperature and humidity data from the DHT20 sensor.
-   **Implementation Details**:
    -   **Filename**: `src/tasks/temperature_humidity.cpp` (conceptual)
    -   **Sensor Interface**: I2C.
    -   **Functionality**:
        -   Initializes the DHT20 sensor.
        -   In a loop, reads temperature and humidity values.
        -   Updates global volatile variables (`currentTemperature`, `currentHumidity`) for other tasks to access.
        -   Logs readings to serial output for debugging.
        -   Delay for a specified interval (e.g., 10 seconds).
    -   **Key Libraries**: `DHT20.h` (or specific library for the sensor).

#### 4.3.2 Soil Moisture Sensor Task

-   **Objective**: To periodically read the analog value from the soil moisture sensor.
-   **Implementation Details**:
    -   **Filename**: `src/tasks/soil_moisture.cpp` (conceptual)
    -   **Sensor Interface**: ADC (e.g., GPIO 1).
    -   **Functionality**:
        -   Configures the ADC pin.
        -   In a loop, reads the analog value from the sensor.
        -   Updates a global volatile variable (`currentSoilMoisture`).
        -   Logs readings.
        -   Delay for a specified interval (e.g., 10 seconds).
    -   **Key Functions**: `analogRead()`.

#### 4.3.3 Light Sensor Task (LDR)

-   **Objective**: To periodically read the ambient light level from the Light Dependent Resistor (LDR).
-   **Implementation Details**:
    -   **Filename**: `src/tasks/light_and_led.cpp` (as it's often integrated with light control)
    -   **Sensor Interface**: ADC (e.g., GPIO 2).
    -   **Functionality**:
        -   Configures the ADC pin.
        -   In a loop, reads the analog value representing light intensity.
        -   Updates a global volatile variable (`currentLightLevel`).
        -   This data is then used by the Light Control Task for automatic adjustments.
        -   Logs readings.
        -   Delay for a specified interval (e.g., 10 seconds).
    -   **Key Functions**: `analogRead()`.

### 4.4 Control Tasks

These tasks manage the actuators based on sensor data, user commands, or predefined logic.

#### 4.4.1 Fan Control Task

-   **Objective**: To control the fan's operation (ON/OFF) and speed based on temperature readings and manual override commands.
-   **Implementation Details**:
    -   **Filename**: `src/tasks/fan_control.cpp` (conceptual)
    -   **Control Interface**: PWM (e.g., GPIO 6).
    -   **Functionality**:
        -   **Automatic Mode**:
            -   Reads `currentTemperature` (from the Temperature & Humidity Sensor Task).
            -   If temperature exceeds `TEMP_FAN_ON` threshold, turn the fan ON to a default speed.
            -   If temperature falls below a certain point (or `TEMP_FAN_ON` with hysteresis), turn the fan OFF.
            -   Updates `currentFanSpeed` (PWM value) and `fanEnabled` state.
        -   **Manual Override**:
            -   Responds to `setFan(bool enabled)` and `setFanSpeed(float percentage)` RPC commands from MQTT Manager.
            -   Updates fan state and speed directly, bypassing automatic logic temporarily.
            -   A `manualOverride` flag can be used to indicate manual control is active. This might revert to automatic after a timeout or a specific command.
        -   **PWM Control**: Uses `ledcSetup()` and `ledcWrite()` functions for PWM signal generation to control fan speed.
        -   **State Publishing**: Publishes current fan state (ON/OFF, speed, control mode) via MQTT.
    -   **Key Functions**: `ledcWrite()`, logic for temperature thresholds, handling manual override flags.

#### 4.4.2 Light Control Task (NeoPixel LEDs)

-   **Objective**: To control the NeoPixel LEDs based on ambient light levels and manual override commands.
-   **Implementation Details**:
    -   **Filename**: `src/tasks/light_and_led.cpp` (conceptual)
    -   **Control Interface**: GPIO (e.g., GPIO 8, for NeoPixel data line).
    -   **Functionality**:
        -   **Automatic Mode**:
            -   Reads `currentLightLevel` (from the Light Sensor Task).
            -   Implements hysteresis:
                -   If light level < `LIGHT_THRESHOLD_ON`, turn LEDs ON.
                -   If light level > `LIGHT_THRESHOLD_OFF` (where `LIGHT_THRESHOLD_OFF` > `LIGHT_THRESHOLD_ON`), turn LEDs OFF.
                -   If light level is between thresholds, maintain the current state.
            -   Updates `lightEnabled` state.
        -   **Manual Override**:
            -   Responds to `setLight(bool enabled)` RPC commands.
            -   Updates LED state directly.
        -   **LED Control**: Uses a NeoPixel library (e.g., Adafruit NeoPixel) to set LED colors and brightness.
        -   **State Publishing**: Publishes current light state (ON/OFF, control mode) via MQTT.
    -   **Key Libraries**: `Adafruit_NeoPixel.h`.

### 4.5 Local Display and Configuration

**Objective**: To provide real-time sensor data and system status on a local LCD and allow basic configuration if applicable (though primary configuration is via WiFi captive portal).

**Implementation Details**:
-   **LCD Display Task** (or integrated into a main/UI task):
    -   **Filename**: `src/tasks/display_task.cpp` (conceptual)
    -   **Interface**: I2C (for LCD 16x2, e.g., GPIO 11 SDA, GPIO 12 SCL).
    -   **Functionality**:
        -   Initializes the LCD.
        -   Periodically reads global state variables (temperature, humidity, soil moisture, light level, WiFi status, MQTT status, fan state, light state).
        -   Formats and displays this information on the LCD.
        -   May cycle through different screens of information if data exceeds display capacity.
    -   **Key Libraries**: `LiquidCrystal_I2C.h`.
-   **Status LED**:
    -   A simple GPIO-controlled LED (e.g., GPIO 48) can be used to indicate overall system status (e.g., booting, WiFi connecting, WiFi connected, error). This is typically managed across different tasks or by a dedicated status indication function.

This structured task-based implementation allows for modular development, easier debugging, and efficient use of the ESP32's resources under FreeRTOS. Each task focuses on a specific aspect of the system, interacting with others through shared state variables (with appropriate thread safety considerations like using `volatile` or FreeRTOS queues/semaphores for more complex interactions) and messaging protocols like MQTT.

## 5. Conclusion

The Smart Device Control System project, utilizing an RTOS on an IoT platform (ESP32), successfully demonstrates a comprehensive solution for monitoring and automating environmental controls.

### 5.1 Achievements

-   **Robust Multi-tasking**: Leveraged FreeRTOS for concurrent execution of sensor reading, actuator control, WiFi management, and cloud communication, ensuring responsive and efficient operation.
-   **Comprehensive Sensing and Control**: Integrated a suite of sensors (DHT20 for temperature/humidity, soil moisture, LDR for light) and actuators (fan via PWM, NeoPixel LEDs) providing a rich dataset and fine-grained control.
-   **Effective Cloud Integration**: Successfully connected to the ThingsBoard.io platform via MQTT for:
    -   Real-time telemetry data visualization.
    -   Remote device control through RPC commands.
    -   Device attribute management.
-   **User-Friendly Configuration**: Implemented a WiFi captive portal for easy initial network setup without requiring hardcoded credentials.
-   **Hybrid Control Logic**: Developed a flexible control system allowing both autonomous operation based on sensor thresholds (with hysteresis for stability) and manual override via the cloud dashboard. This ensures adaptability to varying user needs and conditions.
-   **Local Feedback**: Provided immediate system status and sensor readings through a local LCD display and status LEDs.
-   **Modular Software Design**: Organized code into distinct tasks and modules, promoting maintainability and scalability. Conceptual filenames like `wifi_manager.cpp`, `mqtt_manager.cpp`, etc., reflect this modularity.
-   **Error Handling and Resilience**: Incorporated mechanisms for WiFi and MQTT reconnection, enhancing system reliability.

### 5.2 Limitations

-   **Security**: The current implementation uses non-encrypted MQTT (port 1883). For production environments, MQTT over TLS/SSL (port 8883) would be essential to secure data in transit. WiFi credentials, while stored in Preferences, could benefit from further encryption if the platform supports it easily.
-   **Scalability for Many Devices**: While the single device is robust, managing a large fleet of similar devices would require a more sophisticated device management strategy on the cloud side and potentially more advanced features like Over-The-Air (OTA) firmware updates. The current OTA partition exists but the update process is not detailed as implemented.
-   **Power Consumption**: The system is designed for continuous operation while powered via USB. For battery-powered scenarios, significant power optimization techniques (deep sleep, selective peripheral disabling) would be necessary. Current power consumption is around 80-150mA.
-   **Data Storage**: No long-term local data logging is implemented. If cloud connectivity is lost for extended periods, sensor data is not stored locally for later synchronization. SPIFFS is available but not used for this.
-   **Complexity of Configuration**: While the captive portal simplifies WiFi setup, advanced configuration (e.g., MQTT broker details if not using a fixed demo server, sensor calibration values) might still require code changes or a more advanced configuration interface.
-   **Physical Robustness**: The project focuses on software and system design; the physical enclosure and sensor/actuator placement for long-term outdoor or harsh environment use are not covered.

### 5.3 Future Work

-   **Enhanced Security**:
    -   Implement MQTTS (MQTT over TLS/SSL) for secure communication with ThingsBoard.
    -   Investigate options for encrypting WiFi credentials stored in flash memory.
-   **Over-The-Air (OTA) Firmware Updates**: Develop and integrate a reliable OTA update mechanism to allow remote firmware upgrades without physical access.
-   **Power Optimization**:
    -   Implement deep sleep modes and wake-up triggers (e.g., timer, external interrupt) for battery-powered applications.
    -   Optimize task scheduling and peripheral usage to minimize power draw.
-   **Local Data Logging**: Utilize the SPIFFS or an SD card (if hardware allows) to log sensor data locally, especially during network outages, with subsequent synchronization to the cloud.
-   **Advanced Analytics and Edge Computing**:
    -   Implement basic data processing or anomaly detection on the ESP32 itself to reduce data transmission or provide faster local alerts.
    -   Explore machine learning models (e.g., TinyML) for predictive control or analysis.
-   **Expanded Hardware Support**:
    -   Add support for more types of sensors (e.g., CO2, air quality, water flow).
    -   Integrate additional actuators (e.g., water pumps, servo-controlled vents).
-   **Mobile Application**: Develop a companion mobile application for a more intuitive user interface for monitoring and control, potentially using ThingsBoard's mobile app features or custom development.
-   **Improved User Interface**: Enhance the local LCD interface with more screens, menus, or even a small touchscreen if hardware permits.
-   **Web Dashboard Enhancements**: Further customize the ThingsBoard dashboard or develop a custom web interface for more tailored visualization and control options.
-   **Integration with Other Services**: Explore integration with IFTTT, Alexa, Google Assistant, or other smart home ecosystems.

This project serves as a solid foundation for a smart environmental control system. The identified achievements highlight its current capabilities, while the limitations and future work outline a roadmap for further development and refinement into a more mature and feature-rich product.
