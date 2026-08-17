/******************************************************************************
 *
 *  TUSMULE Embedded Platform
 *
 *  Module:
 *      BLEProtocol
 *
 ******************************************************************************/

#include <Arduino.h>

#include "BLEProtocol.h"
#include "BLEManager.h"
#include "Storage.h"
#include "Config.h"
#include "ProtocolSession.h"
#include "SampleSerializer.h"
#include "RTCManager.h"
#include "StateMachine.h"
#include "Mission.h"
#include "MissionStorage.h"

BLEProtocol Protocol;

//==========================================================
// Initialization
//==========================================================

bool BLEProtocol::begin()
{
    return true;
}

//==========================================================
// Main Update
//==========================================================

void BLEProtocol::update()
{
    if (BLE.available())
    {
    std::string cmd = BLE.read();

    Serial.print("Protocol RX -> ");
    Serial.println(cmd.c_str());

  

    //------------------------------------------------------
    // PING
    //------------------------------------------------------

    if (cmd == "PING")
    {
        cmdPing();
        return;
    }

    //------------------------------------------------------
    // INFO
    //------------------------------------------------------

    if (cmd == "INFO")
    {
        cmdInfo();
        return;
    }

    //------------------------------------------------------
    // COUNT
    //------------------------------------------------------

    if (cmd == "COUNT")
    {
        cmdCount();
        return;
    }

if(cmd=="MISSION?")
{
    cmdMission();

    return;
}
  //------------------------------------------------------
    // GET n
    //------------------------------------------------------

    if (cmd.rfind("GET ", 0) == 0)
    {
      uint16_t index = atoi(cmd.substr(4).c_str());
      cmdGet(index);
      return;
    }


    if(cmd.rfind("NEXTFLIGHT=",0)==0)
    {
      uint32_t t = strtoul(cmd.substr(11).c_str(), nullptr, 10);
      cmdNextFlight(t);
      return;
    }

    if(cmd == "TESTBLE")
{
    FSM.startBLEWindow();

    BLE.send("OK");

    return;
}


if(cmd=="TIME?")
{
    cmdTimeQuery();
    return;
}

if(cmd=="NEXTFLIGHT?")
{
    cmdNextFlightQuery();
    return;
}

//------------------------------------------------------
// BEGIN
//------------------------------------------------------

if(cmd == "BEGIN")
{
    cmdBegin();
    return;
}

//------------------------------------------------------
// NEXT
//------------------------------------------------------

if(cmd == "NEXT")
{
    cmdNext();
    return;
}

//------------------------------------------------------
// NEXTBIN
//------------------------------------------------------

if(cmd=="NEXTBIN")
{
    cmdNextBinary();
    return;
}

//------------------------------------------------------
// END
//------------------------------------------------------

if(cmd == "END")
{
    cmdEnd();
    return;
}

if(cmd=="HELLO")
{
    cmdHello();
    return;
}

if(cmd=="BYE")
{
    cmdBye();
    return;
}

if(cmd.rfind("TIME=",0)==0)
{
    uint32_t t =
        strtoul(cmd.substr(5).c_str(), nullptr, 10);

    cmdTime(t);

    return;
}


    //------------------------------------------------------
    // Unknown Command
    //------------------------------------------------------

      BLE.send("ERR");
    }



}



//==========================================================
// Command: PING
//==========================================================

void BLEProtocol::cmdPing()
{
    BLE.send("PONG");
}

//==========================================================
// Command: INFO
//==========================================================


void BLEProtocol::cmdInfo()
{
    String msg;

    msg.reserve(128);

    msg = "NODE=";
    msg += NODE_ID;

    msg += ";FW=";
    msg += FW_VERSION;

    msg += ";TIME=";
    msg += RTC.now();

    msg += ";NEXT=";
    msg += Session.nextFlight();

    msg += ";COUNT=";
    msg += Samples.count();

    BLE.send(msg.c_str());
}


//==========================================================
// Command: COUNT
//==========================================================

void BLEProtocol::cmdCount()
{
    String msg;

    msg = "COUNT=";
    msg += Samples.count();

    BLE.send(msg.c_str());
}



//==========================================================
// Command: GET
//==========================================================

void BLEProtocol::cmdGet(uint16_t index)
{
    Sample sample;

    if (!Samples.read(index, sample))
    {
        BLE.send("ERR");

        return;
    }


    BLE.send(SampleSerializer::toText(sample).c_str());

}



void BLEProtocol::cmdBegin()
{
    Session.start();

    String msg = "COUNT=";

    msg += Samples.count();

    BLE.send(msg.c_str());
}


void BLEProtocol::cmdNext()
{
    if(!Session.isActive())
    {
        BLE.send("NOSESSION");
        return;
    }

    Sample sample;

    if(!Samples.read(Session.current(), sample))
    {
        BLE.send("END");
        return;
    }

    Session.next();

    BLE.send(SampleSerializer::toText(sample).c_str());

}



void BLEProtocol::cmdEnd()
{
    Session.stop();

    BLE.send("OK");
}



void BLEProtocol::cmdHello()
{
    String msg;

    msg  = "NODE=";
    msg += NODE_ID;

    msg += ";FW=";
    msg += FW_VERSION;

    msg += ";COUNT=";
    msg += Samples.count();


    BLE.send(msg.c_str());

    NodeMission.setLastConnection(RTC.now());
}

//==========================================================
// Command: BYE
//==========================================================

void BLEProtocol::cmdBye()
{
    Session.stop();

    BLE.send("BYE");
    FSM.endBLESession();

    NodeMission.setLastDownload(RTC.now());
 //  FSM.setState(NodeState::DeepSleep);
  
}



void BLEProtocol::cmdNextFlightQuery()
{
    String msg = "NEXTFLIGHT=";
    msg += Session.nextFlight();

    BLE.send(msg.c_str());
}


//==========================================================
// Command: NEXTBIN
//==========================================================

void BLEProtocol::cmdNextBinary()
{
    if(!Session.isActive())
    {
        BLE.send("NOSESSION");
        return;
    }

    Sample sample;

    if(!Samples.read(Session.current(), sample))
    {
        BLE.send("END");
        return;
    }

    uint8_t buffer[sizeof(Sample)];

    SampleSerializer::toBinary(sample, buffer);

    BLE.sendBinary(buffer, sizeof(Sample));

    Session.next();
}

void BLEProtocol::cmdNextFlight(uint32_t unixTime)
{
    if(unixTime <= RTC.now())
    {
        BLE.send("ERR");
        return;
    }
    NodeMission.setNextFlight(unixTime);
    MissionStore.saveNextFlight(unixTime);

    Serial.print("Next Flight : ");
    Serial.println(unixTime);

    BLE.send("OK");
}


void BLEProtocol::cmdTime(uint32_t unixTime)
{
    // Rifiuta timestamp palesemente errati
    if (unixTime < 1700000000UL)
    {
        BLE.send("ERR");
        return;
    }

    RTC.setTime(unixTime);

    Serial.print("RTC Updated : ");
    Serial.println(unixTime);

    BLE.send("OK");
}


void BLEProtocol::cmdTimeQuery()
{
    String msg = "TIME=";
    msg += RTC.now();

    BLE.send(msg.c_str());
}

void BLEProtocol::cmdMission()
{
    String msg;

    msg.reserve(96);

    msg = "NEXT=";
    msg += NodeMission.nextFlight();

    msg += ";LASTCONN=";
    msg += NodeMission.lastConnection();

    msg += ";LASTDOWN=";
    msg += NodeMission.lastDownload();

    BLE.send(msg.c_str());
}
