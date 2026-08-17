#ifndef PROTOCOL_SESSION_H
#define PROTOCOL_SESSION_H

#include <Arduino.h>

class ProtocolSession
{
private:

    bool active;

    uint16_t currentSample;

    uint16_t transferredSamples;

    uint32_t startTime;

    uint32_t lastActivity;

public:

    ProtocolSession();

    void begin();

    void reset();

    void start();

    void stop();

    bool isActive() const;

    uint16_t current() const;

    void next();

    uint16_t transferred() const;

    void updateActivity();

bool downloading;

void beginDownload();

void endDownload();

bool isDownloading() const;

uint32_t nextFlightTime;
void setNextFlight(uint32_t t);

uint32_t nextFlight() const;

};

extern ProtocolSession Session;

#endif