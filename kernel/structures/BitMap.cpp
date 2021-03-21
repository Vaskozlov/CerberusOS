#include <BitMap.hpp>

u8 BitMap::operator[](size_t index){
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;

    if ((this->buffer[byteIndex] & bitIndexer) > 0){
        return 1;
    }

    return 0;
}


void BitMap::set(size_t index, u8 value){
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    this->buffer[byteIndex] &= ~bitIndexer;
    
    if (value == 1){
        this->buffer[byteIndex] |= bitIndexer;
    }
}
