#include "Sensor.h"
#include "Config.h"

Sensor SoilSensor;

bool Sensor::begin()
{
    pinMode(PIN_SOIL, INPUT);

    analogReadResolution(12);      // ADC 0..4095

    return true;
}

uint16_t Sensor::readRaw()
{
    return analogRead(PIN_SOIL);
}

uint8_t Sensor::readPercent()
{
    uint16_t raw = readRaw();

    int percent = map(raw,
                      3500,
                      1000,
                      0,
                      100);

    percent = constrain(percent,0,100);

    return (uint8_t)percent;
}