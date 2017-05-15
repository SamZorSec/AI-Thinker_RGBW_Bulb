#include "AI-Thinker_RGBW_Bulb.h"

volatile uint8_t effect = EFFECT_NOT_DEFINED;

///////////////////////////////////////////////////////////////////////////
//   CONSTRUCTOR, INIT() AND LOOP()
///////////////////////////////////////////////////////////////////////////
AIRGBWBulb::AIRGBWBulb(void) {
  // creates a new instance of the my9291 driver
  m_my9291 = new my9291(MY9291_DI_PIN, MY9291_DCKI_PIN, MY9291_COMMAND_DEFAULT);

#if defined(GAMMA_CORRECTION)
  m_isGammaCorrectionEnabled = true;
#endif
}

void AIRGBWBulb::init(void) {
  // sets initial color to white (255, 255, 255)
  //setColor(255, 255, 255);

  // sets initial brightness to 100% (255)
  //setBrightness(255);

  // sets initial color temperature to 500 (Home Assistant max value)
  //setColorTemperature(COLOR_TEMP_HA_MIN_IN_MIRED); // COLOR_TEMP_HA_MIN_IN_MIRED: white point

  // sets initial white to 100% (255)
  setWhite(255);

  // sets initial state to false
  m_my9291->setState(true);
}

void AIRGBWBulb::loop(void) {
  // TODO: handles effects
  static unsigned long last = millis();
  switch (effect) {
    case EFFECT_NOT_DEFINED:
      break;
    case EFFECT_RAMBOW:
      static unsigned char count = 0;
      if (millis() - last > EFFECT_RAINBOW_DELAY) {
        last = millis();
        rainbowEffect(count++);
      }
      break;
    case EFFECT_BLINK:
      if (millis() - last > EFFECT_BLINK_DELAY) {
        last = millis();
        setState(!getState());
      }
      break;
  }
}

///////////////////////////////////////////////////////////////////////////
//   STATE
///////////////////////////////////////////////////////////////////////////
bool AIRGBWBulb::getState(void) {
  return m_my9291->getState();
}

bool AIRGBWBulb::setState(bool p_state) {
  // checks if the given state is different from the actual state
  if (p_state != m_my9291->getState())
    m_my9291->setState(p_state);
  else
    return false;

  return m_my9291->getState() == p_state;
}

///////////////////////////////////////////////////////////////////////////
//   BRIGHTNESS
///////////////////////////////////////////////////////////////////////////
uint8_t AIRGBWBulb::getBrightness(void) {
  return m_brightness;
}

bool AIRGBWBulb::setBrightness(uint8_t p_brightness) {
  // checks if the value is smaller, bigger or equal to the actual brightness value
  if (p_brightness < 0 || p_brightness > 255 || p_brightness == m_brightness)
    return false;

  // saves the new brightness value
  m_brightness = p_brightness;

  if (m_color.white != 0)
    //m_color.white = p_brightness;
    return setWhite(p_brightness);

  return setColor();
}

///////////////////////////////////////////////////////////////////////////
//   RGB COLOR
///////////////////////////////////////////////////////////////////////////
Color AIRGBWBulb::getColor(void) {
  return m_color;
}

bool AIRGBWBulb::setColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue) {
  if ((p_red < 0 || p_red > 255) || (p_green < 0 || p_green > 255) || (p_blue < 0 || p_blue > 255))
    return false;

  // saves the new values
  m_color.red = p_red;
  m_color.green = p_green;
  m_color.blue = p_blue;

  // switches off the white leds
  m_color.white = 0;

  return setColor();
}

bool AIRGBWBulb::setColor() {
  // maps the RGB values with the actual brightness
  uint8_t red = (m_isGammaCorrectionEnabled) ? pgm_read_byte(&gamma8[m_color.red]) : m_color.red;
  uint8_t green = (m_isGammaCorrectionEnabled) ? pgm_read_byte(&gamma8[m_color.green]) : m_color.green;
  uint8_t blue = (m_isGammaCorrectionEnabled) ? pgm_read_byte(&gamma8[m_color.blue]) : m_color.blue;

  red = map(red, 0, 255, 0, m_brightness);
  green = map(green, 0, 255, 0, m_brightness);
  blue = map(blue, 0, 255, 0, m_brightness);

  // sets the new color
  m_my9291->setColor((my9291_color_t) {
    red,
    green,
    blue,
    0
  });

  // checks if the values have been successfully updated
  my9291_color_t my9291Color = m_my9291->getColor();
  if (my9291Color.red != red || my9291Color.green != green || my9291Color.blue != blue)
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////////
//   WHITE COLOR
///////////////////////////////////////////////////////////////////////////
bool AIRGBWBulb::setWhite(uint8_t p_white) {
  // checks if the value is smaller, bigger or equal to the actual white value
  if (p_white < 0 || p_white > 255 || p_white == m_color.white)
    return false;

  // saves the new white value
  m_color.white = p_white;
  m_brightness = p_white;

  // switch off the RGB leds
  m_color.red = 0;
  m_color.green = 0;
  m_color.blue = 0;

  // adjusts the white value
  m_my9291->setColor((my9291_color_t) {
    0,
    0,
    0,
    m_color.white
  });

  // checks if the value has been successfully updated
  my9291_color_t my9291Color = m_my9291->getColor();
  if (my9291Color.white != m_color.white)
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////////
//   GETTER/SETTER COLOR TEMPERATURE
///////////////////////////////////////////////////////////////////////////
uint16_t AIRGBWBulb::getColorTemperature(void) {
  return m_colorTemperature;
}

bool AIRGBWBulb::setColorTemperature(uint16_t p_colorTemperature) {
  // checks if the value is equal to the actual color temperature
  if (p_colorTemperature < COLOR_TEMP_HA_MIN_IN_MIRED || p_colorTemperature == m_colorTemperature || p_colorTemperature > COLOR_TEMP_HA_MAX_IN_MIRED)
    return false;

  // switches off the white leds
  m_color.white = 0;

  // saves the new colour temperature
  m_colorTemperature = p_colorTemperature;

  // https://fr.wikipedia.org/wiki/Mired
  // http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/
  // M = 1000000 / T <> T [kelvin] = 1000000 / M [mired]
  //      int tmpKelvin = 1000000 / m_colorTemperature;
  //
  //      if (tmpKelvin < 1000) {
  //        tmpKelvin = 1000;
  //      } else if (tmpKelvin > 40000) {
  //        tmpKelvin = 40000;
  //      }

  int tmpKelvin = map(p_colorTemperature, COLOR_TEMP_HA_MIN_IN_MIRED, COLOR_TEMP_HA_MAX_IN_MIRED, COLOR_TEMP_MAX_IN_KELVIN, COLOR_TEMP_MIN_IN_KELVIN);
  tmpKelvin = tmpKelvin / 100;

  // computes red
  if (tmpKelvin <= 66) {
    m_color.red = 255;
  } else {
    float red = tmpKelvin - 60;
    red = 329.698727446 * pow(red, -0.1332047592);
    if (red < 0) {
      m_color.red = 0;
    } else if (red > 255) {
      m_color.red = 255;
    } else {
      m_color.red = red;
    }
  }

  // computes green
  if (tmpKelvin <= 66) {
    float green = tmpKelvin;
    green = 99.4708025861 * log(green) - 161.1195681661;
    if (green < 0) {
      m_color.green = 0;
    } else if (green > 255) {
      m_color.green = 255;
    } else {
      m_color.green = green;
    }
  } else {
    float green = tmpKelvin - 60;
    green = 288.1221695283 * pow(green, -0.0755148492);
    if (green < 0) {
      m_color.green = 0;
    } else if (green > 255) {
      m_color.green = 255;
    } else {
      m_color.green = green;
    }
  }

  // computes blue
  if (tmpKelvin <= 66) {
    m_color.blue = 255;
  } else {
    if (tmpKelvin <= 19) {
      m_color.blue = 0;
    } else {
      float blue = tmpKelvin - 10;
      blue = 138.5177312231 * log(blue) - 305.0447927307;
      if (blue < 0) {
        m_color.blue = 0;
      } else if (blue > 255) {
        m_color.blue = 255;
      } else {
        m_color.blue = blue;
      }
    }
  }

  return setColor();
}

///////////////////////////////////////////////////////////////////////////
//   EFFECTS
///////////////////////////////////////////////////////////////////////////
bool AIRGBWBulb::setEffect(const char* p_effect) {
  if (strcmp(p_effect, EFFECT_NOT_DEFINED_NAME) == 0) {
    effect = EFFECT_NOT_DEFINED;
    return true;
  } else if (strcmp(p_effect, EFFECT_RAINBOW_NAME) == 0) {
    effect = EFFECT_RAMBOW;
    return true;
  } else if (strcmp(p_effect, EFFECT_BLINK_NAME) == 0) {
    effect = EFFECT_BLINK;
    return true;
  }

  return false;
}

// https://github.com/xoseperez/my9291/blob/master/examples/esp8285/esp8285_rainbow.cpp
void AIRGBWBulb::rainbowEffect(uint8_t p_index) {
  if (p_index < 85) {
    setColor(p_index * 3, 255 - p_index * 3, 0);
  } else if (p_index < 170) {
    p_index -= 85;
    setColor(255 - p_index * 3, 0, p_index * 3);
  } else {
    p_index -= 170;
    setColor(0, p_index * 3, 255 - p_index * 3);
  }
}

///////////////////////////////////////////////////////////////////////////
//   MQTT DISCOVERY
///////////////////////////////////////////////////////////////////////////
bool AIRGBWBulb::isDiscovered(void) {
  return m_isDiscovered;
}

void AIRGBWBulb::isDiscovered(bool p_isDiscovered) {
  m_isDiscovered = p_isDiscovered;
}

///////////////////////////////////////////////////////////////////////////
//   GAMMA CORRECTION
///////////////////////////////////////////////////////////////////////////
bool AIRGBWBulb::isGammaCorrectionEnabled(void) {
  return m_isGammaCorrectionEnabled;
}

void AIRGBWBulb::isGammaCorrectionEnabled(bool p_isGammaCorrectionEnabled) {
  m_isGammaCorrectionEnabled = p_isGammaCorrectionEnabled;
}

