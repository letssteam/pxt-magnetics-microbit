#include "AdvertisingReport.h"

constexpr uint8_t GAP_COMPLETE_LOCAL_NAME = 0x09;
constexpr uint8_t GAP_SERVICE_DATA = 0x16;

AdvertisingReport::AdvertisingReport(const ble_gap_evt_adv_report_t& report) : rssi(report.rssi), name(""), address(report.peer_addr.addr), serviceData{.uuid = 0x0000, .data = RawData()} {

    uint8_t* p_data = report.data.p_data;
    uint8_t idx = 0;

    while( idx < report.data.len){
        const uint8_t len = p_data[idx++];
        const uint8_t type = p_data[idx++];
        uint8_t value[len] = {0x00};

        for(uint8_t i = 0; i < len-1; ++i){
            value[i] = p_data[idx++];
        }

        switch(type){
            case GAP_COMPLETE_LOCAL_NAME:
                name = std::string((char*) value, len - 1);
                break;

            case GAP_SERVICE_DATA:
                {
                    serviceData.uuid = ((uint16_t)value[1] << 8) | value[0];
                    serviceData.data = RawData(value + 2, len - 3);
                    break;
                }


            default: break;
        }
    }
}
