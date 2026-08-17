/******************************************************************************
 *
 *  TUSMULE Embedded Platform
 *
 *  Module:
 *      Mission
 *
 ******************************************************************************/

#include "Mission.h"

Mission NodeMission;

//----------------------------------------------------------
// Initialization
//----------------------------------------------------------

void Mission::begin()
{
    clear();
}

//----------------------------------------------------------
// Clear Mission
//----------------------------------------------------------

void Mission::clear()
{
    nextFlightTime = 0;
    lastConnectionTime = 0;
    lastDownloadTime = 0;
}

//----------------------------------------------------------
// Next Flight
//----------------------------------------------------------

void Mission::setNextFlight(uint32_t unixTime)
{
    nextFlightTime = unixTime;
}

uint32_t Mission::nextFlight() const
{
    return nextFlightTime;
}

//----------------------------------------------------------
// Last Connection
//----------------------------------------------------------

void Mission::setLastConnection(uint32_t unixTime)
{
    lastConnectionTime = unixTime;
}

uint32_t Mission::lastConnection() const
{
    return lastConnectionTime;
}

//----------------------------------------------------------
// Last Download
//----------------------------------------------------------

void Mission::setLastDownload(uint32_t unixTime)
{
    lastDownloadTime = unixTime;
}

uint32_t Mission::lastDownload() const
{
    return lastDownloadTime;
}

//----------------------------------------------------------
// Mission Validity
//----------------------------------------------------------

bool Mission::valid() const
{
    return (nextFlightTime != 0);
}