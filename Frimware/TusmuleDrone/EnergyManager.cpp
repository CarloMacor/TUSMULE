#include "EnergyManager.h"

EnergyManager Energy;


EnergyManager::EnergyManager()
{
    state = NodePowerState::Boot;
    droneWindow = false;
}


void EnergyManager::begin()
{
    state = NodePowerState::Awake;

    Serial.println("Energy Manager Ready");
}

void EnergyManager::update()
{
    switch(state)
    {
        case NodePowerState::Boot:
            break;

        case NodePowerState::Awake:
            break;

        case NodePowerState::Sampling:
            break;

        case NodePowerState::Idle:
            break;

        case NodePowerState::DroneWindow:
            break;

        case NodePowerState::Sleep:
            break;
    }
}



void EnergyManager::enterSampling()
{
    state = NodePowerState::Sampling;
}

void EnergyManager::enterDroneWindow()
{
    state = NodePowerState::DroneWindow;
    droneWindow = true;
}

void EnergyManager::enterSleep()
{
    state = NodePowerState::Sleep;
    droneWindow = false;
}

void EnergyManager::wakeUp()
{
    state = NodePowerState::Awake;
}

NodePowerState EnergyManager::currentState() const
{
    return state;
}

bool EnergyManager::isDroneWindow() const
{
    return droneWindow;
}

