#include <Arduino.h>
#include "Scheduler.h"
#include "Config.h"
#include "NodeConfiguration.h"

Scheduler NodeScheduler;



void Scheduler::begin()
{
    uint32_t now = millis();
    lastSample = now;

}



bool Scheduler::sampleEvent()
{
    uint32_t elapsed = millis() - lastSample;

    if (elapsed >= samplePeriod * 1000UL)
    {
        uint32_t now = millis();
        lastSample = now;

        return true;
    }

    return false;
}

