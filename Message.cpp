#include "Message.h"

Message::Message()
{
}

void Message::setButtonsAndLeds(Button* buttons[], uint8_t number_of_buttons, Led* leds[], uint8_t number_of_leds)
{
  this->buttons = buttons;
  this->number_of_buttons = number_of_buttons;
  this->leds = leds;
  this->number_of_leds = number_of_leds;
}

void Message::process(uint8_t channel, uint8_t control, uint8_t value)
{
  if (isLedMessage(channel)) {
    processLedMessage(control, value);
  }
  if (isButtonModeMessage(channel)) {
    processButtonModeMessage(control, value);
  }
  if (isSettingMessage(channel)) {
    processSettingMessage(control, value);
  }
}

bool Message::isLedMessage(uint8_t channel)
{
  if (channel == LEDS_MIDI_CHANNEL) {
    return true;
  }
  return false;
}

bool Message::isButtonModeMessage(uint8_t channel)
{
  if (channel == BUTTON_MODE_MIDI_CHANNEL) {
    return true;
  }
  return false;
}

bool Message::isSettingMessage(uint8_t channel)
{
  if (channel == SETTING_CHANNEL) {
    return true;
  }
  return false;
}

void Message::processLedMessage(uint8_t cc, uint8_t value)
{
  int led_index = getLedIndexByCc(cc);
  if (led_index > -1) {
    if (value == 127) {
      leds[led_index]->on();
    } else {
      leds[led_index]->off();
    }
  }
}

void Message::processButtonModeMessage(uint8_t cc, uint8_t value)
{
  int button_index = getButtonIndexByMomentaryCc(cc);
  if (button_index > -1) {
    bool momentary_state = (value == 127) ? true : false;
    buttons[button_index]->changeMomentary(momentary_state);
  }
}

int Message::getLedIndexByCc(uint8_t cc)
{
  for (uint8_t i = 0; i < number_of_leds; i++) {
    if (cc == leds[i]->getLedCc()) {
      return i;
    }
  }
  return -1;
}

int Message::getButtonIndexByMomentaryCc(uint8_t cc)
{
  for (uint8_t i = 0; i < number_of_buttons; i++) {
    if (cc == buttons[i]->getMomentaryCc()) {
      return i;
    }
  }
  return -1;
}

void Message::processSettingMessage(uint8_t cc, uint8_t value)
{
  Settings::setSettingValue(cc, value);
}
