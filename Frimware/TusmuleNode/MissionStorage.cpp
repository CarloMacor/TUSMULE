#include "MissionStorage.h"

#include <Preferences.h>

Preferences prefs;

MissionStorage MissionStore;

bool MissionStorage::begin()
{
    return prefs.begin("mission", false);
}

bool MissionStorage::saveNextFlight(uint32_t value)
{
    prefs.putULong("next", value);

    return true;
}

bool MissionStorage::loadNextFlight(uint32_t &value)
{
    value = prefs.getULong("next", 0);

    return value != 0;
}