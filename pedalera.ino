#ifndef PEDALERA_INO
#define PEDALERA_INO

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

#include "Button.h"
#include "Led.h"
#include "Screen.h"
#include "Settings.h"
#include "SongSelector.h"
#include "Tuner.h"
#include "songs.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128

//const uint8_t   OLED_pin_scl_sck        = 13;
//const uint8_t   OLED_pin_sda_mosi       = 11;
const uint8_t   OLED_pin_cs_ss          = 10;
const uint8_t   OLED_pin_res_rst        = 8;
const uint8_t   OLED_pin_dc_rs          = 9;

const uint8_t button_pins[]            = {27,30,31,32,16,17, 3, 2,  38,34,35,39,40,23,22,21,20,19};
const uint8_t button_ccs[]             = {14,15,20,21,22,23,24,25,  26,27,28,29,30,31,85,86,87,88};
const uint8_t momentary_ccs[]          = { 0, 0,89, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const uint8_t button_push_actions[]    = { 0, 0, 0, 0, 3, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
const uint8_t button_release_actions[] = { 0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0, 1, 2};
const uint8_t SETTINGS_ACTION = 1;
const uint8_t SONG_SELECTOR_ACTION = 2;
const uint8_t TUNER_ACTION = 3;
const uint8_t NUMBER_OF_BUTTONS = sizeof(button_pins) / sizeof(button_pins[0]);

const uint8_t settings_buttons[]       = { 0, 0, 0, 0, 7, 0, 0, 0,   0, 0, 5, 6, 3, 0, 0, 4, 1, 2};

const uint8_t led_pins[]    = { 7, 4, 5, 6,24,25,28,29,  33,36,37,14,18,15};
const uint8_t led_ccs[]     = {14,15,20,21,22,23,24,25,  26,27,28,29,85,86};
const uint8_t NUMBER_OF_LEDS = sizeof(led_pins) / sizeof(led_pins[0]);

Button *buttons[NUMBER_OF_BUTTONS];
Led *leds[NUMBER_OF_LEDS];
const int LED_FLASHING_ON  = 500;
const int LED_FLASHING_OFF = 500;
const int LED_FLASHIN_TIMES = 1;

uint8_t action;

uint8_t  current_song_index = 0;
char current_song[21];
char current_part[21];

const uint8_t leds_midi_channel = 2;
const uint8_t button_mode_midi_channel = 2;
const uint8_t tuner_midi_channel = 3;

Adafruit_SSD1351 adafruit = Adafruit_SSD1351(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &SPI,
  OLED_pin_cs_ss,
  OLED_pin_dc_rs,
  OLED_pin_res_rst
);
Screen screen(&adafruit);

void setup()
{
  Serial.begin(9600);
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    buttons[i] = new Button(button_pins[i], button_ccs[i], button_push_actions[i], button_release_actions[i], settings_buttons[i]);
  }
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i] = new Led(led_pins[i], led_ccs[i]);
  }

  usbMIDI.setHandleControlChange(receivedMidiMessage);
  usbMIDI.setHandleSystemExclusive(receivedSysEx);

  screen.begin();
  screen.clean();
  start();
}

void start()
{
  strcpy(current_song, "");
  strcpy(current_part, "Starting...");
  screen.writeSong(current_song, current_part);
  flash_leds(LED_FLASHIN_TIMES);
  strcpy(current_part, "READY");
  screen.writeSong(current_song, current_part);
}

void loop()
{
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    action = buttons[i]->changed();
    if (action > 0) {
      if (action == SETTINGS_ACTION) {
        settingsMode();
      }
      if (action == SONG_SELECTOR_ACTION) {
        songSelectorMode();
      }
      if (action == TUNER_ACTION) {
        tunerMode();
      }
    }
  }
  usbMIDI.read();
}

void flash_leds(int times)
{
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    leds[i]->flash(LED_FLASHING_ON, LED_FLASHING_OFF, times);
  }
  leds_flash_update();
}

void leds_flash_update()
{
  bool leds_flashing[] = {true,true,true,true,true,true,true,true,true,true,true,true,true,true};
  while (any_led_flashing(leds_flashing)) {
    for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
      leds_flashing[i] = leds[i]->flashUpdateTimes();
    }
  }
}

bool any_led_flashing(bool leds_flashing[])
{
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    if (leds_flashing[i]) {
      return true;
    }
  }
  return false;
}

void receivedMidiMessage(uint8_t channel, uint8_t control, uint8_t value)
{
  check_leds(channel, control, value);
  check_button_modes(channel, control, value);
}

void check_leds(uint8_t channel, uint8_t control, uint8_t value)
{
  if (channel != leds_midi_channel) {
    return;
  }
  int led_index = getLedIndexByCc(control);
  if (led_index > -1) {
    if (value == 127) {
      leds[led_index]->on();
    } else {
      leds[led_index]->off();
    }
  }
}

void check_button_modes(uint8_t channel, uint8_t control, uint8_t value)
{
  if (channel != button_mode_midi_channel) {
    return;
  }
  int button_index = getButtonIndexByMomentaryCc(control);
  if (button_index > -1) {
    bool momentary_state = (value == 127) ? true : false;
    buttons[button_index]->changeMomentary(momentary_state);
  }
}

int getLedIndexByCc(uint8_t cc)
{
  for (uint8_t i = 0; i < NUMBER_OF_LEDS; i++) {
    if (cc == led_ccs[i]) {
      return i;
    }
  }
  return -1;
}

int getButtonIndexByMomentaryCc(uint8_t cc)
{
  for (uint8_t i = 0; i < NUMBER_OF_BUTTONS; i++) {
    if (cc == momentary_ccs[i]) {
      return i;
    }
  }
  return -1;
}

void receivedSysEx(uint8_t *data, unsigned int length)
{
  char *message = getMessage(data, length);
  replaceTildeVowels(message);
  char type[6];
  getMessageType(message, type);
  if (strcmp(type, "song") == 0) {
    current_song_index = getMessageSongIndex(message);
    getCurrentSongName(current_song_index);
    getMessagePart(message);
    screen.writeSong(current_song, current_part);
  }
  if (strcmp(type, "chord") == 0) {
    char *chord = getMessageChord(message);
    if (strcmp(chord, "0") != 0) {
      screen.writeChord(chord);
    } else {
      screen.removeChord();
    }
    delete[] chord;
  }
  free(message);
}

void getCurrentSongName(int song_index)
{
  strcpy(current_song, SONGS[song_index]);
}

char * getMessage(uint8_t *data, unsigned int length)
{
  uint8_t message[length - 2];
  for (unsigned int i = 1; i < (length - 1); i++) {
    message[i - 1] = data[i];
  }
  char* charMessage = (char*)malloc(sizeof(char) * (length-1));
  memcpy(charMessage, message, length-2);
  charMessage[length-2] = '\0';
  return charMessage;
}

void replaceTildeVowels(char *message) {
  int length = strlen(message);
  for (int i = 0; i < length; i++) {
    if (message[i] == '&') {
      switch (message[i+1]) {
        case 'a': message[i] = char(160); break;
        case 'e': message[i] = char(130); break;
        case 'i': message[i] = char(161); break;
        case 'o': message[i] = char(162); break;
        case 'u': message[i] = char(163); break;
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

void getMessageType(char* message, char* type)
{
  char* delimiter = strchr(message, ':');
  if (delimiter != NULL) {
    int type_len = delimiter - message;
    memcpy(type, message, type_len);
    type[type_len] = '\0';
  }
}

uint8_t getMessageSongIndex(char* message)
{
  char* song_number = NULL;
  char* delimiter1 = strchr(message, ':');
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

void getMessagePart(char* message)
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

char* getMessageChord(char* message)
{
  char* chord = NULL;
  char* delimiter = strchr(message, ':');
  if (delimiter != NULL) {
    int chord_len = strlen(delimiter + 1);
    chord = new char[chord_len + 1];
    memcpy(chord, delimiter + 1, chord_len);
    chord[chord_len] = '\0';
  }
  return chord;
}

void settingsMode()
{
  Settings settings(&screen, buttons, NUMBER_OF_BUTTONS, leds, NUMBER_OF_LEDS);
  settings.startSettingsMode();
  settings.settingsMode();
  settings.exitSettingsMode();
  exitSettingsMode();
}

void exitSettingsMode()
{
  screen.clean();
  screen.writeSong(current_song, current_part);
}

void songSelectorMode()
{
  SongSelector songSelector(&screen, buttons, NUMBER_OF_BUTTONS, leds, NUMBER_OF_LEDS, SONGS, NUMBER_OF_SONGS);
  songSelector.startSongSelectorMode(current_song_index);
  songSelector.songSelectorMode();
  songSelector.exitSongSelectorMode();
  exitSongSelectorMode();
}

void exitSongSelectorMode()
{
  screen.clean();
  screen.writeSong(current_song, current_part);
}

void tunerMode()
{
  Tuner tuner(&screen, buttons, NUMBER_OF_BUTTONS, leds, NUMBER_OF_LEDS, tuner_midi_channel);
  tuner.startTunerMode();
  tuner.tunerMode();
  tuner.exitTunerMode();
  exitTunerMode();
}

void exitTunerMode()
{
  screen.clean();
  screen.writeSong(current_song, current_part);
}

#endif
