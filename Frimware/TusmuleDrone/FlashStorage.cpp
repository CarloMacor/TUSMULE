#include "FlashStorage.h"

FlashStorage Storage;

FlashStorage::FlashStorage()
{
    sampleCount = 0;
}

bool FlashStorage::begin()
{
    sampleCount = 0;

    return true;
}

bool FlashStorage::append(const Sample &sample)
{
    if(sampleCount >= STORAGE_MAX_SAMPLES)
        return false;

    samples[sampleCount] = sample;

    sampleCount++;

    return true;
}

bool FlashStorage::read(uint32_t index,
                        Sample &sample)
{
    if(index >= sampleCount)
        return false;

    sample = samples[index];

    return true;
}

bool FlashStorage::clear()
{
    sampleCount = 0;

    return true;
}

uint32_t FlashStorage::count()
{
    return sampleCount;
}

bool FlashStorage::isFull()
{
    return sampleCount >= STORAGE_MAX_SAMPLES;
}