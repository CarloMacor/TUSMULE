#ifndef BLE_PROTOCOL_H
#define BLE_PROTOCOL_H

#include <Arduino.h>
#include "Sample.h"


class BLEProtocol
{
public:

    bool begin();

    void update();


private:

    void cmdPing();

    void cmdInfo();

    void cmdCount();

    void cmdGet(uint16_t index);

    void cmdBegin();

    void cmdNext();

    void cmdEnd();
    
    void cmdHello();

    void cmdBye();

    void cmdNextBinary();

    void cmdNextFlight(uint32_t unixTime);

    void cmdTime(uint32_t unixTime);

    void cmdTimeQuery();

    void cmdNextFlightQuery();

    void cmdMission();
};

extern BLEProtocol Protocol;

#endif