#ifndef DRONE_STATE_MACHINE_H
#define DRONE_STATE_MACHINE_H

#include <Arduino.h>

enum class DroneState
{
    Boot,
    Initialization,
    Idle,
    Scan,
    Connect,
    BLESession,
    Disconnect
};

class DroneStateMachine
{
public:

    bool begin();

    void start();

    void update();

    void setState(DroneState s);

    DroneState state() const;

 

private:

    void enterState(DroneState s);

    void updateBoot();

    void updateInitialization();

    void updateIdle();

    void updateScan();

    void updateConnect();

    void updateBLESession();

    void updateDisconnect();

    DroneState current = DroneState::Boot;

    uint32_t stateEnterTime = 0;
};

extern DroneStateMachine DroneFSM;

#endif