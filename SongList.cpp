#include "SongList.h"

char* SongList::song_list[SongList::MAX_SONGS];
uint8_t SongList::current_song_index = 0;
char* SongList::current_part = nullptr;
uint8_t SongList::number_of_songs = 0;

void SongList::addSongs(char* songs[], uint8_t total_songs)
{
  current_song_index = 0;
  number_of_songs = total_songs;
  memcpy(song_list, songs, number_of_songs);
}

char** SongList::getSongList()
{
  return song_list;
}

uint8_t SongList::getMaximumNumberOfSongs()
{
  return MAX_SONGS;
}

char* SongList::getCurrentSong()
{
  return song_list[current_song_index];
}

char* SongList::getCurrentPart()
{
  return current_part;
}

void SongList::setCurrentSongIndex(uint8_t song_index)
{
  current_song_index = song_index;
}

uint8_t SongList::getCurrentSongIndex()
{
  return current_song_index;
}

void SongList::setCurrentPart(char* part)
{
  current_part = part;
}

uint8_t SongList::getNumberOfSongs()
{
  return number_of_songs;
}
