# wearable
A wearable sleeve project for riders, utilizing the Nano 33 IoT microcontroller with integrated MAX30102, GSR, and DHT22 sensors. The system continuously monitors physiological and environmental data, connects to WiFi, and communicates with a Telegram bot to provide real-time updates and sensor data.


# Rider Sleeve Project

## Overview
This project involves the development of a wearable sleeve for riders, equipped with a Nano 33 IoT microcontroller and multiple sensors (MAX30102, GSR, and DHT22). The sleeve monitors physiological and environmental data, connects to WiFi, and communicates with a Telegram bot for real-time updates and data retrieval.

## Features
- Continuous monitoring of heart rate, skin conductance, and temperature
- WiFi connectivity
- Interaction with a Telegram bot for data requests and updates

## Components
- Nano 33 IoT
- MAX30102 sensor (heart rate and SpO2)
- GSR sensor (Galvanic Skin Response)
- DHT22 sensor (temperature and humidity)

## Setup
1. Clone this repository.
2. Follow the setup instructions for each sensor.
3. Connect the sensors to the Nano 33 IoT according to the provided schematics.
4. Upload the Arduino code to the Nano 33 IoT.
5. Run the Python script to start interacting with the Telegram bot.

## Usage
- Start the system and wait for it to connect to WiFi.
- Use the Telegram bot to request data by sending commands.
- The bot will respond with the requested sensor data or IP address.

## Library
- #include <WiFiNINA.h>
- #include <ArduinoJson.h>
- #include <Wire.h>
- #include <DHT.h>
- #include <DHT_U.h>
- #include <SkinConductance.h>
- #include "DFRobot_BloodOxygen_S.h"
- #include <Adafruit_Sensor.h>

##telegram bot helps
- https://www.youtube.com/watch?v=vZtm1wuA2yc
- https://www.youtube.com/watch?v=GZbeL5AcTgw

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


