#include "RTCManager.h"

RTCManager RTC;

RTCManager::RTCManager()
{
    unixTime = 0;
    lastTick = 0;
}

void RTCManager::begin()
{
    unixTime = 0;
    lastTick = millis();
}

void RTCManager::update()
{
    while ((millis() - lastTick) >= 1000)
    {
        unixTime++;
        lastTick += 1000;
    }
}

uint32_t RTCManager::now()
{
    return unixTime;
}

void RTCManager::setTime(uint32_t t)
{
    unixTime = t;
    lastTick = millis();
}