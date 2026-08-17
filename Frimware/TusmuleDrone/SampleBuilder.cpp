#include "SampleBuilder.h"

#include "Sensor.h"
#include "Storage.h"
#include "RTCManager.h"
#include "CRC16.h"

SampleBuilder Builder;

void SampleBuilder::begin()
{

}

bool SampleBuilder::acquire()
{
    Sample s;

   s.unixTime = RTC.now();

    s.soil = SoilSensor.readPercent();

    s.battery = 4000;

    s.temperature = 250;

    s.flags = 0;

    s.reserved = 0;


    s.crc = 0;

    s.crc = CRC16::calculate(
            &s,
            sizeof(Sample) - sizeof(uint16_t));

    

    return Samples.append(s);
}