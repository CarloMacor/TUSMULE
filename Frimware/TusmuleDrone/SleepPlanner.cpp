#include "SleepPlanner.h"

#include "RTCManager.h"
#include "ProtocolSession.h"
#include "Config.h"

SleepPlanner Sleep;

bool SleepPlanner::begin()
{
    return true;
}

uint32_t SleepPlanner::nextWakeDelay()
{
    uint32_t now = RTC.now();

    uint32_t nextFlight = Session.nextFlight();

    if(nextFlight == 0)
    {
        return SAMPLE_PERIOD_SEC;
    }

    if(nextFlight <= now)
    {
        return SAMPLE_PERIOD_SEC;
    }

    uint32_t delta = nextFlight - now;

    if(delta > SAMPLE_PERIOD_SEC)
        return SAMPLE_PERIOD_SEC;

    return delta;
}

bool SleepPlanner::shouldEnableBLE()
{
    uint32_t nextFlight = Session.nextFlight();

    if(nextFlight == 0)
        return false;

    uint32_t now = RTC.now();

    if(now >= nextFlight - DRONE_WINDOW_SEC)
        return true;

    return false;
}