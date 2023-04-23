#include "Button.h"

Button::Button(uint8_t pin, uint8_t cc, uint8_t action, uint8_t settings_action)
{
  pinMode(pin, INPUT_PULLUP);
  button_pin = pin;
  button_cc = cc;
  button_momentary = false;
  button_action = action;
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
    if (button_action > 0) {
      button_pressed = true;
      button_time_start = millis();
    }
  }
  if (button_debouncer->rose()) {
    if (button_momentary) {
      sendControlChange();
    }
    if (button_action > 0) {
      button_pressed = false;
    }
  }
  if (button_pressed) {
    button_time_now = millis();
    if (button_time_now - button_time_start > ACTION_TIME) {
      button_pressed = false;
      return button_action;
    }
  }
  return 0;
}

void Button::sendControlChange()
{
  if (button_cc > 0) {
    usbMIDI.sendControlChange(button_cc, top_velocity, channel);
  }
}

void Button::changeMomentary(bool state)
{
  button_momentary = state;
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