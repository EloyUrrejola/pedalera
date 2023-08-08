#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include <TimeLib.h>
#include <Timezone.h>
#include <ctime>
#include "Screen.h"
#include "Button.h"

class Clock
{
  public:
    Clock(Screen *screen, Button **buttons, const uint8_t number_of_buttons);
    static void setDatetime(int datetime);
    void startClockMode();
    void clockMode(uint8_t wait_seconds);
  private:
    Screen *_screen;
    Button **_buttons;
    const uint8_t _number_of_buttons;
    const uint16_t RELEASE_TIME = 1000;

    const uint8_t _exit = 8;
    int minutes = -1;

    static void setSpainDatetime(time_t current_time);
    void showClock();
};

#endif