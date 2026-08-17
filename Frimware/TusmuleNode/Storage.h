#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include "Sample.h"

#define STORAGE_SIZE 512

class Storage
{
private:

    Sample buffer[STORAGE_SIZE];

    uint16_t head;

    uint16_t tail;

    uint16_t items;

    uint32_t nextId;

public:

    Storage();

    void begin();

    bool append(Sample &sample);

    bool read(uint16_t index, Sample &sample);

    void clear();

    uint16_t count();

    bool empty() const;

    bool isFull();


    uint32_t firstId();

    uint32_t lastId();
};

extern Storage Samples;

#endif