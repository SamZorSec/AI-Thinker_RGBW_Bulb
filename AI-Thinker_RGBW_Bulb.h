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

// https://learn.adafruit.com/led-tricks-gamma-correction/the-quick-fix
const uint8_t PROGMEM gamma8[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
};

typedef struct Color {
  uint8_t red;
  uint8_t green;
  uint8_t blue;
  uint8_t white;
};

enum CMD {
  CMD_NOT_DEFINED,
  CMD_STATE_CHANGED,
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

    bool      isGammaCorrectionEnabled(void);
    void      isGammaCorrectionEnabled(bool p_isGammaCorrectionEnabled);

  private:
    my9291*   m_my9291;
    
    uint8_t   m_brightness;
    Color     m_color;
    uint16_t  m_colorTemperature;

    bool      m_isDiscovered = false;

    bool      m_isGammaCorrectionEnabled = false;
    
    bool      setColor();

    void      rainbowEffect(uint8_t p_index);
};

#endif
