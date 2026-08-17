#ifndef ENERGY_MANAGER_H
#define ENERGY_MANAGER_H

#include <Arduino.h>

enum class NodePowerState
{
    Boot,
    Awake,
    Sampling,
    Idle,
    DroneWindow,
    Sleep
};


class EnergyManager
{
private:

    NodePowerState state;

    bool droneWindow;

public:

    EnergyManager();

    void begin();

    void update();

    void enterSampling();

    void enterDroneWindow();

    void enterSleep();

    void wakeUp();

    NodePowerState currentState() const;

    bool isDroneWindow() const;

};

extern EnergyManager Energy;

#endif