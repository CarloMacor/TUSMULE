#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H

#include <Arduino.h>
#include <NimBLEDevice.h>

class BLEManager
{
public:

    //----------------------------------------------------------
    // Initialization
    //----------------------------------------------------------

    bool begin();

    void update();

    //----------------------------------------------------------
    // Scan
    //----------------------------------------------------------

    bool startScan();

    bool stopScan();

    bool scanning() const;

    bool nodeFound() const;

    String nodeName() const;

    String nodeAddress() const;

    int nodeRSSI() const;

    //----------------------------------------------------------
    // Connection
    //----------------------------------------------------------

    bool connect();

    void disconnect();

    bool connected() const;

    //----------------------------------------------------------
    // Data
    //----------------------------------------------------------

    bool available() const;

    std::string read();

    void send(const std::string& msg);

    void sendBinary(const void* data,
                    uint16_t size);

private:

    //----------------------------------------------------------
    // Internal
    //----------------------------------------------------------

    void setConnected(bool state);

    //----------------------------------------------------------
    // Scan Callback
    //----------------------------------------------------------

    class ScanCallbacks : public NimBLEScanCallbacks
    {
    public:

        void onResult(const NimBLEAdvertisedDevice* device) override;
    };

    //----------------------------------------------------------
    // Client Callback
    //----------------------------------------------------------

    class ClientCallbacks : public NimBLEClientCallbacks
    {
    public:

        void onConnect(NimBLEClient* client) override;

        void onDisconnect(NimBLEClient* client,
                          int reason) override;
    };

    //----------------------------------------------------------
    // Notify Callback
    //----------------------------------------------------------

    static void notifyCallback(
            NimBLERemoteCharacteristic* characteristic,
            uint8_t* data,
            size_t length,
            bool isNotify);

    //----------------------------------------------------------
    // Callback Objects
    //----------------------------------------------------------

    ScanCallbacks scanCallbacks;

    ClientCallbacks clientCallbacks;

    //----------------------------------------------------------
    // BLE Objects
    //----------------------------------------------------------

    NimBLEScan* scanner = nullptr;

    NimBLEClient* client = nullptr;

    NimBLEAdvertisedDevice* advertisedDevice = nullptr;

    NimBLERemoteService* service = nullptr;

    NimBLERemoteCharacteristic* rx = nullptr;

    NimBLERemoteCharacteristic* tx = nullptr;

    //----------------------------------------------------------
    // Runtime State
    //----------------------------------------------------------

    bool isScanning = false;

    bool isConnected = false;

    bool foundNode = false;

    bool newMessage = false;

    std::string rxBuffer;

    String foundName;

    String foundAddress;

    int foundRSSI = 0;
};

extern BLEManager BLE;

#endif