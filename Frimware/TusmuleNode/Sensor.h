#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor
{
public:

    bool begin();

    uint16_t readRaw();

    uint8_t readPercent();

};

extern Sensor SoilSensor;

#endif