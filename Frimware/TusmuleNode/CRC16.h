#ifndef CRC16_H
#define CRC16_H

#include <Arduino.h>

class CRC16
{
public:

    static uint16_t calculate(const void* data,
                              uint16_t length);

};

#endif