#include "menu.h"

const char* MENU[] = {
  "Led Intensity",
  "Audio Out"
};

const char* AUDIO_SUBMENU[] = {
  "Bass Left",
  "Bass Right",
  "MIDI Left",
  "MIDI Rigth"
};

const char** SUBMENUS[] = {
  0,
  AUDIO_SUBMENU,
};
