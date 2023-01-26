#ifndef __MAGNETICS_BLE_ADDRESS_H__
#define __MAGNETICS_BLE_ADDRESS_H__

#include <cstdint>
#include <string>
#include <cstring>

class BLEAddress{

    public:
        BLEAddress(uint8_t* addr) {
            memcpy(address, addr, 6);
        };

        std::string to_string(){
            std::string str(17, '\0'); // 6*2 hex digits + 5 seperators
            char buffer[3] = {0};

            for(uint8_t i = 0; i < 6; i++){
                sprintf(buffer, "%0.2X:", address[i]);
                str.append(buffer);
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