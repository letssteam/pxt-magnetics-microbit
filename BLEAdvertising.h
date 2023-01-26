#ifndef __BLE_ADVERTISING_H__
#define __BLE_ADVERTISING_H__

#include "MicroBitConfig.h"

#if CONFIG_ENABLED(DEVICE_BLE)

#include "MicroBitBLEManager.h"

#include "pxt.h"
#include "ble.h"
#include "ble_gap.h"
#include "nrf_sdh_ble.h"
#include <string>



constexpr int NAME_MAX_SIZE = 10;
constexpr int DATA_MAX_SIZE = 20;


class BLEAdvertising
{

    public:
        BLEAdvertising(BLEDevice &ble);
        void setDeviceName(std::string name) { this->name = name; updateAdvertising(); }
        void setData(std::string str);

        void startAdvertising();
        void stopAdvertising();

        void startScanning();
        void stopScanning();

        void handleAdvertisingReport(ble_gap_evt_adv_report_t& report);

    private:
        BLEDevice &ble;
        bool isAdvertise;
        bool isScanning;
        std::string name;

        int dataLength;
        char data[DATA_MAX_SIZE];
        char gapName[28];

        void updateAdvertising();
        void setGapName();
};

#endif

#endif