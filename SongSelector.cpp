#include "SongSelector.h"

SongSelector::SongSelector()
{
}

void SongSelector::setScreenButtonsAndLeds(Screen *screen, Button **buttons, uint8_t number_of_buttons, Led **leds, uint8_t number_of_leds)
{
  this->screen = screen;
  this->buttons = buttons;
  this->number_of_buttons = number_of_buttons;
  this->leds = leds;
  this->number_of_leds = number_of_leds;
}

void SongSelector::startSongSelectorMode()
{
  _temp_song_index = SongList::getCurrentSongIndex();
  _song_list = SongList::getSongList();
  screen->clean();
  for (uint8_t i = 0; i < 4; i++) {
    leds[leds_index[i]]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, -1);
  }
  showSongSelectorPanel(_temp_song_index, DOWN);
}

void SongSelector::songSelectorMode()
{
  bool settings_mode = true;
  while (settings_mode) {
    for (uint8_t i = 0; i < 4; i++) {
      leds[leds_index[i]]->flashUpdate();
    }
    for (uint8_t i = 0; i < number_of_buttons; i++) {
      uint8_t action = buttons[i]->settingsChanged();
      if (action == _pg_up && _temp_song_index > 0) {
        _temp_song_index = _temp_song_index + UP;
        showSongSelectorPanel(_temp_song_index, UP);
      }
      if (action == _pd_dn && _temp_song_index < SongList::getNumberOfSongs() - 1) {
        _temp_song_index = _temp_song_index + DOWN;
        showSongSelectorPanel(_temp_song_index, DOWN);
      }
      if (action == _cancel) {
        settings_mode = false;
      }
      if (action == _select) {
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
    leds[leds_index[i]]->off();
  }
}

void SongSelector::showSongSelectorPanel(uint8_t song_index, int direction)
{
  uint8_t first_song = 0;
  uint8_t selected_song = 1;
  uint8_t max_number_of_visible_songs = 6;
  uint8_t number_of_songs = SongList::getNumberOfSongs();
  uint8_t number_of_visible_songs = (number_of_songs < max_number_of_visible_songs) ? number_of_songs : max_number_of_visible_songs;
  const char* visible_songs[number_of_visible_songs];

  if (number_of_songs < max_number_of_visible_songs) {
    first_song = 0;
    selected_song = song_index;
  } else {
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
      uint8_t last_song = number_of_songs - 1;
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
  }
  getRangeSongs(first_song, number_of_visible_songs, visible_songs);
  screen->writeSongList(visible_songs, selected_song, number_of_visible_songs);
}

void SongSelector::getRangeSongs(uint8_t first_song, uint8_t number_of_visible_songs, const char** visible_songs)
{
  for (int i = 0; i < number_of_visible_songs; i++) {
    visible_songs[i] = _song_list[first_song + i].c_str();
  }
}
