#ifndef __BLE_ADVERTISING_H__
#define __BLE_ADVERTISING_H__

#include "MicroBitConfig.h"

#if CONFIG_ENABLED(DEVICE_BLE)

#include "MicroBitBLEManager.h"

#include "pxt.h"
#include "ble.h"
#include <string>


constexpr int DATA_MAX_SIZE = 32;

class BLEAdvertising
{

    public:
        BLEAdvertising(BLEDevice &ble) : ble(ble), isAdvertise(false) {}
        void setDeviceName(std::string name) { this->name = name; updateAdvertising(); }
        void setData(std::string str);

        void startAdvertising();
        void stopAdvertising();

    private:
        BLEDevice &ble;
        bool isAdvertise;
        std::string name;

        int dataLength;
        char data[DATA_MAX_SIZE];
        char gapName[28];

        void updateAdvertising();
        void setGapName();
};

#endif

#endif