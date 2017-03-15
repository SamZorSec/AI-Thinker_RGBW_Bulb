#pragma once
#ifndef _AI-THINKER_RGBW_BULB_
#define _AI-THINKER_RGBW_BULB_

#include <ESP8266WiFi.h>    // https://github.com/esp8266/Arduino
#include <my9291.h>         // https://github.com/xoseperez/my9291
#include <PubSubClient.h>   // https://github.com/knolleary/pubsubclient
#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson
#include "config.h"

#define MY9291_DI_PIN       13
#define MY9291_DCKI_PIN     15

typedef struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
};

class AIRGBWBulb {
  public:
    AIRGBWBulb(void);
    void    init(void);
    void    loop(void);
    bool    getState(void);
    bool    setState(bool p_state);
    uint8_t getBrightness(void);
    bool    setBrightness(uint8_t p_brightness);
    Color   getColor(void);
    bool    setColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue);
    uint8_t getWhite(void);
    bool    setWhite(uint8_t p_white);

  private:
    uint8_t m_brightness;
    Color   m_color;
    uint8_t m_white;
    my9291* m_my9291;
    bool    setColor(void);
};

#endif
