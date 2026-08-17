#ifndef MISSION_H
#define MISSION_H

#include <Arduino.h>

class Mission
{
public:

    void begin();

    void clear();

    void setNextFlight(uint32_t unixTime);
    uint32_t nextFlight() const;

    void setLastConnection(uint32_t unixTime);
    uint32_t lastConnection() const;

    void setLastDownload(uint32_t unixTime);
    uint32_t lastDownload() const;

    bool valid() const;

private:

    uint32_t nextFlightTime = 0;

    uint32_t lastConnectionTime = 0;

    uint32_t lastDownloadTime = 0;
};

extern Mission NodeMission;

#endif