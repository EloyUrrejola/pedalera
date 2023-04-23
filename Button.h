#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <Bounce2.h>
#define Button_h

class Button
{
  public:
    Button(uint8_t, uint8_t, uint8_t, uint8_t);
    void changeMomentary(bool state);
    uint8_t changed();
    uint8_t settingsChanged();
    void sendControlChange();
    
  private:
    const uint8_t channel = 1;
    const uint8_t top_velocity = 127;
    const uint8_t low_velocity = 0;
    const uint8_t debouncer_interval = 5;
    const unsigned int ACTION_TIME = 300;

    uint8_t button_pin;
    uint8_t button_cc;
    bool button_momentary;
    uint8_t button_action;
    uint8_t button_settings_action;
    bool button_pressed;
    unsigned long button_time_now;
    unsigned long button_time_start;
    Bounce *button_debouncer;
};

#endif