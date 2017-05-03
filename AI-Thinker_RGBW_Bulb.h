#pragma once
#ifndef _AI-THINKER_RGBW_BULB_
#define _AI-THINKER_RGBW_BULB_

#include <my9291.h>         // https://github.com/xoseperez/my9291
#include <ArduinoJson.h>    // https://github.com/bblanchon/ArduinoJson
#include "FS.h"

#include "config.h"

#define MY9291_DI_PIN       13
#define MY9291_DCKI_PIN     15

#define COLOR_TEMP_HA_MIN_IN_MIRED   154    // Home Assistant minimum color temperature
#define COLOR_TEMP_HA_MAX_IN_MIRED   500    // Home Assistant maximum color temperature
#define COLOR_TEMP_MIN_IN_KELVIN     1000   // minimum color temperature
#define COLOR_TEMP_MAX_IN_KELVIN     15000  // maximum color temperature

typedef struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t white;
};

enum CMD {
  CMD_NOT_DEFINED,
  CMD_STATE_CHANGED,
  CMD_BRIGHTNESS_CHANGED,
  CMD_COLOR_CHANGED,
  CMD_WHITE_CHANGED,
  CMD_COLOR_TEMP_CHANGED,
  CMD_SAVE_STATE
};

#define EFFECT_NOT_DEFINED_NAME "None"
#define EFFECT_RAMBOW_NAME      "Rainbow"
#define EFFECT_RAINBOW_DELAY    20

#define EFFECT_LIST EFFECT_RAMBOW_NAME

enum EFFECT {
  EFFECT_NOT_DEFINED,
  EFFECT_RAMBOW,
};

class AIRGBWBulb {
  public:
    AIRGBWBulb(void);
    
    void      init(void);
    void      loop(void);
    
    bool      getState(void);
    bool      setState(bool p_state);
    
    uint8_t   getBrightness(void);
    bool      setBrightness(uint8_t p_brightness);
    
    Color     getColor(void);
    bool      setColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue);
    
    bool      setWhite(uint8_t p_white);
    
    uint16_t  getColorTemperature(void);
    bool      setColorTemperature(uint16_t p_colorTemperature);
    
    bool      setEffect(const char* p_effect);

    bool      isDiscovered(void);
    void      isDiscovered(bool p_isDiscovered);

  private:
    my9291*   m_my9291;
    
    uint8_t   m_brightness;
    Color     m_color;
    uint16_t  m_colorTemperature;

    bool      m_isDiscovered = false;
    
    bool      setColor();

    void      rainbowEffect(uint8_t p_index);
};

#endif
