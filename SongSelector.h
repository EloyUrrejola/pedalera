#ifndef SONG_SELECTOR_H
#define SONG_SELECTOR_H

#include <Arduino.h>
#include <string>

#include "Button.h"
#include "Led.h"
#include "Screen.h"
#include "SongList.h"

class SongSelector
{
  public:
    SongSelector();
    void setScreenButtonsAndLeds(Screen *screen, Button **buttons, uint8_t number_of_buttons, Led **leds, uint8_t number_of_leds);
    void startSongSelectorMode();
    void songSelectorMode();
    void exitSongSelectorMode();

  private:
    Screen *screen;
    Button **buttons;
    Led    **leds;
    uint8_t number_of_buttons;
    uint8_t number_of_leds;

    const int LED_FLASHING_ON  = 500;
    const int LED_FLASHING_OFF = 500;
    const uint8_t leds_index[4] = {8, 9, 10, 11};
    const uint8_t _cancel = 1;
    const uint8_t _pg_up = 2;
    const uint8_t _pd_dn = 3;
    const uint8_t _select = 6;
    const int UP = -1;
    const int DOWN = 1;

    uint8_t _temp_song_index;
    uint8_t _current_song_index;
    std::string* _song_list;

    void showSongSelectorPanel(uint8_t song_index, int direction);
    void getRangeSongs(uint8_t first_song, uint8_t number_of_visible_songs, const char** songs);
};

#endif