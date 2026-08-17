#ifndef SLEEP_PLANNER_H
#define SLEEP_PLANNER_H

#include <Arduino.h>

class SleepPlanner
{
public:

    bool begin();

    uint32_t nextWakeDelay();

    bool shouldEnableBLE();

private:

};

extern SleepPlanner Sleep;

#endif