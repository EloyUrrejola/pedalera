#include "Clock.h"

Clock::Clock(Screen *screen, Button **buttons, const uint8_t number_of_buttons)
    : _number_of_buttons(number_of_buttons)
{
  _screen = screen;
  _buttons = buttons;
}

void Clock::startClockMode()
{
  _screen->clean();
  _screen->showClockBackground();
}

void Clock::setDatetime(int datetime)
{
  time_t current_time = static_cast<time_t>(datetime);
  setSpainDatetime(current_time);
}

void Clock::setSpainDatetime(time_t current_time)
{
  TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};
  TimeChangeRule CET = {"CET", Last, Sun, Oct, 3, 60};
  Timezone europeMadrid(CEST, CET);

  time_t local_time = europeMadrid.toLocal(current_time);
  setTime(local_time);
}

void Clock::clockMode(uint8_t wait_seconds)
{
  bool clock_mode = true;
  unsigned long start_time = millis();
  showClock();
  while (clock_mode) {
    if (millis() - start_time >= wait_seconds * 1000) {
      clock_mode = false;
    }
  }
}

void Clock::showClock()
{
  time_t current_time = now();
  int last_minutes = minutes;
  minutes = minute(current_time);
  if (minutes != last_minutes) {
    _screen->showClock(hour(current_time), minute(current_time), second(current_time), day(current_time), month(current_time), year(current_time));
  }
}