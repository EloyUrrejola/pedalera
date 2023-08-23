#ifndef SYS_EX_MESSAGE_H
#define SYS_EX_MESSAGE_H

#include <Arduino.h>
#include <sstream>
#include <string>

#include "Clock.h"
#include "Screen.h"
#include "SongList.h"

class SysExMessage
{
  public:
    SysExMessage();
    void setScreen(Screen *screen);
    void process(uint8_t *data, unsigned int length);
    
  private:
    Screen *screen;
    const char* TYPE_LIST = "list";
    const char* TYPE_SONG = "song";
    const char* TYPE_CHORD = "chord";
    const char* TYPE_TIME = "time";
    const char TYPE_DELIMITER = ':';
    const char LIST_DELIMITER = '%';
    const char SONG_DELIMITER = '|';

    void getMessage(uint8_t *data, unsigned int length, char *message);
    void replaceTildeVowels(char *message);
    void getMessageType(char* message, char* type);
    void getSongListFromMessage(char *message);
    void getSongAndPartFromMessage(char* message);
    uint8_t getSongIndexFromMessage(char* message);
    void getPartFromMessage(char* message, char* current_part);
    void getChord(char* message);
    void getChordFromMessage(char* message, char* chord);
    void getDatetime(char* message);
    int getDatetimeFromMessage(char* message);

    void hasNullTerminator(const char* str);
};

#endif