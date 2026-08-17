#ifndef CONFIG_H
#define CONFIG_H

//==========================================================
// Firmware
//==========================================================

#define FW_VERSION        "1.0.0"

//==========================================================
// Node
//==========================================================

#define NODE_ID           5

//==========================================================
// BLE
//==========================================================

#define BLE_NAME          "TUSMULE_05"

#define BLE_SERVICE_UUID  "8F5F0001-52D3-4C5A-A1B0-000000000001"
#define BLE_RX_UUID       "8F5F0002-52D3-4C5A-A1B0-000000000001"
#define BLE_TX_UUID       "8F5F0003-52D3-4C5A-A1B0-000000000001"

//==========================================================
// Scheduler
//==========================================================


#define SAMPLE_PERIOD_SEC     10
#define DRONE_PERIOD_SEC      30

// constexpr uint32_t SAMPLE_PERIOD_SEC = 3600;
// constexpr uint32_t DRONE_PERIOD_SEC  = 86400;

//==========================================================
// Storage
//==========================================================

#define STORAGE_MAX_SAMPLES   512

#define PIN_SOIL 4

constexpr uint32_t DRONE_WINDOW_SEC = 120;


#endif