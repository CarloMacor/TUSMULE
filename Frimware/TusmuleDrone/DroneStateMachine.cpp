#include "DroneStateMachine.h"

#include "BLEManager.h"
#include "BLEProtocol.h"

DroneStateMachine DroneFSM;

bool DroneStateMachine::begin()
{
    current = DroneState::Boot;
    return true;
}

void DroneStateMachine::start()
{
    Serial.println();
    Serial.println("Drone FSM Start");

    setState(DroneState::Initialization);
}

DroneState DroneStateMachine::state() const
{
    return current;
}

void DroneStateMachine::setState(DroneState s)
{
    if(current == s)
        return;

    Serial.print("Transition ");
    Serial.print((int)current);
    Serial.print(" -> ");
    Serial.println((int)s);

    current = s;

    enterState(current);
}

void DroneStateMachine::enterState(DroneState s)
{
    stateEnterTime = millis();

    Serial.print("STATE -> ");

    switch(s)
    {
        case DroneState::Boot:
            Serial.println("BOOT");
            break;

        case DroneState::Initialization:
            Serial.println("INITIALIZATION");
            break;

        case DroneState::Idle:
            Serial.println("IDLE");
            break;

     
        case DroneState::Scan:
            BLE.startScan();
        break;

        case DroneState::Connect:
            Serial.println("CONNECT");
            break;

        case DroneState::BLESession:
            Serial.println("BLE SESSION");
            break;

        case DroneState::Disconnect:
            Serial.println("DISCONNECT");
            break;
    default:
        break;
    
    }
}

void DroneStateMachine::update()
{
    switch(current)
    {
        case DroneState::Boot:
            setState(DroneState::Initialization);
            break;

        case DroneState::Initialization:
            setState(DroneState::Idle);
            break;

        case DroneState::Idle:
            updateIdle();
            break;

        case DroneState::Scan:
            updateScan();
            break;

        case DroneState::Connect:
            updateConnect();
            break;

        case DroneState::BLESession:
            updateBLESession();
            break;

        case DroneState::Disconnect:
            updateDisconnect();
            break;
    }
}

void DroneStateMachine::updateIdle()
{
    if(millis() - stateEnterTime < 1000)
        return;

    Serial.println("Start Scan");

    setState(DroneState::Scan);
}

void DroneStateMachine::updateScan()
{
    static bool started = false;

    if(!started)
    {
        BLE.startScan();

        started = true;

        return;
    }

    BLE.update();

    if(BLE.nodeFound())
    {
        Serial.println("Node Found");

        started = false;

        BLE.stopScan();

        setState(DroneState::Connect);

        return;
    }

    if(!BLE.scanning())
    {
        started = false;
    }
}


void DroneStateMachine::updateConnect()
{
    if(BLE.connected())
    {
        Protocol.start();

        setState(DroneState::BLESession);

        return;
    }

    if(BLE.connect())
    {
        Protocol.start();

        setState(DroneState::BLESession);

        return;
    }

    if(millis() - stateEnterTime > 5000)
    {
        Serial.println("Connection Timeout");

        BLE.disconnect();

        setState(DroneState::Scan);
    }
}

void DroneStateMachine::updateBLESession()
{
    if(!BLE.connected())
    {
        Serial.println("Connection Lost");

        setState(DroneState::Scan);

        return;
    }

    Protocol.update();

    if(Protocol.finished())
    {
        Serial.println("Protocol Completed");

        setState(DroneState::Disconnect);

        return;
    }
}



void DroneStateMachine::updateDisconnect()
{
    BLE.disconnect();

    delay(100);

    setState(DroneState::Scan);
}

