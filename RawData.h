#ifndef __MAGNETICS_RAW_DATA_H__
#define __MAGNETICS_RAW_DATA_H__

#include <cstdint>
#include <string>
#include <vector>

class RawData{
    public:
        RawData() : data() {}
        
        RawData(uint8_t* data, uint8_t size) : data(data, data + size) {}

        std::vector<uint8_t> to_vec(){
            return data;
        }

        std::string to_string(){
            std::string str(data.size(), '\0');

            for( size_t i = 0; i < data.size(); ++i){
                str[i] = (char)data[i];
            }

            return str;
        }

        bool empty(){
            return data.empty();
        }

        inline bool operator==(const RawData& other){
            return data == other.data;
        }

        inline bool operator!=(const RawData& other){
            return data != other.data;
        }

    private:
        std::vector<uint8_t> data;
};

#endif