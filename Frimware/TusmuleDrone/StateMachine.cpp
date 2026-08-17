#include "StateMachine.h"
#include "Scheduler.h"
#include "SampleBuilder.h"
#include "Storage.h"
#include "BLEManager.h"
#include "DeepSleep.h"
#include "WakeScheduler.h"
#include "WakeReason.h"

StateMachine FSM;

bool StateMachine::begin()
{
    current = NodeState::Boot;

    return true;
}

void StateMachine::update()
{
    switch(current)
    {
        case NodeState::Boot:
            setState(NodeState::Initialization);
            break;

        case NodeState::Initialization:
            setState(NodeState::Idle);
            break;

        case NodeState::Idle:
            updateIdle();
            break;

        case NodeState::Sampling:
            updateSampling();
            break;

        case NodeState::BLEWait:
            updateBLEWait();
            break;

        case NodeState::BLESession:
            updateBLESession();
            break;

        case NodeState::PreSleep:
            updatePreSleep();
            break;

        case NodeState::DeepSleep:
            updateDeepSleep();
            break;

        default:
            break;
    }
}

void StateMachine::setState(NodeState s)
{
Serial.print("Transition ");

Serial.print((int)current);

Serial.print(" -> ");

Serial.println((int)s);


    if(current == s)
        return;
    current = s;
    enterState(current);
}

NodeState StateMachine::state() const
{
    return current;
}


void StateMachine::updateIdle()
{
    if(NodeScheduler.sampleEvent())
    {
        setState(NodeState::Sampling);
        return;
    }

    if(WakeMgr.shouldEnableBLE())
    {
        setState(NodeState::BLEWait);
        return;
    }
}

void StateMachine::updateSampling()
{
    if(Builder.acquire())
    {
        Serial.println("Sample Stored");
    }

    if(WakeMgr.shouldEnableBLE())
        setState(NodeState::BLEWait);
    else
        setState(NodeState::PreSleep);
}

void StateMachine::enterState(NodeState s)
{

    current = s;

    stateEnterTime = millis();

    Serial.print("STATE -> ");

    switch(s)
    {
        case NodeState::Boot:
            Serial.println("BOOT");
            break;

        case NodeState::Initialization:
            Serial.println("INITIALIZATION");
            break;

        case NodeState::Idle:
            Serial.println("IDLE");
            break;

        case NodeState::Sampling:
            Serial.println("SAMPLING");
            break;

        case NodeState::BLEWait:
            Serial.println("BLE WAIT");
            break;

        case NodeState::BLESession:
            Serial.println("BLE SESSION");
            break;

        case NodeState::PreSleep:
            Serial.println("PRE SLEEP");
            break;

        case NodeState::DeepSleep:
            Serial.println("DEEP SLEEP");
            break;
    }
}



void StateMachine::updateBLEWait()
{
    if(BLE.connected())
    {
        setState(NodeState::BLESession);
        return;
    }

    if(!WakeMgr.shouldEnableBLE())
    {
  //      setState(NodeState::PreSleep);
        return;
    }
}



void StateMachine::updateBLESession()
{
}

void StateMachine::updatePreSleep()
{

    Serial.println();

    Serial.println("===== PRE SLEEP =====");

    Serial.print("Wake Delay = ");
    Serial.println(WakeMgr.nextWakeDelay());

    Serial.println("=====================");

   //setState(NodeState::DeepSleep);
    setState(NodeState::BLEWait);
}

void StateMachine::updateDeepSleep()
{
      Serial.println("Entering Deep Sleep...");
    DeepSleep.sleep( WakeMgr.nextWakeDelay());
}

void StateMachine::onSampleEvent()
{
    if(current == NodeState::Idle)
    {
        setState(NodeState::Sampling);
    }
}

void StateMachine::onDroneEvent()
{
   Serial.print("FSM State = ");
   Serial.println((int)current);

    if(current == NodeState::Idle)
    {
        setState(NodeState::BLEWait);
    }
}

void StateMachine::onBLEConnected()
{
    if(current == NodeState::BLEWait)
    {
        setState(NodeState::BLESession);
    }
}

void StateMachine::onBLEDisconnected()
{
    if(current == NodeState::BLESession)
    {
        setState(NodeState::Idle);
    }
}

void StateMachine::onDownloadCompleted()
{
    if(current == NodeState::BLESession)
    {
        setState(NodeState::PreSleep);
    }
}


void StateMachine::startBLEWindow()
{
    setState(NodeState::BLEWait);
}

void StateMachine::endBLESession()
{
    if(current == NodeState::BLESession)
    {
        setState(NodeState::PreSleep);
    }
}


void StateMachine::start()
{
    Serial.println();
    Serial.println("FSM Start");

    if (Wake.timerWakeup())
    {
        Serial.println("Wake-up from Timer");

        setState(NodeState::Sampling);
    }
    else
    {
        Serial.println("Cold Boot");

        setState(NodeState::Idle);
    }
}



