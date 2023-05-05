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
  _screen->writeSettingsTitle(settings_title);
  startFlashingLeds();
}

void Settings::settingsMode()
{
  bool settings_mode = true;
  uint8_t selected_option = 0;
  uint8_t number_of_options = NUMBER_OF_MENU_OPTIONS;
  uint8_t level = 1;
  char **menu_options = (char**)MENU;
  uint8_t *option_values = _menu_option_values;
  uint8_t *min_values = (uint8_t*)_menu_min_options;
  uint8_t *max_values = (uint8_t*)_menu_max_options;
  uint8_t *option_ccs = (uint8_t*)_menu_option_ccs;
  showMenuOptions(menu_options, number_of_options, selected_option, option_values);

  while (settings_mode) {
    updateFlashingLeds();
    for (uint8_t i = 0; i < _number_of_setting_buttons; i++) {
      uint8_t action = _buttons[_buttons_index[i]]->settingsChanged();
      changeOption(action, menu_options, number_of_options, selected_option, option_values);
      if (action == _select) {
        if (level == 1 && SUBMENUS[selected_option] != NULL) {
          selectSubmenu(menu_options, number_of_options, selected_option, option_values, min_values, max_values, option_ccs);
          level++;
        } else {
          editOption(
            menu_options,
            number_of_options,
            selected_option,
            level,
            option_values[selected_option],
            min_values[selected_option],
            max_values[selected_option],
            option_ccs[selected_option]
          );
          showMenuOptions(menu_options, number_of_options, selected_option, option_values);
        }
      }
      exitOption(action, menu_options, number_of_options, selected_option, level, settings_mode, option_values, min_values, max_values, option_ccs);
    }
  }
}

void Settings::changeOption(uint8_t action, char **menu_options, uint8_t number_of_options, uint8_t &selected_option, uint8_t *option_values)
{
  if (isMenuOptionChange(action, number_of_options, selected_option)) {
    if (action == _up) {
      selected_option--;
    }
    if (action == _dn) {
      selected_option++;
    }
    showMenuOptions(menu_options, number_of_options, selected_option, option_values);
  }
}

bool Settings::isMenuOptionChange(uint8_t action, uint8_t number_of_options, uint8_t selected_option)
{
  if ((action == _up && selected_option > 0) || (action == _dn && selected_option < number_of_options - 1)) {
    return true;
  }
  return false;
}

void Settings::selectSubmenu(char **&menu_options, uint8_t &number_of_options, uint8_t &selected_option, uint8_t *&option_values, uint8_t *&min_values, uint8_t *&max_values, uint8_t *&option_ccs)
{
  _screen->clean();
  _screen->writeSettingsTitle(menu_options[selected_option]);
  menu_options = (char**)SUBMENUS[selected_option];
  option_values = _submenu_option_values[selected_option];
  min_values = (uint8_t*)_submenu_min_options[selected_option];
  max_values = (uint8_t*)_submenu_max_options[selected_option];
  number_of_options = NUMBER_OF_SUBMENU_OPTIONS[selected_option];
  option_ccs = (uint8_t*)_submenu_option_ccs[selected_option];
  selected_option = 0;
  showMenuOptions(menu_options, number_of_options, selected_option, option_values);
}

void Settings::exitOption(uint8_t action, char **&menu_options, uint8_t &number_of_options, uint8_t &selected_option, uint8_t &level, bool &settings_mode, uint8_t *&option_values, uint8_t *&min_values, uint8_t *&max_values, uint8_t *&option_ccs)
{
  if (action == _exit) {
    if (level > 1) {
      menu_options = (char**)MENU;
      number_of_options = NUMBER_OF_MENU_OPTIONS;
      option_values = _menu_option_values;
      min_values = (uint8_t*)_menu_min_options;
      max_values = (uint8_t*)_menu_max_options;
      option_ccs = (uint8_t*)_menu_option_ccs;
      selected_option = 0;
      level--;
      _screen->clean();
      _screen->writeSettingsTitle(settings_title);
      showMenuOptions(menu_options, number_of_options, selected_option, option_values);
    } else {
      settings_mode = false;
    }
  }
}

void Settings::editOption(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t level, uint8_t option_value, uint8_t min_value, uint8_t max_value, uint8_t option_cc)
{
  bool options_mode = true;
  showMenuOptionEdition(menu_options, number_of_options, selected_option, option_value);
  while (options_mode) {
    updateFlashingLeds();
    for (uint8_t i = 0; i < _number_of_setting_buttons; i++) {
      uint8_t action = _buttons[_buttons_index[i]]->settingsChanged();
      if (isValueChange(action, selected_option, option_value, min_value, max_value)) {
        if (action == _up) {
          option_value++;
        }
        if (action == _dn) {
          option_value--;
        }
        if (level == 1 && selected_option == led_intensity_menu_option) {
          Led::setLedIntensityLevel(option_value);
        }
        if (option_cc) {
          usbMIDI.sendControlChange(option_cc, (option_value * 127) / max_value, _settings_midi_chanel);
        }
        showMenuOptionEdition(menu_options, number_of_options, selected_option, option_value);
      }
      if (action == _select) {
        _menu_option_values[selected_option] = option_value;
        options_mode = false;
      }
      if (action == _exit) {
        if (level == 1 && selected_option == led_intensity_menu_option) {
          Led::setLedIntensityLevel(option_value);
        }
        options_mode = false;
      }
    }
  }
}

bool Settings::isValueChange(uint8_t action, uint8_t selected_option, uint8_t option_value, uint8_t min_value, uint8_t max_value)
{
  if ((action == _up && option_value < max_value) || (action == _dn && option_value > min_value)) {
    return true;
  }
  return false;
}

void Settings::startFlashingLeds()
{
  for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
    _leds[_leds_index[i]]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, -1);
  }
}

void Settings::updateFlashingLeds()
{
  for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
    _leds[_leds_index[i]]->flashUpdate();
  }
}

void Settings::showMenuOptions(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t *option_values)
{
  _screen->showSettingOptions(menu_options, number_of_options, selected_option, option_values);
}

void Settings::showMenuOptionEdition(char **menu_options, uint8_t number_of_options, uint8_t selected_option, uint8_t option_value)
{
  _screen->showSettingOptionEdition(menu_options, number_of_options, selected_option, option_value);
}

void Settings::exitSettingsMode()
{
  for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
    _leds[_leds_index[i]]->off();
  }
}
