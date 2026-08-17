#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <Arduino.h>

enum class NodeState
{
    Boot,
    Initialization,
    Idle,
    Sampling,
    BLEWait,
    BLESession,
    PreSleep,
    DeepSleep
};

class StateMachine
{
public:

    bool begin();

    void start();

    void update();

    void setState(NodeState s);

    void onSampleEvent();

    void onDroneEvent();

    void onBLEConnected();

    void onBLEDisconnected();

    void onDownloadCompleted();

    NodeState state() const;

    void endBLESession();

void startBLEWindow();


    private:

    void enterState(NodeState s);

    void updateBoot();

    void updateInitialization();

    void updateIdle();

    void updateSampling();

    void updateBLEWait();

    void updateBLESession();

    void updatePreSleep();

    void updateDeepSleep();

    NodeState current = NodeState::Boot;

    uint32_t stateEnterTime = 0;

    static constexpr uint32_t BLE_WAIT_TIMEOUT_MS = 30000;

    uint32_t sleepDelay = 0;

};

extern StateMachine FSM;

#endif