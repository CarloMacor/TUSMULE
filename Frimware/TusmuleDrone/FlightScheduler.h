#ifndef FLIGHT_SCHEDULER_H
#define FLIGHT_SCHEDULER_H

#include <Arduino.h>

class FlightScheduler
{
public:

    void begin();

    bool isDroneWindow() const;

    uint32_t secondsToFlight() const;

};

extern FlightScheduler Flight;

#endif