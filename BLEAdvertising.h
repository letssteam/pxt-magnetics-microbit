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
#include <vector>
#include <functional>

#include "AdvertisingReport.h"


constexpr int NAME_MAX_SIZE = 10;
constexpr int DATA_MAX_SIZE = 20;

class BLEAdvertising
{

    public:
        /// @brief Get the instance of BLEAdvertising (created if needed and if ble parameter is provided)
        /// @param ble BLEDevice instance
        /// @return BLEAdvertising object if exist (or created) or nullptr if no instance exist (and no ble parameter)
        static BLEAdvertising* getInstance(BLEDevice* ble = nullptr);


        /// @brief Set the device name during advertising
        /// @param name The name of the device
        void setDeviceName(std::string name) { this->name = name; updateAdvertising(); }


        /// @brief Set the data sent with advertising packets
        /// @param str The data
        void setData(std::string str);


        /// @brief Start the advertisment (If the scan is in progress, it will be halted)
        void startAdvertising();

        /// @brief Stop the advertisment
        void stopAdvertising();

        /// @brief Start the scanning (If the advertisment is in progress, it will be halted)
        void startScanning();

        /// @brief Stop the scanning
        void stopScanning();

        /// @brief Handle an advertising report (or scanning report)
        /// @param report the report to handle
        void handleAdvertisingReport();


        /// @brief Get how many scan result is available
        /// @return 
        uint8_t scanReportAvailable();


        /// @brief Get if any report is avaible for device with specific name
        /// @param name The device name
        /// @return 
        bool scanReportAvailableFor(std::string& name);
        
        /// @brief Get all available report. WARNING: They will be removed from the available reports 
        /// @return Vector of reports
        std::vector<AdvertisingReport> getAllScanReport();
        
        /// @brief Get available report for specific name. WARNING: It will be removed from the available reports 
        /// @param name device name
        /// @return 
        std::string getServiceDataFrom(std::string& name);
        
        /// @brief Get available report for specific name. WARNING: It will *NOT* be removed from the available reports 
        /// @param name device name
        /// @return 
        std::string peekServiceDataFrom(std::string& name);

        /// @brief Register a action, when new (and different) message arrived from device.
        /// @param action 
        void onNewMessage(pxt::Action action);


    private:
        static BLEAdvertising* instance;

        BLEDevice *ble;
        bool isAdvertise;
        bool isScanning;
        std::string name;

        int dataLength;
        char data[DATA_MAX_SIZE];
        char gapName[28];

        std::vector<AdvertisingReport> advReports;
        std::vector<pxt::Action> onNewMessageEvents; 

        BLEAdvertising(BLEDevice *ble);
        void updateAdvertising();
        void setGapName();
        void fireOnNewMessageCallbacks();
};

#endif

#endif