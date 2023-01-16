#include "pxt.h"
#include "MicroBit.h"


#include <string>

#include "BLEAdvertising.h"

static BLEAdvertising *advertising = nullptr;

using namespace pxt;

namespace magnetics {
    

    void configure(){
        if( advertising == nullptr ){
            advertising = new BLEAdvertising(*uBit.ble);
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
    void setData(String data){
        configure();
        advertising->setData(std::string(data->getUTF8Data()));
    }

    //%
    void setName(String name){
        configure();
        advertising->setDeviceName(std::string(name->getUTF8Data()));
    }
}