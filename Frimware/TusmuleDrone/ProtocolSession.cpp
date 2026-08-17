#include "ProtocolSession.h"

ProtocolSession Session;

ProtocolSession::ProtocolSession()
{
    reset();
}

void ProtocolSession::begin()
{
    reset();
}

void ProtocolSession::reset()
{
    active = false;

    currentSample = 0;

    transferredSamples = 0;

    startTime = 0;

    lastActivity = 0;

    nextFlightTime = 0;
}

void ProtocolSession::start()
{
    active = true;

    currentSample = 0;

    transferredSamples = 0;

    startTime = millis();

    lastActivity = millis();
}

void ProtocolSession::stop()
{
    reset();
}

bool ProtocolSession::isActive() const
{
    return active;
}

uint16_t ProtocolSession::current() const
{
    return currentSample;
}

void ProtocolSession::next()
{
    currentSample++;

    transferredSamples++;

    lastActivity = millis();
}

uint16_t ProtocolSession::transferred() const
{
    return transferredSamples;
}

void ProtocolSession::updateActivity()
{
    lastActivity = millis();
}


void ProtocolSession::beginDownload()
{
    downloading = true;

    currentSample = 0;

    transferredSamples = 0;
}

void ProtocolSession::endDownload()
{
    downloading = false;
}

bool ProtocolSession::isDownloading() const
{
    return downloading;
}

void ProtocolSession::setNextFlight(uint32_t t)
{
    nextFlightTime = t;
}

uint32_t ProtocolSession::nextFlight() const
{
    return nextFlightTime;
}