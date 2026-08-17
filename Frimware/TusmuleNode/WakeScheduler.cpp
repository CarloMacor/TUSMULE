#include "WakeScheduler.h"

#include "Mission.h"
#include "RTCManager.h"
#include "Config.h"


WakeScheduler WakeMgr;

void WakeScheduler::begin()
{
}

uint32_t WakeScheduler::nextWakeDelay()
{
    uint32_t now = RTC.now();

//    uint32_t sampleDelay = NodeConfig.samplePeriod();

uint32_t sampleDelay = SAMPLE_PERIOD_SEC;

    if(!NodeMission.valid())
        return sampleDelay;

    uint32_t flight = NodeMission.nextFlight();

    if(flight <= now)
        return SAMPLE_PERIOD_SEC;

    uint32_t flightDelay = flight - now;

    if(flightDelay < sampleDelay)
        return flightDelay;

    return sampleDelay;
}

bool WakeScheduler::shouldEnableBLE()
{
    if(!NodeMission.valid())
        return false;

    uint32_t now = RTC.now();

    uint32_t next = NodeMission.nextFlight();

    return (next > now) && ((next - now) < 300);
}