#include "Storage.h"

Storage Samples;

Storage::Storage()
{
}

void Storage::begin()
{
    head = 0;
    tail = 0;
    items = 0;
    nextId = 1;
}

bool Storage::append(Sample &sample)
{
    sample.id = nextId++;

    buffer[head] = sample;

    head++;

    if(head >= STORAGE_SIZE)
        head = 0;

    if(items < STORAGE_SIZE)
    {
        items++;
    }
    else
    {
        tail++;

        if(tail >= STORAGE_SIZE)
            tail = 0;
    }

    return true;
}

bool Storage::read(uint16_t index, Sample &sample)
{
    if(index >= items)
        return false;

    uint16_t pos = tail + index;

    if(pos >= STORAGE_SIZE)
        pos -= STORAGE_SIZE;

    sample = buffer[pos];

    return true;
}

void Storage::clear()
{
    head = 0;
    tail = 0;
    items = 0;
    nextId = 1;
}

uint16_t Storage::count()
{
    return items;
}

bool Storage::isFull()
{
    return items == STORAGE_SIZE;
}

uint32_t Storage::firstId()
{
    if(items == 0)
        return 0;

    return buffer[tail].id;
}

uint32_t Storage::lastId()
{
    if(items == 0)
        return 0;

    uint16_t pos = head;

    if(pos == 0)
        pos = STORAGE_SIZE;

    pos--;

    return buffer[pos].id;
}

bool Storage::empty() const
{
    return items == 0;
}
