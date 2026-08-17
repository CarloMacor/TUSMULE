#include "CRC16.h"

uint16_t CRC16::calculate(const void* data,
                          uint16_t length)
{
    const uint8_t* p = (const uint8_t*)data;

    uint16_t crc = 0xFFFF;

    while(length--)
    {
        crc ^= *p++;

        for(uint8_t i = 0; i < 8; i++)
        {
            if(crc & 1)
                crc = (crc >> 1) ^ 0xA001;
            else
                crc >>= 1;
        }
    }

    return crc;
}