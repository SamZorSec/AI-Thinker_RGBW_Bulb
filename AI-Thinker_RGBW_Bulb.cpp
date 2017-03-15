#include "AI-Thinker_RGBW_Bulb.h"

AIRGBWBulb::AIRGBWBulb(void) {

}

void AIRGBWBulb::init(void) {
  m_my9291 = new my9291(MY9291_DI_PIN, MY9291_DCKI_PIN, MY9291_COMMAND_DEFAULT);
}

void AIRGBWBulb::loop(void) {
}

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

uint8_t AIRGBWBulb::getBrightness(void) {
  return m_brightness;
}

bool AIRGBWBulb::setBrightness(uint8_t p_brightness) {
  // checks if the value is smaller, bigger or equal to the actual brightness value
  if (p_brightness < 0 || p_brightness > 255 || p_brightness == m_brightness)
    return false;

  // saves the new brightness value
  m_brightness = p_brightness;

  return setColor();
}

Color AIRGBWBulb::getColor(void) {
  return m_color;
}

bool AIRGBWBulb::setColor() {
  // maps the RGB values with the actual brightness
  m_color.red = map(m_color.red, 0, 255, 0, m_brightness);
  m_color.green = map(m_color.green, 0, 255, 0, m_brightness);
  m_color.blue = map(m_color.blue, 0, 255, 0, m_brightness);
  
  // sets the new color
  m_my9291->setColor((my9291_color_t) {
    m_color.red,
    m_color.green,
    m_color.blue,
    0
  });

  // checks if the values have been successfully updated
  my9291_color_t my9291Color = m_my9291->getColor();
  if (my9291Color.red != m_color.red || my9291Color.green != m_color.green || my9291Color.blue != m_color.blue)
    return false;

  return true;
}

bool AIRGBWBulb::setColor(uint8_t p_red, uint8_t p_green, uint8_t p_blue) {
  if ((p_red < 0 || p_red > 255) || (p_green < 0 || p_green > 255) || (p_blue < 0 || p_blue > 255))
    return false;
    
  // saves the new values
  m_color.red = p_red;
  m_color.green = p_green;
  m_color.blue = p_blue;

  return setColor();
}

uint8_t AIRGBWBulb::getWhite(void) {
  return m_white;
}

bool AIRGBWBulb::setWhite(uint8_t p_white) {
  // checks if the value is smaller, bigger or equal to the actual white value
  if (p_white < 0 || p_white > 255 || p_white == m_white)
    return false;

  // saves the new white value
  m_white = p_white;

  // adjusts the white value
  m_my9291->setColor((my9291_color_t) {
    0,
    0,
    0,
    m_white
  });

  // checks if the value has been successfully updated
  my9291_color_t my9291Color = m_my9291->getColor();
  if (my9291Color.white != m_white)
    return false;

  return true;
}

