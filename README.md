# ESP32 Environmental Monitor with Telegram

My first ESP32 project: a temperature and humidity monitor
that lets me check sensor readings through a Telegram bot.

The prototype uses a DHT22 sensor connected to an ESP32
on a breadboard. The firmware is written in C++ using
the Arduino framework.

## Features

- Wi-Fi connection.
- Temperature readings in degrees Celsius.
- Relative humidity readings in percent.
- Telegram commands:
  - /start: display the available commands.
  - /status: request sensor readings and an environmental status.
- Access restricted to a configured Telegram chat ID.
- Detection of sensor reading errors.
- Startup notification when the device is online.

## Environmental status

When /status is requested, the program classifies the readings:

- Temperature above 28 °C: high temperature.
- Otherwise, humidity above 70%: high humidity.
- Otherwise: normal conditions.

Temperature takes priority if both thresholds are exceeded.

This version reports readings on request. It does not send
automatic threshold alerts or store historical measurements.

## Hardware

- ESP32 development board.
- DHT22 temperature and humidity sensor.
- Breadboard.
- Jumper wires.
- USB cable for power and programming.

The firmware configures the sensor data signal on GPIO 4.

## Software

- Arduino framework for ESP32.
- WiFi and WiFiClientSecure.
- UniversalTelegramBot.
- ArduinoJson.
- DHT sensor library.
- Network time synchronization before Telegram communication.

## Project files

- esp32_telegram_monitor/esp32_telegram_monitor.ino:
  main firmware.
- esp32_telegram_monitor/secrets.example.h:
  configuration template without real credentials.
- .gitignore:
  excludes the local credentials file.
- images/:
  prototype photographs.

## Configuration

1. Install the ESP32 support package in Arduino IDE.
2. Install UniversalTelegramBot, ArduinoJson and the
   Adafruit DHT sensor library with its dependencies.
3. Open esp32_telegram_monitor.ino.
4. Copy secrets.example.h into the same folder and
   rename the copy to secrets.h.
5. Enter your Wi-Fi credentials, Telegram bot token
   and authorized chat ID in your local secrets.h.
6. Select your ESP32 board and serial port.
7. Compile and upload the firmware.
8. Open your Telegram bot and send /start or /status.

Never upload secrets.h or screenshots containing credentials.

## What I learned

- Connecting a sensor to a microcontroller on a breadboard.
- Reading temperature and humidity in C++.
- Connecting an ESP32 to Wi-Fi.
- Integrating sensor readings with Telegram commands.
- Handling invalid sensor readings.
- Applying basic chat access restrictions.

## Limitations and future improvements

- Wi-Fi and time synchronization waits have no timeout.
- No dedicated Wi-Fi reconnection strategy is implemented.
- Measurements are requested through Telegram.
- Possible future improvements include automatic alerts,
  data logging and connection recovery.

## Author

Pau Serrano Romay
https://github.com/pausse

## Prototype photos

### DHT22 sensor on the breadboard
![DHT22 sensor mounted on a breadboard](images/dht22-breadboard.jpeg)

### ESP32 development board
![ESP32 development board connected via USB](images/esp32-board.jpeg)
