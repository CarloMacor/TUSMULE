/******************************************************************************
 *
 *  TUSMULE Embedded Platform
 *
 *  Main Firmware
 *
 ******************************************************************************/

#include <Arduino.h>

#include "Config.h"
#include "RTCManager.h"
#include "Sensor.h"
#include "Storage.h"
#include "Scheduler.h"
#include "SampleBuilder.h"
#include "BLEManager.h"
#include "BLEProtocol.h"
#include "EnergyManager.h"
#include "SleepPlanner.h"
#include "StateMachine.h"
#include "WakeReason.h"
#include "Mission.h"
#include "MissionStorage.h"
#include "NodeConfiguration.h"


void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.begin(115200);
    delay(10000);

    Serial.println("====================================");
    Serial.println("      TUSMULE Node Firmware");


  //  esp_sleep_wakeup_cause_t cause = esp_sleep_get_wakeup_cause();
    
    Serial.println();
    Serial.println("====================================");
    Serial.println("      TUSMULE Node Firmware");
    Serial.print("Firmware Version : ");
    Serial.println(FW_VERSION);
    Serial.print("Node ID          : ");
    Serial.println(NODE_ID);
    Serial.println("====================================");
    Serial.println();

    //----------------------------------------------------------
    // Modules initialization
    //----------------------------------------------------------
    RTC.begin();
    NodeConfig.begin();
    Wake.begin();
    Energy.begin();
    SoilSensor.begin();
    Samples.begin();
    Builder.begin();
    NodeScheduler.begin();
    BLE.begin();
    Protocol.begin();
    Sleep.begin();    
    FSM.begin();
    NodeMission.begin();
    MissionStore.begin();

    FSM.start();
uint32_t next;

if(MissionStore.loadNextFlight(next))
{
    NodeMission.setNextFlight(next);

    Serial.print("Mission Loaded : ");

    Serial.println(next);
}


    Serial.println();
    Serial.println("System Ready");
    Serial.println();
}

void loop()
{
 
    //----------------------------------------------------------
    // Update modules
    //----------------------------------------------------------
    RTC.update();
   // Node.update();
    if(NodeScheduler.sampleEvent())
    {
     FSM.onSampleEvent();
    }


    BLE.update();
    FSM.update();
    Protocol.update();
    Energy.update();

static uint32_t lastPrint = 0;


}