#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <Arduino.h>
#include <NimBLEDevice.h>

class BLEManager
{
public:

    bool begin();

    void update();

    bool connected() const;

    bool available() const;

    std::string read();

    void send(const std::string& msg);

    void sendBinary(const void* data, uint16_t size);

private:

    //--------------------------------------------------
    // Stato interno
    //--------------------------------------------------

    void setConnected(bool state);

    //--------------------------------------------------
    // Callback Server
    //--------------------------------------------------

    class ServerCallbacks : public NimBLEServerCallbacks
    {
    public:

        void onConnect(NimBLEServer* pServer,
                       NimBLEConnInfo& connInfo) override;

        void onDisconnect(NimBLEServer* pServer,
                          NimBLEConnInfo& connInfo,
                          int reason) override;
    };

    //--------------------------------------------------
    // Callback RX
    //--------------------------------------------------

    class RXCallbacks : public NimBLECharacteristicCallbacks
    {
    public:

        void onWrite(NimBLECharacteristic* pCharacteristic,
                     NimBLEConnInfo& connInfo) override;
    };

    //--------------------------------------------------
    // Callback Objects
    //--------------------------------------------------

    ServerCallbacks callbacks;

    RXCallbacks rxCallbacks;

    //--------------------------------------------------
    // BLE Objects
    //--------------------------------------------------

    NimBLEServer* server = nullptr;

    NimBLEService* service = nullptr;

    NimBLECharacteristic* rx = nullptr;

    NimBLECharacteristic* tx = nullptr;

    //--------------------------------------------------
    // Runtime State
    //--------------------------------------------------

    bool isConnected = false;

    bool newMessage = false;

    std::string rxBuffer;

};

extern BLEManager BLE;

#endif