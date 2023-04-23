#ifndef SONG_SELECTOR_H
#define SONG_SELECTOR_H

#include <Arduino.h>
#include "Screen.h"
#include "Button.h"
#include "Led.h"

class SongSelector
{
  public:
    SongSelector(Screen *screen, Button **buttons, const uint8_t number_of_buttons, Led **leds, const uint8_t number_of_leds, const char **songs, const uint8_t number_of_songs);
    void startSongSelectorMode(uint8_t current_song_index);
    void songSelectorMode();
    void exitSongSelectorMode();
  private:
    Screen *_screen;
    Button **_buttons;
    Led    **_leds;
    const uint8_t _number_of_buttons;
    const uint8_t _number_of_leds;
    const char **_songs;
    const uint8_t _number_of_songs;

    const int LED_FLASHING_ON  = 500;
    const int LED_FLASHING_OFF = 500;
    const uint8_t _leds_index[4] = {8, 9, 10, 11};
    const uint8_t _cancel = 1;
    const uint8_t _pg_up = 2;
    const uint8_t _pd_dn = 3;
    const uint8_t _select = 6;
    const int UP = -1;
    const int DOWN = 1;

    uint8_t _temp_song_index;
    uint8_t _current_song_index;

    void showSongSelectorPanel(uint8_t song_index, int direction);
    const char** getRangeSongs(uint8_t first_song);
};

#endif