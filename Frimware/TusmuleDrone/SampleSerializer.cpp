#include "SampleSerializer.h"

#include <string.h>

String SampleSerializer::toText(const Sample& s)
{
    String msg;

    msg  = "ID=";
    msg += s.id;

    msg += ";TIME=";
    msg += s.unixTime;

    msg += ";SOIL=";
    msg += s.soil;

    msg += ";BAT=";
    msg += s.battery;

    msg += ";TEMP=";
    msg += s.temperature;

    return msg;
}

uint16_t SampleSerializer::toBinary(const Sample& sample,
                                    uint8_t* buffer)
{
    memcpy(buffer,&sample,sizeof(Sample));

    return sizeof(Sample);
}