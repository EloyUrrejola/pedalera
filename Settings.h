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

    const uint8_t _number_of_setting_buttons = 4;
    const uint8_t _buttons_index[4] = {11,12,16,17};

    const int LED_FLASHING_ON  = 500;
    const int LED_FLASHING_OFF = 500;
    const uint8_t _number_of_leds_flashing = 1;
    const uint8_t _leds_index[1] = {11};

    const uint8_t _exit = 1;
    const uint8_t _up = 2;
    const uint8_t _dn = 3;
    const uint8_t _select = 6;

    char settings_title[9] = "SETTINGS";
    const uint8_t led_intensity_menu_option = 0;
    const uint8_t audio_stereo_sep_option = 0;
    const uint8_t audio_midi_left_option = 1;
    const uint8_t audio_midi_right_option = 2;
    const uint8_t audio_bass_left_option = 3;
    const uint8_t audio_bass_right_option = 4;
    const uint8_t _menu_min_options[NUMBER_OF_MENU_OPTIONS] = {1, 0};
    const uint8_t _menu_max_options[NUMBER_OF_MENU_OPTIONS] = {3, 10};
    uint8_t _menu_option_values[NUMBER_OF_MENU_OPTIONS]     = {Led::getLedIntensityLevel(), 0};
    const uint8_t _menu_option_ccs[NUMBER_OF_MENU_OPTIONS]  = {25, 0};

    const uint8_t _audio_min_options[5] = {0,0,0,0,0};
    const uint8_t _audio_max_options[5] = {1,10,10,10,10};
    uint8_t _audio_values[5] = {0,0,10,0,10};

    const uint8_t _audio_ccs[5] = {20, 21, 22, 23, 24};
    const uint8_t* _submenu_option_ccs[2] = {nullptr, _audio_ccs};

    uint8_t *_submenu_option_values[2] = {
      NULL,
      _audio_values
    };
    const uint8_t *_submenu_min_options[2] = {
      NULL,
      _audio_min_options
    };
    const uint8_t *_submenu_max_options[2] = {
      NULL,
      _audio_max_options
    };
    const uint8_t _settings_midi_chanel = 4;

    void startFlashingLeds();
    void updateFlashingLeds();
    bool isMenuOptionChange(uint8_t action, uint8_t number_of_options, uint8_t selected_menu);
    void changeOption(uint8_t action, char **menu_options, uint8_t number_of_options, uint8_t &selected_option, uint8_t *option_values);
    void selectSubmenu(char **&menu_options, uint8_t &number_of_options, uint8_t &selected_option, uint8_t *&option_values, uint8_t *&min_values, uint8_t *&max_values, uint8_t *&option_ccs);
    void exitOption(uint8_t action, char **&menu_options, uint8_t &number_of_options, uint8_t &selected_option, uint8_t &level, bool &settings_mode, uint8_t *&option_values, uint8_t *&min_values, uint8_t *&max_values, uint8_t *&option_ccs);
    void editOption(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t level, uint8_t option_value, uint8_t min_value, uint8_t max_value, uint8_t option_cc);
    bool isValueChange(uint8_t action, uint8_t selected_option, uint8_t option_value, uint8_t min_value, uint8_t max_value);
    void showMenuOptions(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t *option_values);
    void showMenuOptionEdition(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t option_value);
};

#endif