#ifndef WAKE_REASON_H
#define WAKE_REASON_H

#include <Arduino.h>

class WakeReason
{
public:

    void begin();

    bool timerWakeup() const;

    bool powerOn() const;

private:

    esp_sleep_wakeup_cause_t cause;
};

extern WakeReason Wake;

#endif