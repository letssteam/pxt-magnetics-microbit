#include "pxt.h"
#include "MicroBit.h"


#include <string>

#include "BLEAdvertising.h"

BLEAdvertising *advertising = nullptr;

using namespace pxt;

namespace magnetics {
    
    void configure(){
        if( advertising == nullptr ){
            advertising = BLEAdvertising::getInstance(uBit.ble);
        }
    }

    //%
    void startAdvertising(){
        configure();
        advertising->startAdvertising();
    }

    //%
    void stopAdvertising(){
        configure();
        advertising->stopAdvertising();
    }

    //%
    void startScanning(){
        configure();
        advertising->startScanning();
    }

    //%
    void stopScanning(){
        configure();
        advertising->stopScanning();
    }

    //%
    void setData(String data){
        configure();
        advertising->setData(std::string(data->getUTF8Data()));
    }

    //%
    void setName(String name){
        configure();
        advertising->setDeviceName(std::string(name->getUTF8Data()));
    }

    //%
    int available(){
        configure();
        return (int)advertising->scanReportAvailable();
    }

    //%
    bool availableScanFor(String name){
        configure();

        std::string peerName = std::string(name->getUTF8Data());
        return advertising->scanReportAvailableFor(peerName);
    }

    //%
    String getDataFrom(String name){
        configure();

        std::string peerName = std::string(name->getUTF8Data());
        std::string result = advertising->getServiceDataFrom(peerName);

        return mkString(result.data(), result.size());
    }

    //%
    String peekDataFrom(String name){
        configure();

        std::string peerName = std::string(name->getUTF8Data());
        std::string result = advertising->peekServiceDataFrom(peerName);

        return mkString(result.data(), result.size());
    }

    //%
    void onNewMessage(Action action){
        configure();
        advertising->onNewMessage(action);
    }
}