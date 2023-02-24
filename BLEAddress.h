#ifndef __MAGNETICS_BLE_ADDRESS_H__
#define __MAGNETICS_BLE_ADDRESS_H__

#include <cstdint>
#include <string>
#include <cstring>

class BLEAddress{

    public:
        BLEAddress(const uint8_t* addr) {
            memcpy(address, addr, 6);
        }

        std::string to_string(){
            std::string str = "";
            char buffer[3] = {0};

            for(int8_t i = 5; i >= 0; i--){
                sprintf(buffer, "%0.2X:", address[i]);
                str += buffer[0];
                str += buffer[1];
                str += ':';
            }

            return str.substr(0, str.size() - 1);
        }

        inline bool operator==(const BLEAddress& other){
            return memcmp(this->address, other.address, 6) == 0;
        }

    private:
        uint8_t address[6];

};

#endif