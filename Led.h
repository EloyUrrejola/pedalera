#ifndef LED_H
#define LED_H

#include <Arduino.h>
#define Led_h

class Led
{
  public:
    Led(uint8_t, uint8_t);
    void on();
    void off();
    void flash(int time_on, int time_off, int times);
    void flashUpdate();
    bool flashUpdateTimes();
    
  private:
    const uint8_t LED_INTENSITY = 5;
    const bool ON = true;
    const bool OFF = false;

    uint8_t led_pin;
    uint8_t led_cc;
    unsigned int led_time_on;
    unsigned int led_time_off;
    int led_times;
    bool led_state;
    unsigned long led_time_now;
    unsigned long led_time_start;
};

#endif