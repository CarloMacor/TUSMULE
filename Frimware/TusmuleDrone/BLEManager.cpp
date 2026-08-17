/******************************************************************************
 *
 *  TUSMULE Embedded Platform
 *
 *  Module:
 *      BLEManager (Drone)
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

bool BLEManager::scanning() const
{
    return isScanning;
}

bool BLEManager::nodeFound() const
{
    return foundNode;
}

String BLEManager::nodeName() const
{
    return foundName;
}

String BLEManager::nodeAddress() const
{
    return foundAddress;
}

int BLEManager::nodeRSSI() const
{
    return foundRSSI;
}

//==========================================================
// Initialization
//==========================================================

bool BLEManager::begin()
{
    Serial.println();
    Serial.println(F("Starting BLE Client..."));

    NimBLEDevice::init("");

    scanner = NimBLEDevice::getScan();

    if(scanner == nullptr)
    {
        Serial.println(F("Scanner creation FAILED"));
        return false;
    }

    scanner->setScanCallbacks(&scanCallbacks);

    scanner->setActiveScan(true);

    scanner->setInterval(80);

    scanner->setWindow(40);

    Serial.println(F("BLE Client Ready"));

    return true;
}


//==========================================================
// Scan Callback
//==========================================================

void BLEManager::ScanCallbacks::onResult(
        const NimBLEAdvertisedDevice* device)
{
    if(device == nullptr)
        return;

    if(!device->haveName())
        return;

    String name = device->getName().c_str();

    if(name != BLE_NAME)
        return;

    Serial.println();
    Serial.println(F("====== TUSMULE NODE FOUND ======"));

    Serial.print(F("Name : "));
    Serial.println(name);

    Serial.print(F("MAC  : "));
    Serial.println(device->getAddress().toString().c_str());

    Serial.print(F("RSSI : "));
    Serial.println(device->getRSSI());

    Serial.println(F("==============================="));

    BLE.foundNode = true;

    BLE.foundName = name;

    BLE.foundRSSI = device->getRSSI();

    BLE.foundAddress =
        device->getAddress().toString().c_str();

  

if (BLE.advertisedDevice)
{
    delete BLE.advertisedDevice;
}

BLE.advertisedDevice = new NimBLEAdvertisedDevice(*device);


    NimBLEDevice::getScan()->stop();
}


//==========================================================
// Client Callbacks
//==========================================================

void BLEManager::ClientCallbacks::onConnect(
        NimBLEClient* client)
{
    BLE.setConnected(true);

    Serial.println();
    Serial.println(F("BLE Connected"));
}

void BLEManager::ClientCallbacks::onDisconnect(
        NimBLEClient* client,
        int reason)
{
    BLE.setConnected(false);

    BLE.client = nullptr;

    BLE.service = nullptr;

    BLE.rx = nullptr;

    BLE.tx = nullptr;

BLE.foundNode = false;

BLE.newMessage = false;

BLE.rxBuffer.clear();

    Serial.println();
    Serial.println(F("BLE Disconnected"));
}



//==========================================================
// Scan
//==========================================================

bool BLEManager::startScan()
{
    if(scanner == nullptr)
        return false;

    foundNode = false;

    advertisedDevice = nullptr;

    foundName = "";

    foundAddress = "";

    foundRSSI = 0;

    isScanning = true;

    Serial.println();
    Serial.println(F("Scanning..."));

    scanner->start(0, false);

    return true;
}


bool BLEManager::stopScan()
{
    if(scanner == nullptr)
        return false;

    scanner->stop();

    isScanning = false;

    Serial.println(F("Scan stopped"));

    return true;
}

//==========================================================
// Update
//==========================================================

void BLEManager::update()
{
    if(isScanning)
    {
        if(!scanner->isScanning())
            isScanning = false;
    }
}



//==========================================================
// Connect
//==========================================================

bool BLEManager::connect()
{
    if(advertisedDevice == nullptr)
        return false;

    Serial.println();
    Serial.println(F("Connecting..."));

    client = NimBLEDevice::createClient();

    if(client == nullptr)
    {
        Serial.println(F("Client creation failed"));
        return false;
    }

    client->setClientCallbacks(&clientCallbacks, false);

    if(!client->connect(advertisedDevice))
    {
        Serial.println(F("Connection failed"));

        NimBLEDevice::deleteClient(client);

        client = nullptr;

        return false;
    }

    Serial.println(F("Connected"));

    service = client->getService(BLE_SERVICE_UUID);

    if(service == nullptr)
    {
        Serial.println(F("Service not found"));

        disconnect();

        return false;
    }

    Serial.println(F("Service OK"));

        rx = service->getCharacteristic(BLE_RX_UUID);

    tx = service->getCharacteristic(BLE_TX_UUID);

    if(rx == nullptr || tx == nullptr)
    {
        Serial.println(F("Characteristics missing"));

        disconnect();

        return false;
    }

    Serial.println(F("Characteristics OK"));

if(tx->subscribe(true, notifyCallback))
{
    Serial.println(F("Notify Enabled"));
}
else
{
    Serial.println(F("Notify FAILED"));
}


    return true;
}

//==========================================================
// Disconnect
//==========================================================

void BLEManager::disconnect()
{
    if(client)
    {
        if(client->isConnected())
            client->disconnect();

        NimBLEDevice::deleteClient(client);

        client = nullptr;
    }

    service = nullptr;

    rx = nullptr;

    tx = nullptr;

    setConnected(false);

    Serial.println(F("Disconnected"));

    if(advertisedDevice)
{
    delete advertisedDevice;
    advertisedDevice = nullptr;
}

}

//==========================================================
// Notify Callback
//==========================================================

void BLEManager::notifyCallback(
        NimBLERemoteCharacteristic* characteristic,
        uint8_t* data,
        size_t length,
        bool isNotify)
{
    BLE.rxBuffer.assign((char*)data, length);

    BLE.newMessage = true;

    Serial.print(F("RX BLE -> "));

    Serial.println(BLE.rxBuffer.c_str());
}

//==========================================================
// Available
//==========================================================

bool BLEManager::available() const
{
    return newMessage;
}

//==========================================================
// Read
//==========================================================

std::string BLEManager::read()
{
    newMessage = false;

    return rxBuffer;
}

//==========================================================
// Send
//==========================================================

void BLEManager::send(const std::string& msg)
{
    if(rx == nullptr)
        return;

    if(!connected())
        return;

    if(!rx->canWrite())
        return;

    if(rx->writeValue(msg))
    {
        Serial.print(F("TX BLE -> "));
        Serial.println(msg.c_str());
    }
    else
    {
        Serial.println(F("BLE Write Failed"));
    }
}

//==========================================================
// Send Binary
//==========================================================

void BLEManager::sendBinary(const void* data,
                            uint16_t size)
{
    if(rx == nullptr)
        return;

    if(!connected())
        return;

    if(!rx->canWrite())
        return;

    if(rx->writeValue((const uint8_t*)data,
                      size,
                      false))
    {
        Serial.print(F("TX Binary "));
        Serial.print(size);
        Serial.println(F(" bytes"));
    }
    else
    {
        Serial.println(F("Binary Write Failed"));
    }
}


