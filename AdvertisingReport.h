#ifndef __MAGNETICS_ADVERTISING_REPORT_H__
#define __MAGNETICS_ADVERTISING_REPORT_H__

#include <ble_gap.h>
#include <string>
#include <vector>

#include "RawData.h"
#include "BLEAddress.h"

struct GAPServiceData {
    uint16_t uuid;
    RawData data;
};

class AdvertisingReport {

    public:
        AdvertisingReport(ble_gap_evt_adv_report_t& report);

        std::vector<GAPServiceData> serviceData;
        int8_t rssi;
        BLEAddress address;
};

#endif