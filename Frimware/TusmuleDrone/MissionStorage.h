#ifndef MISSION_STORAGE_H
#define MISSION_STORAGE_H

#include <Arduino.h>

class MissionStorage
{
public:

    bool begin();

    bool saveNextFlight(uint32_t value);

    bool loadNextFlight(uint32_t &value);

};

extern MissionStorage MissionStore;

#endif