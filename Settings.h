#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "menu.h"
#include "Screen.h"
#include "Button.h"
#include "Led.h"

class Settings
{
  public:
    Settings(Screen *screen, Button **buttons, const uint8_t number_of_buttons, Led **leds, const uint8_t number_of_leds);
    void startSettingsMode();
    void settingsMode();
    void exitSettingsMode();
  private:
    Screen *_screen;
    Button **_buttons;
    Led    **_leds;
    const uint8_t _number_of_buttons;
    const uint8_t _number_of_leds;

    static const uint8_t NUMBER_OF_MENU_OPTIONS = 2;
    const char* MENU_OPTIONS[NUMBER_OF_MENU_OPTIONS] = {"Led Intensity", "Other..."};

    const int LED_FLASHING_ON  = 500;
    const int LED_FLASHING_OFF = 500;
    const uint8_t _leds_index[3] = {10, 11, 13};
    const uint8_t _exit = 1;
    const uint8_t _pg_up = 2;
    const uint8_t _pd_dn = 3;
    const uint8_t _change = 4;
    const uint8_t _value_up = 5;
    const uint8_t _value_dn = 6;
    const int UP = -1;
    const int DOWN = 1;
    uint8_t led_intensity_value = 1;
    uint8_t _selected_option = 0;

    void showMenuOptions();
};

#endif