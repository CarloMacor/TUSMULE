#ifndef SAMPLE_SERIALIZER_H
#define SAMPLE_SERIALIZER_H

#include <Arduino.h>
#include "Sample.h"

class SampleSerializer
{
public:

    static String toText(const Sample& sample);

    static uint16_t toBinary(const Sample& sample,
                             uint8_t* buffer);

};

#endif