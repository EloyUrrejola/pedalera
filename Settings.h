#ifndef SETTINGS_H
#define SETTINGS_H

#include <Arduino.h>
#include "Screen.h"
#include "Button.h"
#include "Led.h"
#include "menu.h"

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

    const int LED_FLASHING_ON  = 500;
    const int LED_FLASHING_OFF = 500;
    const uint8_t _leds_index[3] = {10, 11, 13};
    const uint8_t _exit = 1;
    const uint8_t _up = 2;
    const uint8_t _dn = 3;
    const uint8_t _select = 6;
    const int UP = -1;
    const int DOWN = 1;

    const uint8_t led_intensity_menu_option = 0;
    const uint8_t _min_options[NUMBER_OF_MENU_OPTIONS] = {1, 0};
    const uint8_t _max_options[NUMBER_OF_MENU_OPTIONS] = {3, 10};
    uint8_t _option_values[NUMBER_OF_MENU_OPTIONS]     = {Led::getLedIntensityLevel(), 0};

    void selectOption(uint8_t selected_menu);
    void showMenuOptions(uint8_t selected_menu);
    void showMenuOptionEdition(uint8_t selected_menu, uint8_t option_value);
    bool isMenuOptionChange(uint8_t action, uint8_t selected_menu);
    bool isValueChange(uint8_t action, uint8_t selected_menu, uint8_t option_value);
};

#endif