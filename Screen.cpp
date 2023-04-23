#include "Screen.h"

Screen::Screen(Adafruit_SSD1351 *adafruit)
{
  screen = adafruit;
}

void Screen::begin()
{
  screen->begin();
}

void Screen::clean()
{
  screen->fillScreen(OLED_Color_Black);
}

void Screen::writeSong(char* song, char* part)
{
  screen->fillRect(0, 0, 128, 56, song_name_bg);

  screen->setFont(song_name_font);
  screen->setTextSize(song_name_size);
  screen->setTextWrap(false);

  uint16_t width = getTextWidth(song);
  int16_t centered_x = getCenteredX(width);

  screen->setTextColor(song_name_color);
  screen->setCursor(centered_x, song_name_y);
  screen->print(song);

  screen->setFont(song_part_font);
  screen->setTextSize(song_part_size);

  width = getTextWidth(part);
  centered_x = getCenteredX(width);

  screen->setTextColor(song_part_color);
  screen->setCursor(centered_x, song_part_y);
  screen->print(part);
}

void Screen::writeChord(char* chord)
{
  screen->fillRect(0, 71, 128, 50, chord_bg);

  screen->setFont(chord_font);
  screen->setTextSize(chord_size);

  uint16_t width = getTextWidth(chord);
  int16_t centered_x = getCenteredX(width);

  screen->setTextColor(chord_color);
  screen->setCursor(centered_x, chord_y);
  screen->print(chord);
}

void Screen::removeChord()
{
  screen->fillRect(0, 71, 128, 50, chord_bg);
}

void Screen::writeSettingsTitle()
{
  screen->fillRect(0, 0, 128, 50, song_name_bg);

  screen->setFont(settings_font);
  screen->setTextSize(settings_size);
  char text[] = "SETTINGS";

  uint16_t width = getTextWidth(text);
  int16_t centered_x = getCenteredX(width);

  screen->setCursor(centered_x, settings_y);
  screen->setTextColor(settings_color);
  screen->print("SETTINGS");

  screen->drawLine(centered_x, 28, centered_x + width, 28, settings_color);
}

void Screen::showSettingOptions(const char **menu, const uint8_t number_of_options, uint8_t selected_option)
{
  uint8_t line_height = 26;
  for (uint8_t i = 0; i < number_of_options; i++) {
    screen->setFont(settings_font);
    screen->setTextSize(settings_size);
    screen->setCursor(0, line_height * (i + 2));
    if (i == selected_option) {
      screen->setTextColor(OLED_Color_Yellow);
    } else {
      screen->setTextColor(settings_color);
    }
    screen->print(menu[i]);
  }
}

uint16_t Screen::getTextWidth(const char* text)
{
  int16_t x, y;
  uint16_t w, h;
  screen->getTextBounds(text, 0, 0, &x, &y, &w, &h);
  return w;
}

int16_t Screen::getCenteredX(uint16_t width)
{
  int16_t centered_x = (screen->width() / 2) - floor(width / 2);
  return centered_x > 0 ? centered_x : 0;
}

void Screen::writeSongList(const char ** songs, int selected_song_index)
{
  clean();
  for (uint8_t i = 0; i < 6; i ++) {
    screen->setFont(settings_song_name_font);
    screen->setTextSize(settings_song_name_size);
    screen->setTextWrap(false);
    uint16_t width = getTextWidth(songs[i]);
    int16_t centered_x = getCenteredX(width);
    if (i == selected_song_index) {
      screen->setTextColor(settings_song_name_color_selected);
    } else {
      screen->setTextColor(settings_song_name_color);
    }
    screen->setCursor(centered_x, settings_song_name_height * (i + 1));
    screen->print(songs[i]);
  }
}
