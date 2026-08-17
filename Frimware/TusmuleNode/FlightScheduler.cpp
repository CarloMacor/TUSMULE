#include "FlightScheduler.h"
#include "ProtocolSession.h"
#include "RTCManager.h"

FlightScheduler Flight;

void FlightScheduler::begin()
{
}

bool FlightScheduler::isDroneWindow() const
{
    uint32_t now = RTC.now();

    uint32_t next = Session.nextFlight();

    if(next == 0)
        return false;

    const uint32_t WINDOW = 120;      // ±2 minuti

    if(now >= next - WINDOW &&
       now <= next + WINDOW)
        return true;

    return false;
}

uint32_t FlightScheduler::secondsToFlight() const
{
    uint32_t now = RTC.now();

    uint32_t next = Session.nextFlight();

    if(next <= now)
        return 0;

    return next - now;
}