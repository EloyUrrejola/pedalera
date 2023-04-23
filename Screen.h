#ifndef SCREEN_H
#define SCREEN_H

#include <Arduino.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#include <Fonts/MyFonts/Roboto_Condensed_24.h>
#include <Fonts/MyFonts/Roboto_Condensed_Bold_12.h>
#include <Fonts/MyFonts/Roboto_Condensed_Bold_18.h>
#include <Fonts/MyFonts/Roboto_Condensed_Bold_24.h>
#include <Fonts/MyFonts/Open_Sans_Condensed_Light_24.h>
#include <Fonts/MyFonts/Open_Sans_Condensed_Bold_18.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

class Screen
{
  public:
    Screen(Adafruit_SSD1351 *adafruit);
    void begin();
    void clean();
    void writeSong(char* song, char* part);
    void writeChord(char* chord);
    void removeChord();
    void writeSettingsTitle();
    void showSettingOptions(const char **menu, const uint8_t number_of_options, uint8_t selected_option);
    void writeSongList(const char ** songs, int selected_song_index);

  private:
    const int OLED_Color_Black        = 0x0000;
    const int OLED_Color_Blue         = 0x001F;
    const int OLED_Color_Red          = 0xF800;
    const int OLED_Color_Green        = 0x07E0;
    const int OLED_Color_Cyan         = 0x07FF;
    const int OLED_Color_Magenta      = 0xF81F;
    const int OLED_Color_Yellow       = 0xFFE0;
    const int OLED_Color_White        = 0xFFFF;

    const GFXfont *song_name_font = &Open_Sans_Condensed_Bold_18;
    const int  song_name_color = OLED_Color_White;
    const int  song_name_bg    = OLED_Color_Black;
    const uint8_t song_name_x     = 0;
    const uint8_t song_name_y     = 20;
    const uint8_t song_name_size  = 1;

    const GFXfont *song_part_font = &Open_Sans_Condensed_Light_24;
    const int song_part_color  = OLED_Color_Cyan;
    const uint8_t song_part_x     = 0;
    const uint8_t song_part_y     = 50;
    const uint8_t song_part_size  = 1;

    const GFXfont *chord_font = &FreeSansBold18pt7b;
    const int  chord_color = 0x8FCF;
    const int  chord_bg    = OLED_Color_Black;
    const uint8_t chord_x     = 0;
    const uint8_t chord_y     = 100;
    const uint8_t chord_size  = 1;

    const GFXfont *settings_font = &Open_Sans_Condensed_Bold_18;
    const int settings_color = OLED_Color_Cyan;
    const uint8_t settings_y     = 20;
    const uint8_t settings_size  = 1;

    const GFXfont *settings_song_name_font = &Open_Sans_Condensed_Bold_18;
    const int settings_song_name_color = 0xBDF7;
    const int settings_song_name_bg    = OLED_Color_Black;
    const int settings_song_name_color_selected = OLED_Color_Cyan;
    const uint8_t settings_song_name_x      = 0;
    const uint8_t settings_song_name_height = 21;
    const uint8_t settings_song_name_size  = 1;

    Adafruit_SSD1351 *screen;
    uint16_t getTextWidth(const char* text);
    int16_t getCenteredX(uint16_t width);
};

#endif