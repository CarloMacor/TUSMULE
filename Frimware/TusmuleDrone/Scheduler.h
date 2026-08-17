#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

class Scheduler
{
private:

    uint32_t lastSample;

    uint32_t samplePeriod;

public:

    void begin();

    bool sampleEvent();

};

extern Scheduler NodeScheduler;

#endif