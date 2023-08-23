#include "SongList.h"

char SongList::list_name[20] = "";
std::string SongList::song_list[SongList::MAX_SONGS];
uint8_t SongList::current_song_index = 0;
char SongList::current_part[MAX_PART_LENGTH] = "";
uint8_t SongList::number_of_songs = 0;

void SongList::addSongs(const std::string& list_name, const std::string songs[])
{
  freeSongs();

  strncpy(SongList::list_name, list_name.c_str(), sizeof(SongList::list_name) - 1);
  SongList::list_name[sizeof(SongList::list_name) - 1] = '\0';

  current_song_index = 0;
  strcpy(current_part, "");

  uint8_t i;
  for (i = 0; songs[i] != ""; i++) {
    song_list[i] = songs[i];
  }
  number_of_songs = i;
}


void SongList::freeSongs()
{
  for (uint8_t i = 0; i < number_of_songs; i++) {
    song_list[i].clear();
  }
  number_of_songs = 0;
}

std::string* SongList::getSongList()
{
  std::string* songs = new std::string[number_of_songs];
  for (uint8_t i = 0; i < number_of_songs; i++) {
    songs[i] = song_list[i];
  }
  return songs;
}

uint8_t SongList::getMaximumNumberOfSongs()
{
  return MAX_SONGS;
}

std::string SongList::getCurrentSong()
{
  return song_list[current_song_index];
}

std::string SongList::getCurrentPart()
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

void SongList::setCurrentPart(const std::string& part)
{
  strncpy(current_part, part.c_str(), sizeof(current_part) - 1);
  current_part[sizeof(current_part) - 1] = '\0';
}

uint8_t SongList::getNumberOfSongs()
{
  return number_of_songs;
}
