#include "BLEAdvertising.h"

#if CONFIG_ENABLED(DEVICE_BLE)

#include <cstdint>
#include "ble_advdata.h"

constexpr uint8_t FLAGS[1] = {0x06}; // Br Edr Not Supported, LE General Discovery Mode
constexpr uint16_t SERVICEDATA_UUID = 0x181C;

BLEAdvertising::BLEAdvertising(BLEDevice &ble) : ble(ble), isAdvertise(false), dataLength(0) {
}

void BLEAdvertising::startAdvertising()
{
    stopAdvertising();
    setGapName();


    uint8_t advHandle;
    static ble_advdata_t advData;
    static ble_advdata_t advScanData;
    static uint8_t  enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];
    static uint8_t  enc_advScandata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];

    memset(&advHandle, 0, sizeof(advHandle));
    memset(enc_advdata, 0, BLE_GAP_ADV_SET_DATA_SIZE_MAX);
    memset(enc_advScandata, 0, BLE_GAP_ADV_SET_DATA_SIZE_MAX);

    advData.uuids_complete.uuid_cnt = 0;
    advData.include_appearance = true;
    advData.name_type = BLE_ADVDATA_FULL_NAME;
    advData.flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED | BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE;

    ble_advdata_service_data_t serviceData[1];
    uint8_array_t serviceArray;

    serviceArray.p_data = (uint8_t *) data;
    serviceArray.size = dataLength;

    serviceData[0].data = serviceArray;
    serviceData[0].service_uuid = SERVICEDATA_UUID;

    advScanData.p_service_data_array = serviceData;
    advScanData.service_data_count = 1;



    ble_gap_adv_params_t    gap_adv_params;
    memset( &gap_adv_params, 0, sizeof( gap_adv_params));
    gap_adv_params.properties.type  = BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED;
    gap_adv_params.duration         = MICROBIT_BLE_ADVERTISING_TIMEOUT /* timeout_seconds */ * 100;              //10 ms units
    gap_adv_params.filter_policy    = BLE_GAP_ADV_FP_ANY;
    gap_adv_params.primary_phy      = BLE_GAP_PHY_1MBPS;

    gap_adv_params.interval         = ( 1000 * MICROBIT_BLE_ADVERTISING_INTERVAL/* interval_ms */) / 625;  // 625 us units
    
    if ( gap_adv_params.interval < BLE_GAP_ADV_INTERVAL_MIN)
        gap_adv_params.interval = BLE_GAP_ADV_INTERVAL_MIN;

    if ( gap_adv_params.interval > BLE_GAP_ADV_INTERVAL_MAX)
        gap_adv_params.interval = BLE_GAP_ADV_INTERVAL_MAX;


    ble_gap_adv_data_t  gap_adv_data;
    memset( &gap_adv_data, 0, sizeof( gap_adv_data));
    gap_adv_data.adv_data.p_data    = enc_advdata;
    gap_adv_data.adv_data.len       = BLE_GAP_ADV_SET_DATA_SIZE_MAX;

    gap_adv_data.scan_rsp_data.p_data    = enc_advScandata;
    gap_adv_data.scan_rsp_data.len       = BLE_GAP_ADV_SET_DATA_SIZE_MAX;
    

    MICROBIT_BLE_ECHK( ble_advdata_encode( &advData, gap_adv_data.adv_data.p_data, &gap_adv_data.adv_data.len));
    MICROBIT_BLE_ECHK( ble_advdata_encode( &advScanData, gap_adv_data.scan_rsp_data.p_data, &gap_adv_data.scan_rsp_data.len));
    MICROBIT_BLE_ECHK( sd_ble_gap_adv_set_configure( &advHandle, &gap_adv_data, &gap_adv_params));

    ble.advertise();
    isAdvertise = true;
}

void BLEAdvertising::stopAdvertising()
{
    ble.stopAdvertising();
    isAdvertise = false;
}

void BLEAdvertising::updateAdvertising()
{
    if (name.size() == 0 || dataLength == 0)
    {
        stopAdvertising();
        return;
    }

    if( isAdvertise ){
        startAdvertising();
    }
}

void BLEAdvertising::setGapName()
{
    int len = sprintf(gapName, "%s", name.substr(0, NAME_MAX_SIZE).c_str());

    ble_gap_conn_sec_mode_t permissions;
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&permissions);
    MICROBIT_BLE_ECHK(sd_ble_gap_device_name_set(&permissions, (uint8_t *)gapName, len));
}

void BLEAdvertising::setData(std::string str){
    dataLength = sprintf(data, "%s", str.substr(0, DATA_MAX_SIZE).c_str());

    updateAdvertising();
}

#endif