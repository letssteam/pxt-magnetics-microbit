#include "BLEAdvertising.h"

#if CONFIG_ENABLED(DEVICE_BLE)

#include <cstdint>
#include "ble_advdata.h"

constexpr uint8_t FLAGS[1] = {0x06}; // Br Edr Not Supported, LE General Discovery Mode
constexpr uint16_t SERVICEDATA_UUID = 0x181C;

void BLEAdvertising::startAdvertising()
{
    stopAdvertising();
    setName();
    setData();


    uint8_t advHandle;
    static ble_advdata_t advData;
    static uint8_t  enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];
    //static uint8_t  enc_scandata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];

    advData.uuids_complete.uuid_cnt = 0;
    advData.include_appearance = true;
    advData.name_type = BLE_ADVDATA_FULL_NAME;
    advData.flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED | BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE;

    //sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_COMPUTER);

    ble_advdata_service_data_t serviceData[1];
    uint8_array_t serviceArray;

    serviceArray.p_data = data;
    serviceArray.size = (data.size() > 32) ? 32 : data.size();

    serviceData[0].data = (uint8_t *) serviceArray;
    serviceData[0].service_uuid = SERVICEDATA_UUID;

    advData.p_service_data_array = serviceData;
    advData.service_data_count = 1;



    ble_gap_adv_params_t    gap_adv_params;
    memset( &gap_adv_params, 0, sizeof( gap_adv_params));
    gap_adv_params.properties.type  = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
    gap_adv_params.duration         = MICROBIT_BLE_ADVERTISING_TIMEOUT /* timeout_seconds */ * 100;              //10 ms units
    gap_adv_params.filter_policy    = BLE_GAP_ADV_FP_ANY;
    gap_adv_params.primary_phy      = BLE_GAP_PHY_1MBPS;

    gap_adv_params.interval         = ( 1000 * MICROBIT_BLE_ADVERTISING_INTERVAL/* interval_ms */) / 625;  // 625 us units
    if ( gap_adv_params.interval < BLE_GAP_ADV_INTERVAL_MIN) gap_adv_params.interval = BLE_GAP_ADV_INTERVAL_MIN;
    if ( gap_adv_params.interval > BLE_GAP_ADV_INTERVAL_MAX) gap_adv_params.interval = BLE_GAP_ADV_INTERVAL_MAX;


    ble_gap_adv_data_t  gap_adv_data;
    memset( &gap_adv_data, 0, sizeof( gap_adv_data));
    gap_adv_data.adv_data.p_data    = enc_advdata;
    gap_adv_data.adv_data.len       = BLE_GAP_ADV_SET_DATA_SIZE_MAX;


    

    MICROBIT_BLE_ECHK( ble_advdata_encode( &advData, gap_adv_data.adv_data.p_data, &gap_adv_data.adv_data.len));
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
    if (name.size() == 0 || data.size() == 0)
    {
        stopAdvertising();
        return;
    }

    if( isAdvertise ){
        startAdvertising();
    }
}

void BLEAdvertising::setName()
{
    int len = sprintf(gapName, "%s", name.c_str());
    ble_gap_conn_sec_mode_t permissions;
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&permissions);
    MICROBIT_BLE_ECHK(sd_ble_gap_device_name_set(&permissions, (uint8_t *)gapName, len));
}

void BLEAdvertising::setData(){
    sprintf(data, "%s", data.substr(0, 32).c_str());
}

#endif