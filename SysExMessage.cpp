#include "SysExMessage.h"

SysExMessage::SysExMessage()
{
}

void SysExMessage::setScreen(Screen *screen)
{
  this->screen = screen;
}

void SysExMessage::process(uint8_t *data, unsigned int length)
{
  char message[length - 1];
  getMessage(data, length, message);
  replaceTildeVowels(message);
  char type[6] = "";
  getMessageType(message, type);

  if (strcmp(type, TYPE_LIST) == 0) {
    getSongListFromMessage(message);
  }
  
  if (strcmp(type, TYPE_SONG) == 0) {
    getSongAndPartFromMessage(message);
  }

  if (strcmp(type, TYPE_CHORD) == 0) {
    getChord(message);
  }

  if (strcmp(type, TYPE_TIME) == 0) {
    getDatetime(message);
  }
}

void SysExMessage::getMessage(uint8_t *data, unsigned int length, char *message)
{
  for (unsigned int i = 0; i < (length - 2); i++) {
    message[i] = data[i + 1];
  }
  message[length - 2] = '\0';
}

void SysExMessage::replaceTildeVowels(char *message)
{
  int length = strlen(message);
  char atilde = 'a';//char(160)
  char etilde = 'e';//char(130)
  char itilde = 'i';//char(161)
  char otilde = 'o';//char(162)
  char utilde = 'u';//char(163)
  for (int i = 0; i < length; i++) {
    if (message[i] == '&') {
      switch (message[i+1]) {
        case 'a': message[i] = atilde; break;
        case 'e': message[i] = etilde; break;
        case 'i': message[i] = itilde; break;
        case 'o': message[i] = otilde; break;
        case 'u': message[i] = utilde; break;
        default: message[i] = '&';
      }
      if (message[i] != '&') {
        for (int j = i+1; j < length-1; j++) {
          message[j] = message[j+1];
        }
        length--;
      }
      message[length] = '\0';
    }
  }
}

void SysExMessage::getMessageType(char* message, char* type)
{
  char* delimiter = strchr(message, TYPE_DELIMITER);
  if (delimiter != NULL) {
    int type_len = delimiter - message;
    memcpy(type, message, type_len);
    type[type_len] = '\0';
  }
}

void SysExMessage::getSongListFromMessage(char *message)
{
  char *delimiter = strchr(message, TYPE_DELIMITER);
  if (delimiter == NULL) {
    return;
  }

  delimiter++;
  std::string song_list_string = delimiter;

  size_t list_delimiter_pos = song_list_string.find(LIST_DELIMITER);
  if (list_delimiter_pos == std::string::npos) {
    return;
  }

  std::string list_name = song_list_string.substr(0, list_delimiter_pos);
  song_list_string.erase(0, list_delimiter_pos + 1);

  const uint8_t max_songs = SongList::getMaximumNumberOfSongs();
  std::string song_list[max_songs];
  uint8_t song_count = 0;

  std::istringstream song_stream(song_list_string);
  std::string song;

  while (std::getline(song_stream, song, SONG_DELIMITER) && song_count < max_songs) {
    song_list[song_count] = song;
    song_count++;
  }

  SongList::addSongs(list_name, song_list);

  if (song_count > 0) {
    char list_name_copy[list_name.length() + 1];
    strcpy(list_name_copy, list_name.c_str());
    screen->writeTempMessage(list_name_copy, (char*)"LOADED");
  }
}

void SysExMessage::getSongAndPartFromMessage(char* message)
{
  if (SongList::getNumberOfSongs() == 0) {
    return;
  }
  uint8_t current_song_index = getSongIndexFromMessage(message);
  SongList::setCurrentSongIndex(current_song_index);
  char current_part[21];
  getPartFromMessage(message, current_part);
  SongList::setCurrentPart(current_part);
  screen->writeSongAndPart();
}

uint8_t SysExMessage::getSongIndexFromMessage(char* message)
{
  char* song_number = NULL;
  char* delimiter1 = strchr(message, TYPE_DELIMITER);
  char* delimiter2 = strchr(message, '-');
  if (delimiter1 != NULL) {
    if (delimiter2 != NULL && delimiter2 > delimiter1) {
      int song_len = delimiter2 - delimiter1 - 1;
      song_number = new char[song_len + 1];
      memcpy(song_number, delimiter1 + 1, song_len);
      song_number[song_len] = '\0';
    } else {
      int song_len = strlen(delimiter1 + 1);
      song_number = new char[song_len + 1];
      memcpy(song_number, delimiter1 + 1, song_len);
      song_number[song_len] = '\0';
    }
    return atoi(song_number);
  }
  return -1;
}

void SysExMessage::getPartFromMessage(char* message, char* current_part)
{
  char* delimiter = strchr(message, '-');
  if (delimiter != NULL) {
    int part_len = strlen(delimiter + 1);
    memcpy(current_part, delimiter + 1, part_len);
    current_part[part_len] = '\0';
  } else {
    current_part[0] = '\0';
  }
}

void SysExMessage::getChord(char* message)
{
  char chord[10];
  getChordFromMessage(message, chord);
  if (strcmp(chord, "0") != 0) {
    screen->writeChord(chord);
  } else {
    screen->removeChord();
  }
}

void SysExMessage::getChordFromMessage(char* message, char* chord)
{
  char* delimiter = strchr(message, TYPE_DELIMITER);
  if (delimiter != NULL) {
    strcpy(chord, delimiter + 1);
  } else {
    strcpy(chord, "");
  }
}

void SysExMessage::getDatetime(char* message)
{
  int current_time = getDatetimeFromMessage(message);
  Clock::setDatetime(current_time);
}

int SysExMessage::getDatetimeFromMessage(char* message)
{
  int datetime = 0;
  char* delimiter = strchr(message, TYPE_DELIMITER);
  if (delimiter != NULL) {
    char* datetime_str = delimiter + 1;
    datetime = atoi(datetime_str);
  }
  return datetime;
}
