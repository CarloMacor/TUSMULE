/******************************************************************************
 *
 * TUSMULE Drone
 *
 * BLE Protocol Client
 *
 ******************************************************************************/

#include "BLEProtocol.h"

#include "BLEManager.h"
#include "ProtocolDefs.h"
#include "Mission.h"
#include "RTCManager.h"

BLEProtocol Protocol;

//==========================================================

bool BLEProtocol::begin()
{
    state = Step::Idle;

    timeout = millis();

    sampleCount = 0;

    currentSample = 0;

    return true;
}

//==========================================================

void BLEProtocol::start()
{
    state = Step::SendHello;

    timeout = millis();
}

//==========================================================

bool BLEProtocol::busy() const
{
    return state != Step::Idle &&
           state != Step::Completed &&
           state != Step::Error;
}

//==========================================================

bool BLEProtocol::finished() const
{
    return state == Step::Completed;
}

//==========================================================

bool BLEProtocol::expired(uint32_t ms)
{
    return millis() - timeout > ms;
}

//==========================================================

void BLEProtocol::nextState(Step s)
{
    state = s;

    timeout = millis();
}


void BLEProtocol::update()
{
    switch(state)
    {

    //------------------------------------------------------
    case Step::Idle:
        break;

    //------------------------------------------------------
    case Step::SendHello:

        Serial.println("HELLO");

        BLE.send(CMD_HELLO);

        nextState(Step::WaitHello);

        break;

    //------------------------------------------------------
    case Step::WaitHello:

        if(BLE.available())
        {
            String msg = BLE.read().c_str();

            Serial.print("RX ");

            Serial.println(msg);

            nextState(Step::SendInfo);
        }

        if(expired())
        {
            state = Step::Error;
        }

        break;

    //------------------------------------------------------
    case Step::SendInfo:

        BLE.send(CMD_INFO);

        nextState(Step::WaitInfo);

        break;


            //------------------------------------------------------
    case Step::WaitInfo:

        if(BLE.available())
        {
            String msg = BLE.read().c_str();

            Serial.println(msg);

            nextState(Step::SendCount);
        }

        if(expired())
        {
            state = Step::Error;
        }

        break;

    //------------------------------------------------------
    case Step::SendCount:

        BLE.send(CMD_COUNT);

        nextState(Step::WaitCount);

        break;

    //------------------------------------------------------
    case Step::WaitCount:

        if(BLE.available())
        {
            String msg = BLE.read().c_str();

            sampleCount = msg.toInt();

            currentSample = 0;

            Serial.print("Samples = ");

            Serial.println(sampleCount);

            nextState(Step::SendBegin);
        }

        if(expired())
        {
            state = Step::Error;
        }

        break;

            //------------------------------------------------------
    case Step::SendBegin:

        Serial.println("BEGIN");

        BLE.send(CMD_BEGIN);

        nextState(Step::WaitBegin);

        break;

    //------------------------------------------------------
    case Step::WaitBegin:

        if(BLE.available())
        {
            String msg = BLE.read().c_str();

            Serial.println(msg);

            nextState(Step::SendNext);
        }

        if(expired())
        {
            state = Step::Error;
        }

        break;

    //------------------------------------------------------
    case Step::SendNext:

        if(currentSample >= sampleCount)
        {
            nextState(Step::SendEnd);
            break;
        }

        BLE.send(CMD_NEXT);

        nextState(Step::WaitNext);

        break;

    //------------------------------------------------------
    case Step::WaitNext:

        if(BLE.available())
        {
            std::string data = BLE.read();

            Serial.print("Sample ");

            Serial.print(currentSample + 1);

            Serial.print(" / ");

            Serial.println(sampleCount);

            //
            // TODO:
            // Salvare qui il campione ricevuto
            //

            currentSample++;

            nextState(Step::SendNext);
        }

        if(expired())
        {
            state = Step::Error;
        }

        break;

    //------------------------------------------------------
    case Step::SendEnd:

        BLE.send(CMD_END);

        nextState(Step::WaitEnd);

        break;

    //------------------------------------------------------
    case Step::WaitEnd:

        if(BLE.available())
        {
            BLE.read();

            nextState(Step::SendTime);
        }

        if(expired())
        {
            state = Step::Error;
        }

        break;

    //------------------------------------------------------
    case Step::SendTime:

        {
            char buffer[32];

            sprintf(buffer,
                    "%lu",
                    (unsigned long)RTC.now());

   char msg[64];

        sprintf(msg,
          "%s%lu",
         CMD_TIME,
          (unsigned long)RTC.now());

            BLE.send(std::string(msg));

            nextState(Step::WaitTime);
        }

        break;

    //------------------------------------------------------
    case Step::WaitTime:

        if(BLE.available())
        {
            BLE.read();

        nextState(Step::SendNextFlight);
        }

        if(expired())
        {
            state = Step::Error;
        }

        break;

  case Step::SendNextFlight:
{
    char msg[64];

    sprintf(msg,
            "%s%lu",
            CMD_NEXTFLIGHT,
            (unsigned long)NodeMission.nextFlight());

    BLE.send(std::string(msg));

    nextState(Step::WaitNextFlight);
}
break;

case Step::WaitNextFlight:

    if(BLE.available())
    {
        BLE.read();

        nextState(Step::SendBye);
    }

    if(expired())
    {
        state = Step::Error;
    }

    break;


    //------------------------------------------------------
    case Step::SendBye:

        BLE.send(CMD_BYE);

        nextState(Step::WaitBye);

        break;

    //------------------------------------------------------
    case Step::WaitBye:

        if(BLE.available())
        {
            BLE.read();

            nextState(Step::Completed);
        }

        if(expired())
        {
            state = Step::Error;
        }

        break;

    //------------------------------------------------------
    case Step::Completed:

        break;

    //------------------------------------------------------
    case Step::Error:

        Serial.println("BLE Protocol Error");

        break;

    }

}