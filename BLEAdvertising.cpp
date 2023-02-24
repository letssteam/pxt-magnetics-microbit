#include "BLEAdvertising.h"

#if CONFIG_ENABLED(DEVICE_BLE)

#include <cstdint>
#include <list>
#include "ble_advdata.h"
#include "ble_gap.h"
#include "nrf_sdh.h"

#include "AdvertisingReport.h"

#define microbit_ble_OBSERVER_PRIO  2

constexpr uint8_t MAX_NB_ADVERTISING_REPORT = 16;
constexpr uint8_t FLAGS[1] = {0x06}; // Br Edr Not Supported, LE General Discovery Mode
constexpr uint16_t SERVICEDATA_UUID = 0x181C;

static uint8_t scanReportBuffer[BLE_GAP_SCAN_BUFFER_MIN] = {0x00};
static ble_data_t scanReport = { scanReportBuffer, sizeof(scanReportBuffer) };

static std::list<ble_gap_evt_adv_report_t> reportsBuffer; 
static void magnetics_ble_evt_handler(ble_evt_t const * event, void * context);

BLEAdvertising* BLEAdvertising::instance = nullptr;

BLEAdvertising* BLEAdvertising::getInstance(BLEDevice* ble){

    if( instance == nullptr && ble != nullptr ){
        instance = new BLEAdvertising(ble);
    }

    return instance;
}

void BLEAdvertising::setData(std::string str){
    dataLength = sprintf(data, "%s", str.substr(0, DATA_MAX_SIZE).c_str());
    updateAdvertising();
}

void BLEAdvertising::startAdvertising()
{
    if( isScanning ){
        stopScanning();
    }

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

    ble->advertise();
    isAdvertise = true;
}

void BLEAdvertising::stopAdvertising()
{
    ble->stopAdvertising();
    isAdvertise = false;
}


void BLEAdvertising::startScanning(){

    if(isAdvertise){
        stopAdvertising();
    }

    static ble_gap_scan_params_t const scanParam =
    {
        .extended                   = 0,
        .report_incomplete_evts     = 0,
        .active                     = 1,
        .filter_policy              = BLE_GAP_SCAN_FP_ACCEPT_ALL,
        .scan_phys                  = BLE_GAP_PHY_1MBPS,
        .interval                   = 0x0050,
        .window                     = 0x0050,
        .timeout                    = 0x0000,
        .channel_mask               = {0x00, 0x00, 0x00, 0x00, 0x00}
    };

    char code[11] = {0x00};

    uint32_t res = sd_ble_gap_scan_start(&scanParam, &scanReport);
    
    if( res == NRF_SUCCESS){
    }
    else{
        uBit.display.scroll("Scan Fail");
        sprintf(code, "0x%0.8X", res);
        uBit.display.scroll(code);
    }

    isScanning = true;
}

void BLEAdvertising::stopScanning(){
    MICROBIT_BLE_ECHK( sd_ble_gap_scan_stop() );
    isScanning = false;
}

void BLEAdvertising::handleAdvertisingReport(){
    static bool isHandleRunning = false;

    if( isHandleRunning ){ return; }

    isHandleRunning = true;

    while(reportsBuffer.size() > 0){
        AdvertisingReport advRep(reportsBuffer.front());
        reportsBuffer.pop_front();

        bool isNew = true;

        for( uint8_t i = 0; i < advReports.size(); ++i){
            if( advReports[i].address == advRep.address ){
                advReports[i].serviceData.uuid = advRep.serviceData.uuid;

                if( advReports[i].name != advRep.name && !advRep.name.empty()){
                    advReports[i].name = advRep.name;
                    fireOnNewMessageCallbacks();
                }

                if( !advRep.serviceData.data.empty() && advRep.serviceData.uuid != 0x0000 && advReports[i].serviceData.data != advRep.serviceData.data ){
                    advReports[i].serviceData.data = advRep.serviceData.data;
                    fireOnNewMessageCallbacks();
                }

                isNew = false;
                break;
            }
        }

        if( isNew ){
            if(advReports.size() >= MAX_NB_ADVERTISING_REPORT){
                return;
            }
            else{
                advReports.push_back(advRep);
                fireOnNewMessageCallbacks();
            }
        }
    }

    isHandleRunning = false;
}

uint8_t BLEAdvertising::scanReportAvailable(){
    return advReports.size();
}


bool BLEAdvertising::scanReportAvailableFor(std::string& name){

    for(auto report : advReports){
        if( report.name == name ){
            return true;
        }
    }

    return false;
}

std::vector<AdvertisingReport> BLEAdvertising::getAllScanReport(){
    auto results = std::vector<AdvertisingReport>(advReports);
    advReports.clear();

    return results;
}

std::string BLEAdvertising::getServiceDataFrom(std::string& name){
    std::string result = "";

    for( auto it = advReports.begin(); it != advReports.end(); ++it){
        if( it->name == name ){
            result = it->serviceData.data.to_string();
            advReports.erase(it);
            break;
        }
    }

    return result;
}

std::string BLEAdvertising::peekServiceDataFrom(std::string& name){
    std::string result = "";

    for( auto it = advReports.begin(); it != advReports.end(); ++it){
        if( it->name == name ){
            result = it->serviceData.data.to_string();
            break;
        }
    }

    return result;
}

void BLEAdvertising::onNewMessage(pxt::Action action){
    onNewMessageEvents.push_back(action);
}

BLEAdvertising::BLEAdvertising(BLEDevice *ble) : ble(ble), isAdvertise(false), isScanning(false), dataLength(0) {
    stopAdvertising();
    NRF_SDH_BLE_OBSERVER( microbit_magnetics_observer, microbit_ble_OBSERVER_PRIO, magnetics_ble_evt_handler, NULL);
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

void BLEAdvertising::fireOnNewMessageCallbacks(){
    for( auto elem : onNewMessageEvents ){
        pxt::runInParallel(elem);
    }
}

static void magnetics_ble_evt_handler(ble_evt_t const * event, void * context){

    auto inst = BLEAdvertising::getInstance();

    if( inst != nullptr ){
        switch(event->header.evt_id){
            case BLE_GAP_EVT_ADV_REPORT:
                if( reportsBuffer.size() < MAX_NB_ADVERTISING_REPORT ){
                    reportsBuffer.push_back(event->evt.gap_evt.params.adv_report);
                    inst->handleAdvertisingReport();
                }
                break;

            default:
                break;
        }
    }

    sd_ble_gap_scan_start(NULL, &scanReport);
}

#endif