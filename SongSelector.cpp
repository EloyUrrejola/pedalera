#include "SongSelector.h"

SongSelector::SongSelector(Screen *screen, Button **buttons, const uint8_t number_of_buttons, Led **leds, const uint8_t number_of_leds, const char **songs, const uint8_t number_of_songs)
    : _number_of_buttons(number_of_buttons), _number_of_leds(number_of_leds), _songs(songs), _number_of_songs(number_of_songs)
{
  _screen = screen;
  _buttons = buttons;
  _leds = leds;
}

void SongSelector::startSongSelectorMode(uint8_t current_song_index)
{
  _current_song_index = current_song_index;
  _temp_song_index = _current_song_index;
  _screen->clean();
  for (uint8_t i = 0; i < 4; i++) {
    _leds[_leds_index[i]]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, -1);
  }
  showSongSelectorPanel(_current_song_index, DOWN);
}

void SongSelector::songSelectorMode()
{
  bool settings_mode = true;
  while (settings_mode) {
    for (uint8_t i = 0; i < 4; i++) {
      _leds[_leds_index[i]]->flashUpdate();
    }
    for (uint8_t i = 0; i < _number_of_buttons; i++) {
      uint8_t action = _buttons[i]->settingsChanged();
      if (action == _pg_up && _temp_song_index > 0) {
        _temp_song_index = _temp_song_index + UP;
        showSongSelectorPanel(_temp_song_index, UP);
      }
      if (action == _pd_dn && _temp_song_index < _number_of_songs - 1) {
        _temp_song_index = _temp_song_index + DOWN;
        showSongSelectorPanel(_temp_song_index, DOWN);
      }
      if (action == _cancel) {
        settings_mode = false;
      }
      if (action == _save) {
        if (_temp_song_index != -1) {
          _current_song_index = _temp_song_index;
          _temp_song_index = -1;
          settings_mode = false;
          usbMIDI.sendProgramChange(_current_song_index, 1);
        }
      }
    }
  }
}

void SongSelector::exitSongSelectorMode()
{
  for (uint8_t i = 0; i < 4; i++) {
    _leds[_leds_index[i]]->off();
  }
}

void SongSelector::showSongSelectorPanel(uint8_t song_index, int direction)
{
  uint8_t first_song = 0;
  uint8_t selected_song = 1;
  if (direction == UP) {
    if (song_index > 4) {
      first_song = song_index - 4;
      selected_song = song_index - first_song;
    } else {
      first_song = 0;
      selected_song = song_index;
    }
  }
  if (direction == DOWN) {
    uint8_t last_song = _number_of_songs - 1;
    if (song_index > last_song - 4) {
      first_song = last_song - 5;
      selected_song = 5 - last_song + song_index;
    } else {
      if (song_index > 0) {
        first_song = song_index - 1;
        selected_song = 1;
      } else {
        first_song = song_index;
        selected_song = 0;
      }
    }
  }
  const char** songs = getRangeSongs(first_song);
  _screen->writeSongList(songs, selected_song);
}

const char** SongSelector::getRangeSongs(uint8_t first_song)
{
  const char** songs = new const char*[6];
  for (int i = 0; i < 6; i++) {
    songs[i] = _songs[first_song + i];
  }
  return songs;
}
