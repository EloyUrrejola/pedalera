#include "Settings.h"

Settings::Settings(Screen *screen, Button **buttons, const uint8_t number_of_buttons, Led **leds, const uint8_t number_of_leds)
    : _number_of_buttons(number_of_buttons), _number_of_leds(number_of_leds)
{
  _screen = screen;
  _buttons = buttons;
  _leds = leds;
}

void Settings::startSettingsMode()
{
  _screen->clean();
  _screen->writeSettingsTitle();
  for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
    _leds[_leds_index[i]]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, -1);
  }
}

void Settings::settingsMode()
{
  bool settings_mode = true;
  uint8_t selected_menu = 0;
  showMenuOptions(selected_menu);
  while (settings_mode) {
    for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
      _leds[_leds_index[i]]->flashUpdate();
    }
    for (uint8_t i = 0; i < _number_of_buttons; i++) {
      uint8_t action = _buttons[i]->settingsChanged();
      if (isMenuOptionChange(action, selected_menu)) {
        if (action == _up) {
          selected_menu -= 1;
        }
        if (action == _dn) {
          selected_menu += 1;
        }
        showMenuOptions(selected_menu);
      }
      if (action == _select) {
        selectOption(selected_menu);
      }
      if (action == _exit) {
        settings_mode = false;
      }
    }
  }
}

bool Settings::isMenuOptionChange(uint8_t action, uint8_t selected_menu)
{
  if ((action == _up && selected_menu > 0) || (action == _dn && selected_menu < NUMBER_OF_MENU_OPTIONS - 1)) {
    return true;
  }
  return false;
}

void Settings::selectOption(uint8_t selected_menu)
{
  bool options_mode = true;
  uint8_t option_value = _option_values[selected_menu];
  showMenuOptionEdition(selected_menu, option_value);
  while (options_mode) {
    for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
      _leds[_leds_index[i]]->flashUpdate();
    }
    for (uint8_t i = 0; i < _number_of_buttons; i++) {
      uint8_t action = _buttons[i]->settingsChanged();
      if (isValueChange(action, selected_menu, option_value)) {
        if (action == _up) {
          option_value += 1;
        }
        if (action == _dn) {
          option_value -= 1;
        }
        if (selected_menu == led_intensity_menu_option) {
          Led::setLedIntensityLevel(option_value);
        }
        showMenuOptionEdition(selected_menu, option_value);
      }
      if (action == _select) {
        _option_values[selected_menu] = option_value;
        showMenuOptions(selected_menu);
        options_mode = false;
      }
      if (action == _exit) {
        showMenuOptions(selected_menu);
        if (selected_menu == led_intensity_menu_option) {
          Led::setLedIntensityLevel(_option_values[selected_menu]);
        }
        options_mode = false;
      }
    }
  }
}

bool Settings::isValueChange(uint8_t action, uint8_t selected_menu, uint8_t option_value)
{
  if ((action == _up && option_value < _max_options[selected_menu]) || (action == _dn && option_value > _min_options[selected_menu])) {
    return true;
  }
  return false;
}

void Settings::showMenuOptions(uint8_t selected_menu)
{
  _screen->showSettingOptions(MENU, NUMBER_OF_MENU_OPTIONS, selected_menu, _option_values);
}

void Settings::showMenuOptionEdition(uint8_t selected_menu, uint8_t option_value)
{
  _screen->showSettingOptionEdition(MENU, NUMBER_OF_MENU_OPTIONS, selected_menu, option_value);
}

void Settings::exitSettingsMode()
{
  for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
    _leds[_leds_index[i]]->off();
  }
}
