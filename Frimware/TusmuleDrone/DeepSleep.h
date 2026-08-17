#ifndef DEEP_SLEEP_H
#define DEEP_SLEEP_H

#include <Arduino.h>

class DeepSleepManager
{
public:

    void sleep(uint32_t seconds);

};

extern DeepSleepManager DeepSleep;

#endif