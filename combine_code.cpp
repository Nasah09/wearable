#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>
#include <SkinConductance.h>
#include "DFRobot_BloodOxygen_S.h"
#include <Adafruit_Sensor.h>

// WiFi credentials
const char* ssid = "user name";
const char* password = "password";

// DHT Sensor
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// MAX30102 Sensor
#define I2C_ADDRESS 0x57
DFRobot_BloodOxygen_S_I2C MAX30102(&Wire, I2C_ADDRESS);

// GSR Sensor
SkinConductance sc(A6);

// Web server
WiFiServer server(80);

void connectToWiFi();
void checkWiFiConnection();

void setup() {
  //Serial.begin(9600);  // Initialize serial communication for debugging
  pinMode(LED_BUILTIN, OUTPUT);

  // Connect to WiFi
  connectToWiFi();

  // Start the server
  server.begin();

  // Initialize sensors
  dht.begin();
  Wire.begin();

  if (MAX30102.begin() == false) {
    digitalWrite(LED_BUILTIN, HIGH); // Indicate init failure
    //Serial.println("MAX30102 initialization failed!");
    while (true) {
      delay(1000);
    }
  }
  digitalWrite(LED_BUILTIN, LOW); // Indicate init success
  MAX30102.sensorStartCollect();

  sc.reset();
}

void loop() {
  // Check and reconnect if WiFi is disconnected
  checkWiFiConnection();

  // Check for client request
  WiFiClient client = server.available();
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Endpoint for sensor data
    if (request.indexOf("GET /sensor-data") != -1) {
      // Read DHT22 sensor data
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();

      // Read MAX30102 sensor data
      MAX30102.getHeartbeatSPO2();
      float spo2 = MAX30102._sHeartbeatSPO2.SPO2;
      float heartRate = MAX30102._sHeartbeatSPO2.Heartbeat;

      // Read GSR sensor data
      sc.update();
      float scr = sc.getSCR();
      float scl = sc.getSCL();
      int raw = sc.getRaw();

      // Create JSON response
      DynamicJsonDocument doc(500);
      doc["temperature"] = temperature;
      doc["humidity"] = humidity;
      doc["spo2"] = spo2;
      doc["heartRate"] = heartRate;
      doc["scr"] = scr;
      doc["scl"] = scl;
      doc["raw"] = raw;
      String response;
      serializeJson(doc, response);

      // Send HTTP response
      client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
      client.print(response);

      // Print to Serial for debugging
      //Serial.println("Sensor Data Sent:");
      //Serial.println(response);
    }

    // Endpoint for IP address
    else if (request.indexOf("GET /ip-address") != -1) {
      // Create JSON response
      DynamicJsonDocument doc(200);
      doc["ip"] = WiFi.localIP().toString();
      String response;
      serializeJson(doc, response);

      // Send HTTP response
      client.print("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n");
      client.print(response);

      // Print to Serial for debugging
      //Serial.println("IP Address Sent:");
      //Serial.println(response);
    }

    // Handle invalid endpoints
    else {
      client.print("HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n");
      client.print("Not Found");

      // Print to Serial for debugging
      //Serial.println("Invalid Endpoint Accessed");
    }

    delay(1000);
    client.stop();
  }

  // Delay for sensor update (every 4 seconds for MAX30102)
  delay(4000);
}

void connectToWiFi() {
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 10) {
    delay(1000);
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, LOW); // Indicate successful connection
    //Serial.println("Connected to WiFi");
    //Serial.print("IP Address: ");
    //Serial.println(WiFi.localIP());
  } else {
    digitalWrite(LED_BUILTIN, HIGH); // Indicate failure to connect
    //Serial.println("Failed to connect to WiFi");
  }
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, HIGH); // Indicate WiFi is disconnected
    connectToWiFi();
  } else {
    digitalWrite(LED_BUILTIN, LOW); // Indicate WiFi is connected
  }
}
