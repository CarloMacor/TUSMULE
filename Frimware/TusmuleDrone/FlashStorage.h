#ifndef FLASH_STORAGE_H
#define FLASH_STORAGE_H

#include <Arduino.h>
#include "Sample.h"
#include "Config.h"

class FlashStorage
{

private:

    Sample samples[STORAGE_MAX_SAMPLES];


    uint32_t sampleCount;

public:

    FlashStorage();

    bool begin();

    bool append(const Sample &sample);

    bool read(uint32_t index,
              Sample &sample);

    bool clear();

    uint32_t count();

    bool isFull();

};

extern FlashStorage Storage;

#endif