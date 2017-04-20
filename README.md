# AI-Thinker RGBW Bulb
Alternative firmware for Home Assistant

<< THIS FIRMWARE IS STILL IN DEVELOPMENT >>

## Features
- MQTT discovery
- MQTT JSON Light
  - State
  - RGB
  - White
  - Brightness
  - Color temperature
  - Effect
    - Rainbow

## How to
1. Rename `config.example.h`to `config.h`
2. Define your WiFi SSID and password
3. Define your MQTT settings
4. Install the external libraries (PubSubClient, ArduinoJson)
4. Define `MQTT_MAX_PACKET_SIZE` to `512`in `Arduino/libraries/pubsubclient/src/PubSubClient.h`
5. Flash the firmware (`Generic ESP8285 module`, `115200`, `1M (128K SPIFFS)`)
