#ifndef WAKE_SCHEDULER_H
#define WAKE_SCHEDULER_H

#include <Arduino.h>

class WakeScheduler
{
public:

    void begin();

    uint32_t nextWakeDelay();

    bool shouldEnableBLE();

};

extern WakeScheduler WakeMgr;

#endif