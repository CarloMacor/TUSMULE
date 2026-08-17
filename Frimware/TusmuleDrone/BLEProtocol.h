#ifndef BLE_PROTOCOL_H
#define BLE_PROTOCOL_H

#include <Arduino.h>

class BLEProtocol
{
public:

    bool begin();

    void update();

    bool busy() const;

    bool finished() const;

    void start();

private:

enum class Step
{
    Idle,

    SendHello,
    WaitHello,

    SendInfo,
    WaitInfo,

    SendCount,
    WaitCount,

    SendBegin,
    WaitBegin,

    SendNext,
    WaitNext,

    SendEnd,
    WaitEnd,

    SendTime,
    WaitTime,

    SendNextFlight,
    WaitNextFlight,

    SendBye,
    WaitBye,

    Completed,
    Error
};

    Step state = Step::Idle;

    uint32_t timeout = 0;

    uint16_t sampleCount = 0;

    uint16_t currentSample = 0;

    void nextState(Step s);

    bool expired(uint32_t ms = 3000);

};

extern BLEProtocol Protocol;

#endif