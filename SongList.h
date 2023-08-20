#ifndef SONG_LIST_H
#define SONG_LIST_H

#include <Arduino.h>

class SongList
{
  public:
    static void addSongs(char* songs[], uint8_t number_of_songs);
    static char** getSongList();
    static uint8_t getMaximumNumberOfSongs();
    static char* getCurrentSong();
    static char* getCurrentPart();
    static void setCurrentSongIndex(uint8_t song_index);
    static uint8_t getCurrentSongIndex();
    static void setCurrentPart(char* part);
    static uint8_t getNumberOfSongs();
    
  private:
    static const int MAX_SONGS = 30;
    static char* song_list[MAX_SONGS];
    static uint8_t number_of_songs;
    static uint8_t current_song_index;
    static char* current_part;
};

#endif