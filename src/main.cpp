#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
#include <ModbusMaster.h>

// WiFi Configuration
struct WiFiConfig
{
  const char *SSID = "YOUR_SSID";
  const char *PASSWORD = "YOUR_PASSWORD";
};

// Hardware Configuration
struct HardwareConfig
{
  static constexpr int LED_PIN = 2;
  static constexpr int SERIAL_BAUD = 9600;
  static constexpr int MODBUS_BAUD = 9600;
};

// Modbus Configuration
struct ModbusConfig
{
  static constexpr int MAX_RETRIES = 3;
  static constexpr int RETRY_DELAY_MS = 500;
  static constexpr int TEMP_UPDATE_INTERVAL_MS = 1000;
};

// Global Instances
AsyncWebServer server(80);
ModbusMaster modbusNodes[3] = {ModbusMaster(), ModbusMaster(), ModbusMaster()};
float temperatures[3] = {0.0f, 0.0f, 0.0f};

// WiFi Connection
bool connectWiFi(const WiFiConfig &config)
{
  WiFi.begin(config.SSID, config.PASSWORD);
  Serial.print("Connecting to WiFi...");

  int attempts = 0;
  const int maxAttempts = 30;
  while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts)
  {
    delay(1000);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnected to WiFi");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
    return true;
  }
  Serial.println("\nFailed to connect to WiFi");
  return false;
}

// SPIFFS Initialization
bool initSPIFFS()
{
  if (!SPIFFS.begin(true))
  {
    Serial.println("Error: Failed to mount SPIFFS");
    return false;
  }
  Serial.println("SPIFFS mounted successfully");
  return true;
}

// Modbus Temperature Reading
float readModbusTemperature(ModbusMaster &node, int id)
{
  for (int attempt = 0; attempt < ModbusConfig::MAX_RETRIES; attempt++)
  {
    uint8_t result = node.readHoldingRegisters(0, 1);
    if (result == node.ku8MBSuccess)
    {
      uint16_t rawValue = node.getResponseBuffer(0);
      return rawValue / 10.0f; // Convert to temperature
    }
    delay(ModbusConfig::RETRY_DELAY_MS);
  }
  Serial.printf("❌ RS485 connection failed for ID %d!\n", id);
  return -1.0f;
}

// Web Server Routes
void setupRoutes(HardwareConfig &hwConfig)
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html"); });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "application/javascript"); });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            {
        String json = "{\"temp1\":" + String(temperatures[0]) +
                     ",\"temp2\":" + String(temperatures[1]) +
                     ",\"temp3\":" + String(temperatures[2]) + "}";
        request->send(200, "application/json", json); });

  server.on("/toggle-led", HTTP_GET, [hwConfig](AsyncWebServerRequest *request)
            {
        if (!request->hasParam("state")) {
            request->send(400, "text/plain", "Missing 'state' parameter");
            return;
        }

        String state = request->getParam("state")->value();
        bool ledState = (state == "1");
        digitalWrite(hwConfig.LED_PIN, ledState ? HIGH : LOW);
        request->send(200, "text/plain", ledState ? "LED turned ON" : "LED turned OFF"); });

  server.begin();
  Serial.println("Web server started");
}

// Setup
void setup()
{
  WiFiConfig wifiConfig;
  HardwareConfig hwConfig;

  Serial.begin(HardwareConfig::SERIAL_BAUD);
  pinMode(hwConfig.LED_PIN, OUTPUT);
  digitalWrite(hwConfig.LED_PIN, LOW);

  // Initialize system components
  if (!connectWiFi(wifiConfig) || !initSPIFFS())
  {
    while (true)
      delay(1000); // Halt if initialization fails
  }

  // Configure Modbus
  Serial2.begin(HardwareConfig::MODBUS_BAUD);
  for (int i = 0; i < 3; i++)
  {
    modbusNodes[i].begin(i + 1, Serial2);
  }

  setupRoutes(hwConfig);
}

// Main Loop
void loop()
{
  static unsigned long lastUpdate = 0;

  if (millis() - lastUpdate >= ModbusConfig::TEMP_UPDATE_INTERVAL_MS)
  {
    lastUpdate = millis();

    // Update temperatures
    for (int i = 0; i < 3; i++)
    {
      temperatures[i] = readModbusTemperature(modbusNodes[i], i + 1);
      Serial.printf("🌡 Temperature %d: %.1f°C\n", i + 1, temperatures[i]);
    }

    // Update LED based on temperature readings
    bool anyTempAboveZero = false;
    for (float temp : temperatures)
    {
      if (temp > 0)
      {
        anyTempAboveZero = true;
        break;
      }
    }
    digitalWrite(HardwareConfig::LED_PIN, anyTempAboveZero ? HIGH : LOW);
  }
}