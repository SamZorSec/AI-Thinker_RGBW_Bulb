/*
  Alternative firmware for AI Thinker RGBW bulbs, based on ESP8266.
  See the README at https://github.com/mertenats/AI-Thinker_RGBW_Bulb for more information.
  Licensed under the MIT license.
*/

#include <ESP8266WiFi.h>          // https://github.com/esp8266/Arduino
#include <PubSubClient.h>         // https://github.com/knolleary/pubsubclient
#include <ArduinoJson.h>          // https://github.com/bblanchon/ArduinoJson
#include "AI-Thinker_RGBW_Bulb.h"

#if defined(DEBUG_TELNET)
  WiFiServer  telnetServer(23);
  WiFiClient  telnetClient;
  #define     DEBUG_PRINT(x)    telnetClient.print(x)
  #define     DEBUG_PRINTLN(x)  telnetClient.println(x)
#elif defined(DEBUG_SERIAL)
  #define     DEBUG_PRINT(x)    Serial.print(x)
  #define     DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define     DEBUG_PRINT(x)
  #define     DEBUG_PRINTLN(x)
#endif

AIRGBWBulb    bulb;
WiFiClient    wifiClient;
PubSubClient  mqttClient(wifiClient);

///////////////////////////////////////////////////////////////////////////
//   TELNET
///////////////////////////////////////////////////////////////////////////
/*
   Function called to handle Telnet clients
   https://www.youtube.com/watch?v=j9yW10OcahI
*/
#if defined(DEBUG_TELNET)
void handleTelnet(void) {
  if (telnetServer.hasClient()) {
    if (!telnetClient || !telnetClient.connected()) {
      if (telnetClient) {
        telnetClient.stop();
      }
      telnetClient = telnetServer.available();
    } else {
      telnetServer.available().stop();
    }
  }
}
#endif

///////////////////////////////////////////////////////////////////////////
//   MQTT
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//  SETUP() AND LOOP()
///////////////////////////////////////////////////////////////////////////

void setup() {
#if defined(DEBUG_SERIAL)
  Serial.begin(115200);
#elif defined(DEBUG_TELNET)
  telnetServer.begin();
  telnetServer.setNoDelay(true);
#endif

  bulb.init();
}

void loop() {
#if defined(DEBUG_TELNET)
  // handle the Telnet connection
  handleTelnet();
#endif

  yield();
  
  bulb.loop();
}
