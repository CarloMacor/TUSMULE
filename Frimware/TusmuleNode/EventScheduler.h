#ifndef EVENT_SCHEDULER_H
#define EVENT_SCHEDULER_H

#include <Arduino.h>

class EventScheduler
{
public:

    bool begin();

    void setNextSample(uint32_t unixTime);

    void setNextFlight(uint32_t unixTime);

    uint32_t nextEvent() const;

    bool isDroneWindow() const;

private:

    uint32_t nextSample = 0;

    uint32_t nextFlight = 0;

};

extern EventScheduler Events;

#endif