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
  for (uint8_t i = 0; i < 3; i++) {
    _leds[_leds_index[i]]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, -1);
  }
}

void Settings::settingsMode()
{
  bool settings_mode = true;
  showMenuOptions();
  while (settings_mode) {
    for (uint8_t i = 0; i < 3; i++) {
      _leds[_leds_index[i]]->flashUpdate();
    }
    for (uint8_t i = 0; i < _number_of_buttons; i++) {
      uint8_t action = _buttons[i]->settingsChanged();
      if (action == _exit) {
        settings_mode = false;
      }
      if (action == _pg_up) {
      }
      if (action == _pd_dn) {
      }
      if (action == _change) {
      }
      if (action == _value_up) {
      }
      if (action == _value_dn) {
      }
    }
  }
}

void Settings::exitSettingsMode()
{
  for (uint8_t i = 0; i < 3; i++) {
    _leds[_leds_index[i]]->off();
  }
}

void Settings::showMenuOptions()
{
  _screen->showSettingOptions(MENU_OPTIONS, NUMBER_OF_MENU_OPTIONS, _selected_option);
}
