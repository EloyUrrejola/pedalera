#include "Tuner.h"

Tuner::Tuner(Screen *screen, Button **buttons, const uint8_t number_of_buttons, Led **leds, const uint8_t number_of_leds, const uint8_t midi_channel)
    : _number_of_buttons(number_of_buttons), _number_of_leds(number_of_leds), _midi_channel(midi_channel)
{
  _screen = screen;
  _buttons = buttons;
  _leds = leds;
}

void Tuner::startTunerMode()
{
  _screen->clean();
  //_screen->writeTunerTitle();
  _screen->showTuningBackground();
}

void Tuner::tunerMode()
{
  bool tuner_mode = true;
  uint8_t cc;
  uint8_t value;
  char note[3];
  uint8_t last_tuning = 0;
  while (tuner_mode) {
    for (uint8_t i = 0; i < _number_of_leds_flashing; i++) {
      _leds[_leds_flashing_index[i]]->flashUpdate();
    }
    for (uint8_t i = 0; i < _number_of_buttons; i++) {
      uint8_t action = _buttons[i]->settingsChanged();
      if (action == _exit) {
        _buttons[i]->sendControlChange(_buttons[i]->getButtonCc());
        tuner_mode = false;
      }
    }
    if (usbMIDI.read(_midi_channel)) {
      //showLedTuning(usbMIDI.getData1(), usbMIDI.getData2());
      cc = usbMIDI.getData1();
      value = usbMIDI.getData2();
      if (cc == 20) {
        getNote(value, note);
        _screen->showNote(note);
      }
      if (cc == 21) {
        showTuning(value, last_tuning);
        last_tuning = value;
      }
    }
  }
}

void Tuner::showTuning(uint8_t tuning, uint8_t last_tuning)
{
  _screen->showTuning(tuning, last_tuning);
  //showLedTuning(note, tuning);
}

void Tuner::getNote(uint8_t value, char* note)
{
  strcpy(note, NOTES[value % 12]);
}

void Tuner::showLedTuning(uint8_t tuning)
{
  
}

void Tuner::exitTunerMode()
{
  
}