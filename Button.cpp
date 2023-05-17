#include "Button.h"

Button::Button(uint8_t pin, uint8_t cc, uint8_t push_action, uint8_t release_action, uint8_t settings_action)
{
  pinMode(pin, INPUT_PULLUP);
  button_pin = pin;
  button_cc = cc;
  button_momentary = false;
  button_push_action = push_action;
  button_release_action = release_action;
  button_settings_action = settings_action;
  button_pressed = false;
  button_debouncer = new Bounce();
  button_debouncer->attach(button_pin);
  button_debouncer->interval(debouncer_interval);
}

uint8_t Button::changed()
{
  button_debouncer->update();
  if (button_debouncer->fell()) {
    sendControlChange();
    if (button_push_action > 0) {
      return button_push_action;
    }
    if (button_release_action > 0) {
      button_pressed = true;
      button_time_start = millis();
    }
  }
  if (button_debouncer->rose()) {
    if (button_momentary) {
      sendControlChange();
    }
    if (button_release_action > 0) {
      button_pressed = false;
    }
  }
  if (button_pressed) {
    button_time_now = millis();
    if (button_time_now - button_time_start > ACTION_TIME) {
      button_pressed = false;
      return button_release_action;
    }
  }
  return 0;
}

void Button::sendControlChange()
{
  uint8_t cc = button_cc;
  if (button_momentary) {
    cc = button_momentary_cc;
  }
  if (cc > 0) {
    usbMIDI.sendControlChange(cc, top_velocity, channel);
  }
}

void Button::changeMomentary(bool state, uint8_t momentary_cc)
{
  button_momentary = state;
  button_momentary_cc = momentary_cc;
}

uint8_t Button::settingsChanged()
{
  button_debouncer->update();
  if (button_debouncer->fell()) {
    if (button_settings_action > 0) {
      return button_settings_action;
    }
  }
  return 0;
}
