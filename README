# ESP32 Modbus Temperature Monitor

This project implements a web-based temperature monitoring system using an ESP32 microcontroller. It reads temperature data from three Modbus RTU sensors via RS485, serves the data through a web interface, and controls an LED based on temperature readings.

## Features
- Connects to WiFi for network access
- Reads temperature from three Modbus RTU sensors (IDs 1-3)
- Web server with real-time temperature display
- LED control via web interface and automatic temperature-based control
- Uses SPIFFS for serving HTML, CSS, and JavaScript files
- Robust error handling and retry mechanism for Modbus communication

## Hardware Requirements
- ESP32 development board
- Three Modbus RTU temperature sensors (supporting RS485)
- LED connected to GPIO 2
- RS485 to TTL converter
- WiFi network access

## Software Requirements
- Arduino IDE or PlatformIO
- Required Arduino libraries:
  - `WiFi.h`
  - `AsyncTCP` (https://github.com/me-no-dev/AsyncTCP)
  - `ESPAsyncWebServer` (https://github.com/me-no-dev/ESPAsyncWebServer)
  - `SPIFFS`
  - `ModbusMaster` (https://github.com/4-20ma/ModbusMaster)

## Installation
1. **Clone the Repository**
  ```
   bash
   git clone <repository-url>
   cd esp32-modbus-temperature-monitor
  ```

2.  **Install Libraries**
    -   Open Arduino IDE
    -   Go to Sketch > Include Library > Manage Libraries
    -   Search and install the required libraries listed above
3.  **Configure WiFi**
    -   Open the .ino file
    -   Update the WiFiConfig struct with your WiFi credentials:

        ```
        struct  WiFiConfig
        {
          const char* ssid = "YOUR_SSID";
          const char* password = "YOUR_PASSWORD";
        };
        ```

4.  **Upload SPIFFS Data**
    -   Create a data folder in your project directory
    -   Add index.html, style.css, and script.js files (see example files below)
    -   Use the ESP32 Sketch Data Upload tool to upload files to SPIFFS
5.  **Upload Code**
    -   Connect your ESP32 to your computer
    -   Select your ESP32 board in Arduino IDE
    -   Upload the sketch

Web Interface Files
-------------------

Create these files in the data folder: index.html, style.css, script.js


Usage
-----

1.  Power on the ESP32
2.  Check the Serial Monitor (9600 baud) for the IP address
3.  Open a web browser and navigate to http://<ESP32-IP-ADDRESS>
4.  View real-time temperature readings and control the LED

Configuration
-------------

-   Modify HardwareConfig for different pin assignments
-   Adjust ModbusConfig for retry settings and update intervals
-   Update temperature conversion in readModbusTemperature if your sensors use a different scale

Troubleshooting
---------------

-   No WiFi connection: Verify credentials and signal strength
-   No temperature readings: Check RS485 wiring and sensor IDs
-   Web interface not loading: Ensure SPIFFS files are uploaded correctly
-   Serial output: Use Serial Monitor for debug messages

License
-------

This project is licensed under the MIT License - see the <LICENSE> file for details.

Contributing
------------

Feel free to submit issues or pull requests to improve this project!