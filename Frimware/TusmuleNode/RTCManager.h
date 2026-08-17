#ifndef RTC_MANAGER_H
#define RTC_MANAGER_H

#include <Arduino.h>

class RTCManager
{
private:

    uint32_t unixTime;
    uint32_t lastTick;

public:

    RTCManager();

    void begin();

    void update();

    uint32_t now();

    void setTime(uint32_t t);

};

extern RTCManager RTC;

#endif