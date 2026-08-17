/******************************************************************************
 *
 *  TUSMULE Embedded Platform
 *
 *  Module:
 *      BLEManager
 *
 ******************************************************************************/

#include "BLEManager.h"
#include "Config.h"

BLEManager BLE;

//==========================================================
// Internal State
//==========================================================

void BLEManager::setConnected(bool state)
{
    isConnected = state;
}

bool BLEManager::connected() const
{
    return isConnected;
}

//==========================================================
// Initialization
//==========================================================

bool BLEManager::begin()
{
    Serial.println(F("Starting BLE..."));

    //------------------------------------------------------
    // Initialize NimBLE
    //------------------------------------------------------

    NimBLEDevice::init(BLE_NAME);

    //------------------------------------------------------
    // Create Server
    //------------------------------------------------------

    server = NimBLEDevice::createServer();

    server->setCallbacks(&callbacks);

    //------------------------------------------------------
    // Create Service
    //------------------------------------------------------

    service = server->createService(BLE_SERVICE_UUID);

    //------------------------------------------------------
    // RX Characteristic
    //------------------------------------------------------

    rx = service->createCharacteristic(
            BLE_RX_UUID,
            NIMBLE_PROPERTY::WRITE);

    rx->setCallbacks(&rxCallbacks);

    //------------------------------------------------------
    // TX Characteristic
    //------------------------------------------------------

    tx = service->createCharacteristic(
            BLE_TX_UUID,
            NIMBLE_PROPERTY::READ |
            NIMBLE_PROPERTY::NOTIFY);

    //------------------------------------------------------
    // Start Service
    //------------------------------------------------------

    service->start();

    //------------------------------------------------------
    // Advertising
    //------------------------------------------------------

    NimBLEAdvertising *advertising =
        NimBLEDevice::getAdvertising();

    advertising->setName(BLE_NAME);

    advertising->enableScanResponse(true);

    advertising->addServiceUUID(service->getUUID());

    bool ok = advertising->start();

    Serial.print(F("Advertising = "));
    Serial.println(ok ? F("OK") : F("FAILED"));

    Serial.println(F("BLE Ready"));

    return true;
}

//==========================================================
// Server Callbacks
//==========================================================

void BLEManager::ServerCallbacks::onConnect(
        NimBLEServer *pServer,
        NimBLEConnInfo &connInfo)
{
    BLE.setConnected(true);

    Serial.println();
    Serial.println(F("Client Connected"));
    Serial.println();
}

void BLEManager::ServerCallbacks::onDisconnect(
        NimBLEServer *pServer,
        NimBLEConnInfo &connInfo,
        int reason)
{
    BLE.setConnected(false);

    Serial.println();
    Serial.println(F("Client Disconnected"));
    Serial.println();

    NimBLEDevice::startAdvertising();

    Serial.println(F("Advertising Restarted"));
}

//==========================================================
// RX Callback
//==========================================================

void BLEManager::RXCallbacks::onWrite(
        NimBLECharacteristic *pCharacteristic,
        NimBLEConnInfo &connInfo)
{
    BLE.rxBuffer = pCharacteristic->getValue();

    BLE.newMessage = true;

    Serial.print(F("RX BLE -> "));
    Serial.println(BLE.rxBuffer.c_str());
}

//==========================================================
// Public API
//==========================================================

bool BLEManager::available() const
{
    return newMessage;
}

std::string BLEManager::read()
{
    newMessage = false;

    return rxBuffer;
}

void BLEManager::send(const std::string &msg)
{
    if(tx == nullptr)
        return;

    tx->setValue(msg);

    tx->notify();

    Serial.print(F("TX BLE -> "));
    Serial.println(msg.c_str());
}

void BLEManager::update()
{
    // Riservato per future estensioni
}

void BLEManager::sendBinary(const void* data,
                            uint16_t size)
{
    if(tx == nullptr)
        return;

    if(!connected())
        return;        

    tx->setValue((const uint8_t*)data, size);

    tx->notify();

    Serial.print("TX Binary ");
    Serial.print(size);
    Serial.println(" bytes");
}